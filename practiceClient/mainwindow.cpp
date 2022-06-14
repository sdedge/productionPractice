#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();  //  при подключении создаем новый сокет

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении сокет сам удалится

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    completer = new QCompleter(this);   //  создаем completer

    fModel = new QFileSystemModel();    //  создаем модель системы
    fModel->setRootPath(QDir::currentPath() + "\\");    //  устанавливаем разделитель

    completer->setModel(fModel);    //  completer'у устанавливаем модель

    completer->setCompletionMode(QCompleter::CompletionMode(0));    //  подсказки во всплывающем окне (2 - как выделенный текст, идущий далее)
    completer->setModelSorting(QCompleter::ModelSorting(2));    //  НЕ чувствительно к регистру (0 -  не сортировать, 1 - чувствительно к регистру)

    ui->filePathLineEdit->setCompleter(completer);  //  устанавливам completer
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectToServerPushButton_clicked()
{
    //  TODO: сделать программным способом задание значений для подключения

    socket->connectToHost("127.0.0.1", 2323);   //  подключение к серверу (локальный адрес + порт такой же, как у сервера)
//    if(socket->state()==QTcpSocket::ConnectedState){     //  если подключились
        ui->filePathLineEdit->setEnabled(true); //  то включаем интерфейс
        ui->lineEdit->setEnabled(true);
        ui->openFilePushButton->setEnabled(true);
        ui->sendFilePushButton->setEnabled(true);
        ui->sendMsgPushButton->setEnabled(true);
        ui->textBrowser->setEnabled(true);
//    } else {
//        ui->filePathLabel->setText("No conection");
//    }


}

void MainWindow::SendToServer(QString str)
{
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
    out << quint16(0) << str;   //  собираем сообщение из размер_сообщения << время_отправки << строка
    out.device()->seek(0);  //  передвигаемся в начало
    out << quint16(Data.size() - sizeof(quint16));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    socket->write(Data);    //  записываем данные в сокет

    ui->lineEdit->clear();  //  чистим lineEdit после отправки сообщения
}

void MainWindow::SendFileToServer(QString filePath)
{
    Data.clear();   //  чистим массив байт от мусора
    QFile file(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными
    fileSize = file.size();     //  определяем размер файла
    QFileInfo fileInfo(file.fileName());    //  без этой строки название файла будет хранить полный путь до него
    fileName = fileInfo.fileName();     //  записываем название файла
    ui->filePathLabel->setText("Size: "+QString::number(fileSize)+" Name: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его открыть
    char *bytes = new char[fileSize];   //  выделяем в куче байты под файл
    if(file.open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
        file.read(bytes, fileSize);     //  читаем файл и записываем данные в байты
        file.close();                   //  закрываем файл
        QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
        out.setVersion(QDataStream::Qt_6_2);
        out << quint16(0);   //  пока сообщение оправлено, мы не можем определить размер блока
        out.device()->seek(0);
        //  избавляемся от зарезервированных двух байт в начале каждого сообщения
        out << quint16(Data.size() - sizeof(quint16)) << fileName << fileSize << bytes;   //  отправляем наше название файла, размер и байты
        out.device()->seek(0);
        socket->write(Data);
    } else {
        ui->filePathLabel->setText("File not open :(");
    }
    delete[] bytes; //  удаляем из кучи массив байт



//    Data.clear();   //  чистим массив байт от мусора
////    QFile file(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными


//    file = new QFile(filePath);
//    fileSize = file->size();     //  определяем размер файла
//    QFileInfo fileInfo(file->fileName());    //  без этой строки название файла будет хранить полный путь до него
//    fileName = fileInfo.fileName();     //  записываем название файла
//    ui->filePathLabel->setText("Size: "+QString::number(fileSize)+" Name: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его открыть

////    char *bytes = new char[fileSize];   //  выделяем в куче байты под файл
//    if(file->open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
////        file.read(bytes, fileSize);     //  читаем файл и записываем данные в байты
//        char block[100];

//        QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
//        out.setVersion(QDataStream::Qt_6_2);
//        out << quint64(0) << fileName << fileSize;   //  пока сообщение оправлено, мы не можем определить размер блока, отправляем наше название файла, размер и байты
//        while(!file->atEnd()){
//            quint16 nextBlockFile = file->read(block, sizeof(block));
//            out << nextBlockFile;
//        }

//        out.device()->seek(0);
//        //  избавляемся от зарезервированных двух байт в начале каждого сообщения
//        out << quint64(Data.size() - sizeof(quint64));// << bytes;   //  записываем размер всего сообщения
//        socket->write(Data);
////        socket->waitForBytesWritten();

//        file->close();                   //  закрываем файл
//        delete file;
////        delete[] bytes; //  удаляем из кучи массив байт
//    } else {
//        ui->filePathLabel->setText("File not open :(");
//    }
}


void MainWindow::slotReadyRead()
{
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "nextBlockSize == 0";
                if(socket->bytesAvailable() < 2){   //  и не должен быть меньше 2-х байт
                    qDebug() << "Data < 2       , break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Data not full";    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо
            QString str;    //  определяем переменную, в которую сохраним данные
            in >> str;  //  выводим в переменную сообщение
            nextBlockSize = 0;  //  обнуляем размер блока для последующего
            ui->textBrowser->append(str);   //  выводим полученное сообщение на экран
        }
    } else {
        ui->textBrowser->append("Error connection");
    }
}

void MainWindow::on_sendMsgPushButton_clicked() //  по нажатию на "Send msg"
{
    if(!ui->lineEdit->text().isEmpty()){    //  проверка на то, НЕ пустое ли сообщение
        ui->filePathLabel->clear();     //  чистим от предыдущего уведомления
        SendToServer("MESS:"+ui->lineEdit->text());     //  отправляем сообщение со служебным префиксом
    } else {
        ui->filePathLabel->setText("Your msg is empty!");   //  уведомляем о пустом сообщении
    }
}


void MainWindow::on_lineEdit_returnPressed()    //  сообщение также отправится, если нажать клавишу Enter
{
    SendToServer("MESS:"+ui->lineEdit->text()); //  отправляем сообщение со служебным префиксом
}


void MainWindow::on_openFilePushButton_clicked()    //  по нажатию на "Or open file"
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла", "C:\\");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    ui->filePathLineEdit->setText(filePath);   //  устанавливаем путь в LineEdit для наглядности
}


void MainWindow::on_sendFilePushButton_clicked()    //  по нажатию на "Send file"
{
    QString filePath;   //  определяем наш путь к файлу
    filePath = ui->filePathLineEdit->text();    //  сохраняем в переменную путь к файлу
    ui->filePathLineEdit->clear();  //  очищаем поле ввода пути файла после выбора нажатии отправки

    SendFileToServer(filePath); //  отправляем серверу файл
}

