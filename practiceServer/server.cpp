#include "server.h"

Server::Server(bool &server_started){
    if(this->listen(QHostAddress::Any, 2323)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
        server_started = true;  //  меняем состояние сервера
        qDebug() << "start";    //  уведомляем в консоли
    } else {
        server_started = false; //  иначе что-то пошло не так
    }
    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы
}

void Server::incomingConnection(qintptr socketDescriptor){  //  обработчик нового подключения
    socket = new QTcpSocket;    //  создание нового сокета под нового клиента
    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, индентифицирующее поток ввода-вывода)

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);  //  связка готовности чтения
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении клиента сервер удалит сокет при первой же возможности

    Sockets.push_back(socket);  //  помещаем сокет в контейнер

    Server::signalStatusServer("new client on " + QString::number(socketDescriptor));   //  уведомление о подключении
    qDebug() << "new client on " << socketDescriptor;
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2); //  установка версии, чтобы не было ошибок
    if(in.status() == QDataStream::Ok){ //  если у нас нет ошибок в состоянии работы in
//        ui->statusbar->showMessage("reading...");
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "nextBlockSize == 0";
                if(socket->bytesAvailable() < 8){   //  и не должен быть меньше 2-х байт
                    qDebug() << "Data < 8, break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(quint64(socket->bytesAvailable()) < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Data not full";    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо
            QString str;    //  создаем переменную строки
            in >> str;  //  записываем в нее строку из объекта in, чтобы проверить содержимое
            if(str.startsWith("MESS:")){     //  если у нас есть подстрока-префикс "MESS:"
                /// По правилам названия файла нельзя использовать двоеточие, поэтому префикс не будет реагировать на название файла
                Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера
                SendToClient("<font color = black><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5));      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
                SendToClient(delimiter);    //  вставляем разделитель
            } else {    //  отправляется файл

                QFile *file = new QFile;     //  определяем файл
                in >> fileSize; //  считываем его размер

                /// !!! - str хранит в себе название файла, потому что поток данных QDataStream
                /// представляет собой стек. Изначально достав данные в переменную str оказалось,
                /// что оно хранит название файла
                qDebug() << str;
                file->setFileName(str);
                QDir::setCurrent("C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\");  //  устанавливаем путь сохранения на рабочем столе

                char block[100];
                int sizeReceivedData = 0;
                qint64 toFile;

                if(file->open(QIODevice::WriteOnly)){
                    while(!in.atEnd()){
                        toFile = in.readRawData(block, sizeof(block));
                        sizeReceivedData += toFile;
                        file->write(block, toFile);
                    }
                    qDebug() << sizeReceivedData << " | " << fileSize;
                    if(sizeReceivedData == fileSize){
                        file->close();
                        file = NULL;
                        fileSize = 0;
                        sizeReceivedData = 0;
                    } else {
                        qDebug() << "File Data not full";
                        break;
                    }
                    //  оформляем чат на стороне Сервера
                    //  уведомление о "кто: какой файл"
                    SendToClient("<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+str+"\"");
                    SendToClient(delimiter);    //  вставляем разделитель
                    Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+str+"\"");
//                    delete[] bytes; //  удаляем из кучи массив
//                    file->close();   //закрываем файл
                }






//                char *bytes = new char[fileSize];   //  выделяем байты под файл
//                in >> bytes;    //  считываем байты
//                file.setFileName(newDirPath+str);     //  устанавливаем это название файлу






            }   //  конец, если отправляется файл

            nextBlockSize = 0;  //  обнуляем для новых сообщений
            break;  //  выходим, делать больше нечего

        }   //  конец while
    } else {
        Server::signalStatusServer("Something happened :(");    //  при ошибке чтения сообщения
    }
}

void Server::slotNewSaveDir(QString newDirPath) //  пока неработающий обработчик новой директории
{
    this->newDirPath = newDirPath;  //  установили новую директорию
}

void Server::SendToClient(QString str){ //  отправка клиенту сообщений
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;   //  записываем в поток размер сообщения и строку
    out.device()->seek(0);  //  в начало потока
    out << quint16(Data.size() - sizeof(quint16));  //  высчитываем размер сообщения
    for(int i = 0; i < Sockets.size(); i++){    //  пробегаемся по всем сокетам и
        Sockets[i]->write(Data);    //  отправляем по соответствующему сокету данные
    }
}
