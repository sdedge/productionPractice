#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /// Глоссарий, описывающий тип отправляемого сообщения
    /// первое число определяет тип (0 - простой сигнал о чем-то \ 1 - запрос чего-то)
    /// второе число определяет конец какого-то действия, если оно в несколько этапов, например, передача файла
    /// третье и последующие числа определяют тип передаваемых данных

    mapRequest[""] = "";  //  ничего не нужно
    mapRequest["000"] = "Disconnect";   //  сигнал на отключение
    mapRequest["001"] = "Message";   //  отправляется простое сообщение
    mapRequest["0011"] = "Message from someone";    //  отправляется сообщение от кого-то конкретного
    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
    mapRequest["103"] = "Request part of processing file";  //  запрос на еще одну часть обрабатываемого файла
    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)
    mapRequest["004"] = "Possible treatments ComboBox data";    //  отправка данных по доступным обработкам
    mapRequest["0041"] = "Set treatment on client";     //  закрепление возможной обработки за сокетом


    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    completer = new QCompleter(this);   //  создаем completer

    fModel = new QFileSystemModel();    //  создаем модель системы
    fModel->setRootPath(QDir::currentPath() + "\\");    //  устанавливаем разделитель

    completer->setModel(fModel);    //  completer'у устанавливаем модель

    completer->setCompletionMode(QCompleter::CompletionMode(0));    //  подсказки во всплывающем окне (2 - как выделенный текст, идущий далее)
    completer->setModelSorting(QCompleter::ModelSorting(2));    //  НЕ чувствительно к регистру (0 -  не сортировать, 1 - чувствительно к регистру)

    ui->filePathLineEdit->setCompleter(completer);  //  устанавливам completer

    fileSystemWatcher = new QFileSystemWatcher;
    fileSystemWatcher->addPath(rawInformationDirectory);    //  устанавливаем папку для слежки
    connect(fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFolderForRawInformationChanged(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectToServerPushButton_clicked()
{
    //  TODO: сделать программным способом задание значений для подключения
    if(ui->IPLineEdit->text().isEmpty()){
        ui->filePathLabel->setText("IP is empty!");
        return;
    }
    if(ui->PortLineEdit->text().isEmpty()){
        ui->filePathLabel->setText("Port is empty!");
        return;
    }
    socket = new QTcpSocket();  //  при подключении создаем новый сокет
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении сокет сам удалится

    socket->connectToHost(ui->IPLineEdit->text(), ui->PortLineEdit->text().toUInt());   //  подключение к серверу (локальный адрес + порт такой же, как у сервера)
    if(!(socket->isOpen())){
        ui->filePathLabel->setText("Check your IP and host! You're offline");
        return;
    }
        //    if(socket->bind(ui->IPLineEdit->text(), ui->PortLineEdit->text()))
    //    if(socket->state()==QTcpSocket::ConnectedState){     //  если подключились
        ui->filePathLabel->clear();
        ui->filePathLineEdit->setEnabled(true); //  то включаем интерфейс
        ui->lineEdit->setEnabled(true);
        ui->openFilePushButton->setEnabled(true);
        ui->sendFilePushButton->setEnabled(true);
        ui->sendMsgPushButton->setEnabled(true);
        ui->textBrowser->setEnabled(true);
        ui->callMeLineEdit->setEnabled(true);
        ui->chooseTreatmentComboBox->setEnabled(true);
        ui->chooseTreatmentPushButton->setEnabled(true);

        ui->connectToServerPushButton->setEnabled(false);   //  и гасим кнопку подключения
    //    } else {
    //        ui->filePathLabel->setText("No conection");
    //    }
        ui->textBrowser->append("You are online!"+delimiter);
}

void MainWindow::SendTextToServer(QString str)
{
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
//    if(!ui->callMeLineEdit->text().isEmpty()){
        out <<  quint64(0) << QString("Message") << str << ui->callMeLineEdit->text();   //  собираем сообщение из размер_сообщения << тип_сообщения << строка << отправитель
//    } else {
//        out << quint64(0) << mapRequest["001"] << str;   //  собираем сообщение из размер_сообщения << тип_сообщения << строка
//    }
    out.device()->seek(0);  //  передвигаемся в начало
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    socket->write(Data);    //  записываем данные в сокет

    ui->lineEdit->clear();  //  чистим lineEdit после отправки сообщения
}

void MainWindow::SendPartOfFile()
{
    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
    }

    socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались
    Data.clear();

    qDebug() << "read " << file->read(bytes, blockData);     //  читаем файл и записываем данные в байты
    qDebug() << "block: "+QString::number(blockData);   //  нужно, чтобы видеть текущий размер блоков


    QByteArray buffer;
    buffer = buffer.fromRawData(bytes, blockData);

    qDebug() << "block size" << blockData << "buffer size" << buffer.size();

    QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << QString("File") << buffer;   //  отправляем байты
    out.device()->seek(0);
    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    qDebug() << "sending blockSize = " << quint64(Data.size() - sizeof(quint64));
    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
    socket->write(Data);
    qDebug() << "Data size = " << Data.size();
}

void MainWindow::SendToServer(QString typeOfMsg, QString str)   //  отдельная функция отправки сообщения, чтобы она не пересекалась с другими
{
    Data.clear();   //  может быть мусор

    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << typeOfMsg << str;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    socket->write(Data);    //  отправляем данные на сервер
}

void MainWindow::BlockingInterface()
{
    ui->filePathLabel->clear();
    ui->filePathLineEdit->setEnabled(false); //  то включаем интерфейс
    ui->lineEdit->setEnabled(false);
    ui->openFilePushButton->setEnabled(false);
    ui->sendFilePushButton->setEnabled(false);
    ui->sendMsgPushButton->setEnabled(false);
    ui->textBrowser->setEnabled(false);
    ui->callMeLineEdit->setEnabled(false);
    ui->chooseTreatmentComboBox->setEnabled(false);
    ui->chooseTreatmentPushButton->setEnabled(false);

    ui->connectToServerPushButton->setEnabled(true);   //  и гасим кнопку подключения
}

void MainWindow::SendFileToServer(QString filePath) //  метод отправки файла серверу (его начало)
{
    Data.clear();   //  чистим массив байт от мусора
    file = new QFile(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными
    fileSize = file->size();     //  определяем размер файла
    QFileInfo fileInfo(file->fileName());    //  без этой строки название файла будет хранить полный путь до него
    fileName = fileInfo.fileName();     //  записываем название файла
    ui->filePathLabel->setText("Size: "+QString::number(fileSize)+" Name: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его открыть

    if(fileSize < blockData){   //  если размер файла меньше выделенного блока
        blockData = fileSize;
    } else {    //  если мы еще раз отправляем какой-нибудь файл
        blockData = 1000000;  //  возвращаем дефолтное значение
    }
    bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    if(file->open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
        socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались

        QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
        out.setVersion(QDataStream::Qt_6_2);
        out << quint64(0) << QString("File") << fileName << fileSize;   //  отправляем название файла и его размер
        out.device()->seek(0);
        //  избавляемся от зарезервированных двух байт в начале каждого сообщения
        out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
        qDebug() << "sending data size: " << Data.size() - sizeof(quint64);
        socket->write(Data);
    } else {
        ui->filePathLabel->setText("File not open :(");
    }
}


void MainWindow::slotReadyRead()
{
    ///     mapRequest[""] = "";  //  ничего не нужно
    ///    mapRequest["001"] = "Message";   //  отправляется простое сообщение
    ///    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
    ///    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
    ///    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)

    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "nextBlockSize == 0";
                if(socket->bytesAvailable() < 8){   //  и не должен быть меньше 8-и байт
                    qDebug() << "Data < 8, break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Data not full | socket->bytesAvailable() = "+QString::number(socket->bytesAvailable()) + " | nextBlockSize = "+QString::number(nextBlockSize);    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо

            QString typeOfMessage;
            in >> typeOfMessage;    //  определение типа сообщения

            if(typeOfMessage == "Message"){ //  сервер прислал сообщение
                QString str;    //  определяем переменную, в которую сохраним данные
                in >> str;  //  выводим в переменную сообщение
                qDebug() << str;
                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str);   //  выводим полученное сообщение на экран
            }

            if(typeOfMessage == "File"){    //  отправляется файл

                // mapRequest["002"] << fileName << fileSize

                if(fileName.isEmpty()){    //  если файла не существует
                    in >> fileName;  //  записываем из потока название файла
                    in >> fileSize; //  считываем его размер

                    if(fileSize < blockData){   //  если размер файла меньше выделенного блока
                        blockData = fileSize;   //  устанавливаем размер блока ровно по файлу (передача произойдет в один этап)
                    } else {
                        blockData = 1000000;  //  устанавливаем по умолчанию (на случай последующей передачи, если размер файла будет куда больше)
                    }

                    file = new QFile;     //  определяем файл
                    file->setFileName(fileName);    //  устанавливаем имя файла
                    QDir::setCurrent(rawInformationDirectory);  //  устанавливаем путь сохранения на рабочем столе


                    SendToServer(mapRequest["103"],"Downloading new part of processing file to "+socket->localAddress().toString()+"...");    //  запрашиваем первую часть файла
                }
            }

            if(typeOfMessage == "Request part of file"){    //  если серверу нужна еще одна часть файла
                QString str;    //  определяем переменную, в которую сохраним уведомление от запроса
                in >> str;  //  выводим в переменную сообщение

                qDebug() << str;  //  выводим в консоль
                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str); //  выводим клиенту

                nextBlockSize = 0;  //  заранее обнуляем размер сообщения
                SendPartOfFile();   //  вызываем соответствующий метод отправки
            }

            if(typeOfMessage == "Request part of processing file"){
                if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
                    blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
                }

                bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

                in >> bytes;    //  считываем байты

                if(file->open(QIODevice::Append)){  //  записываем в конец файла
                    file->write(bytes, blockData);    //  записываем в файл
                } else {
                    qDebug() << "Не удается открыть файл "+fileName;
                }

                if(file->size() < fileSize){    //  если размер до сих пор не полон
                    qDebug() << "Текущий размер файла "+fileName+" от "+QString::number(socket->socketDescriptor())+" = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize);

                    //  SendToAllClients(mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем новую часть файл
                    SendToServer(mapRequest["103"],"<font color = black><\\font>Downloading new part of processing file...<font color = black><\\font>");    //  запрашиваем первую часть файла
                } else {
                    //  оформляем чат на стороне Сервера
                    //  уведомление о "кто: какой файл" при сигнале "012" - File downloaded
                    qDebug() << "Server: send file by name \""+fileName+"\"";
                    //  SendToAllClients(mapRequest["012"],"<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

                    SendToServer(mapRequest["012"], "<font color = green><\\font>file \""+fileName+"\" downloaded \n"+delimiter);

                    //  TODO: при отправке всем происходит баг в задержке сообщений. решить
                    //  SendToAllClients(mapRequest["001"], "<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

                    file->close();  //  закрываем файл
                    file = nullptr; //  удаляем файл
                    fileName.clear();   //  очищаем его название
                    fileSize = 0;   //  очищаем его размер
                    blockData = 1000000;  //  устанавливаем прежний размер байтов
                    delete[] bytes; //  удаляем байты из кучи
                    nextBlockSize = 0;  //  обнуляем для новых сообщений

                    return; //  выходим
                }

                file->close();   //закрываем файл
                if(bytes != nullptr){   //  удаляем байты из кучи, делая проверку на случай двойного удаления
                    delete[] bytes;
                    bytes = nullptr;
                }
                nextBlockSize = 0;  //  обнуляем для новых сообщений
            }

            if(typeOfMessage == "File downloaded"){ //  если файл полностью скачался
                QString str;    //  определяем переменную, в которую сохраним данные
                in >> str;  //  выводим в переменную сообщение
                qDebug() << "File "+fileName+" downloaded";   //  выводим консоль, какой файл был загружен
                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str);  //  и то же самое клиенту
                file->close();
                delete file; //  удаляем файл
                file = nullptr;
                fileName.clear();   //  очищаем его название
                delete[] bytes; //  удаляем байты из кучи
                nextBlockSize = 0;  //  обнуляем для новых сообщений
            }

            if(typeOfMessage == "Possible treatments ComboBox data"){
                possibleTreatments.clear();
                ui->chooseTreatmentComboBox->clear();
                in >> possibleTreatments;  //  выводим в глобальную переменную map из доступных обработок
                qDebug() << possibleTreatments;

                for(auto item = possibleTreatments.begin(); item != possibleTreatments.end(); ++item)
                {
                    //  Вставляем в комбобокс "в конец, читаемый текст, префикс"
                    ui->chooseTreatmentComboBox->insertItem(ui->chooseTreatmentComboBox->count(), item.value(), item.key());
                }
                nextBlockSize = 0;  //  обнуляем для новых сообщений
            }

            if(typeOfMessage == "Disconnect"){
                QString str;
                in >> str;

                qDebug() << "Disconnect";
                ui->textBrowser->append(str);
                BlockingInterface();
                socket->disconnectFromHost();
            }

            nextBlockSize = 0;  //  обнуляем для новых сообщений
            if(socket->bytesAvailable() == 0){
                break;  //  выходим, делать больше нечего
            }
        }   //  конец whil'a
    } else {    //  если произошли проблемы с подключением
        ui->textBrowser->append("Error connection");    //  уведомление клиента
    }
}

void MainWindow::slotFolderForRawInformationChanged(const QString &folderName)
{
    QDir workWithDirectory;     //  создаем рабочую директорию
    workWithDirectory.cd(folderName); //  переходим в нужный каталог

    QStringList filters;    //  создаем список для фильтра
    filters << "processed_*.txt";  //  заполняем

    workWithDirectory.setNameFilters(filters);  //  устанавливаем фильтр
    workWithDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);     //  устанавливаем фильтр выводимых файлов/папок
    workWithDirectory.setSorting(QDir::Size | QDir::Reversed);  //  устанавливаем сортировку "от меньшего к большему"
    QFileInfoList list = workWithDirectory.entryInfoList();     //  получаем список файлов директории
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName()).arg(fileInfo.filePath()));   //  выводим в формате "размер имя"
        SendFileToServer(fileInfo.filePath());  //  отправляем файл клиенту
    }
    qDebug() << folderName;
    qDebug() << "================";     // переводим строку
}

void MainWindow::on_sendMsgPushButton_clicked() //  по нажатию на "Send msg"
{
    if(!ui->lineEdit->text().isEmpty()){    //  проверка на то, НЕ пустое ли сообщение
        ui->filePathLabel->clear();     //  чистим от предыдущего уведомления
        SendTextToServer("MESS:"+ui->lineEdit->text());     //  отправляем сообщение со служебным префиксом
    } else {
        ui->filePathLabel->setText("Your msg is empty!");   //  уведомляем о пустом сообщении
    }
}


void MainWindow::on_lineEdit_returnPressed()    //  сообщение также отправится, если нажать клавишу Enter
{
    if(!ui->lineEdit->text().isEmpty()){    //  проверка на то, НЕ пустое ли сообщение
        ui->filePathLabel->clear();     //  чистим от предыдущего уведомления
        SendTextToServer("MESS:"+ui->lineEdit->text()); //  отправляем сообщение со служебным префиксом
    } else {
        ui->filePathLabel->setText("Your msg is empty!");   //  уведомляем о пустом сообщении
    }
}


void MainWindow::on_openFilePushButton_clicked()    //  по нажатию на "Or open file"
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла", "C:\\");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    ui->filePathLineEdit->setText(filePath);   //  устанавливаем путь в LineEdit для наглядности
}


void MainWindow::on_sendFilePushButton_clicked()    //  по нажатию на кнопку "Send file"
{
    QString filePath;   //  определяем наш путь к файлу
    filePath = ui->filePathLineEdit->text();    //  сохраняем в переменную путь к файлу
    ui->filePathLineEdit->clear();  //  очищаем поле ввода пути файла после выбора нажатии отправки

    SendFileToServer(filePath); //  отправляем серверу файл
}


void MainWindow::on_chooseTreatmentPushButton_clicked() //  по нажатию на кнопку "Choose treatment"
{
    SendToServer(mapRequest["0041"], ui->chooseTreatmentComboBox->currentData().toString());   //  отправляем серверу текущий текст в комбобоксе
    ui->textBrowser->append("Selected "+ui->chooseTreatmentComboBox->currentData().toString()+delimiter);   //  пишем клиенту, что он выбрал
}

