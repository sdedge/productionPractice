#include "server.h"

Server::Server(bool &server_started){
    if(this->listen(QHostAddress::Any, 2323)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
        server_started = true;  //  меняем состояние сервера
        qDebug() << "start";    //  уведомляем в консоли
    } else {
        server_started = false; //  иначе что-то пошло не так
    }
    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    /// Глоссарий, описывающий тип отправляемого сообщения
    /// первое число определяет тип (0 - простой сигнал о чем-то \ 1 - запрос чего-то)
    /// второе число определяет конец какого-то действия, если оно в несколько этапов, например, передача файла
    /// третье и последующие числа определяют тип передаваемых данных

    mapRequest[""] = "";  //  ничего не нужно
    mapRequest["001"] = "Message";   //  отправляется простое сообщение
    mapRequest["0011"] = "Message from someone";    //  отправляется сообщение от кого-то конкретного
    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)

    fileSystemWatcher = new QFileSystemWatcher;
    fileSystemWatcher->addPath(folderForRawInformation);    //  устанавливаем папку для слежки
    connect(fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFolderForRawInformationChanged(QString)));
}

void Server::slotFolderForRawInformationChanged(const QString &fileName)
{
    QDir workWithDirectory;
    workWithDirectory.cd(fileName);
    workWithDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);     //  устанавливаем фильтр выводимых файлов/папок
    workWithDirectory.setSorting(QDir::Size | QDir::Reversed);  //  устанавливаем сортировку "от меньшего к большему"
    QFileInfoList list = workWithDirectory.entryInfoList();     //  получаем список файлов директории
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));     //  выводим в формате "размер имя"
        qDebug() << "";     // переводим строку
    }
    qDebug() << fileName;
}

void Server::incomingConnection(qintptr socketDescriptor){  //  обработчик нового подключения
    socket = new QTcpSocket;    //  создание нового сокета под нового клиента
    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, идентифицирующее  поток ввода-вывода)
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);  //  связка готовности чтения
//    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении клиента сервер удалит сокет при первой же возможности
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect); //  связка удаления клиента

    Sockets.push_back(socket);  //  помещаем сокет в контейнер    
    Server::signalStatusServer("new client on " + QString::number(socketDescriptor));   //  уведомление о подключении
    Server::signalAddSocketToListWidget(socket);    //  отображаем на форме в clientsListWidget этот сокет
    SendToAllClients(mapRequest["001"], "new client on " + QString::number(socketDescriptor)+delimiter);
    qDebug() << "new client on " << socketDescriptor;
    qDebug() << "push quantity of clients: "+QString::number(Sockets.length());
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2); //  установка версии, чтобы не было ошибок
    if(in.status() == QDataStream::Ok){ //  если у нас нет ошибок в состоянии работы in
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "nextBlockSize == 0";
                qDebug() << "size of waiting bytes" << socket->bytesAvailable();   //  выводим размер ожидающих байтов
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

            QString typeOfMess;
            in >> typeOfMess;   //  считываем тип сообщения

            if(typeOfMess == "Message"){     //  если тип данных "Message"
                QString str;    //  создаем переменную строки
                in >> str;  //  записываем в нее строку из объекта in, чтобы проверить содержимое
                Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера

                SendToAllClients(mapRequest["001"],"<font color = black><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5)+delimiter);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
            }

            if(typeOfMess == "Message from someone"){   //  если сообщение с конкретным отправителем
                QString str, someone;    //  создаем переменную строки и отправителя
                in >> str >> someone;   //  считываем
                Server::signalStatusServer(someone+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера
                SendToAllClients(mapRequest["001"],"<font color = black><\\font>"+someone+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5)+delimiter);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
            }

            if(typeOfMess == "File"){    //  отправляется файл

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
                    QDir::setCurrent(newDirPath);  //  устанавливаем путь сохранения на рабочем столе
                    Server::signalStatusServer("Файл "+fileName+" создан на сервере");  //  уведомляем

                    SendToOneClient(socket, mapRequest["102"],"Downloading new part of file...");    //  запрашиваем первую часть файла
                }
            }

            if(typeOfMess == "Request part of file"){   //  отправляется часть файла
                if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
                    blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
                }

                bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

                in >> bytes;    //  считываем байты

                if(file->open(QIODevice::Append)){  //  записываем в конец файла
                    file->write(bytes, blockData);    //  записываем в файл
                } else {
                    Server::signalStatusServer("Не удается открыть файл "+fileName);
                }

                if(file->size() < fileSize){    //  если размер до сих пор не полон
                    Server::signalStatusServer("Текущий размер файла "+fileName+" от "+QString::number(socket->socketDescriptor())+" = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize));

                    //  SendToAllClients(mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем новую часть файл
                    SendToOneClient(socket, mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем первую часть файла
                } else {
                    //  оформляем чат на стороне Сервера
                    //  уведомление о "кто: какой файл" при сигнале "012" - File downloaded
                    Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\"");
                    //  SendToAllClients(mapRequest["012"],"<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

                    SendToOneClient(socket, mapRequest["012"], "<font color = green><\\font>file \""+fileName+"\" downloaded \n"+delimiter);

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

            }   //  конец, если отправляется файл

            nextBlockSize = 0;  //  обнуляем для новых сообщений
            break;  //  выходим, делать больше нечего
        }   //  конец while
    } else {
        Server::signalStatusServer("Something happened :(");    //  при ошибке чтения сообщения
    }
}

void Server::slotDisconnect()
{
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());
    Sockets.removeOne(disconnectedSocket);
    qDebug() << "pop quantity of clients: "+QString::number(Sockets.length());
    SendToAllClients(mapRequest["001"], "<font color = red><\\font>User  "+disconnectedSocket->localAddress().toString()+": has disconnected \n"+delimiter);
    Server::signalDeleteSocketFromListWidget(disconnectedSocket);
    disconnectedSocket->deleteLater();  //  оставляем удаление сокета программе
}

void Server::slotNewSaveDir(QString newDirPath) //  пока неработающий обработчик новой директории
{
    this->newDirPath = newDirPath;  //  установили новую директорию
}

void Server::SendToAllClients(QString typeOfMsg, QString str){ //  отправка клиенту сообщений
    Data.clear();   //  может быть мусор

    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << typeOfMsg << str;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    for(int i = 0; i < Sockets.size(); i++){    //  пробегаемся по всем сокетам и
        Sockets[i]->write(Data);    //  отправляем по соответствующему сокету данные
    }
}

void Server::SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str)
{
    Data.clear();   //  может быть мусор

    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << typeOfMsg << str;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    socket->write(Data);    //  отправляем по сокету данные
}

