#include "server.h"

Server::Server(bool &server_started){
    if(this->listen(QHostAddress::Any, 2323)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
        server_started = true;
        qDebug() << "start";
    } else {
        server_started = false;
    }
    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы
}

void Server::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket;    //  создание нового сокета
    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, индентифицирующее поток ввода-вывода)

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении клиента сервер удалит сокет при первой же возможности

    Sockets.push_back(socket);  //  помещаем сокет в контейнер

    Server::signalStatusServer("new client on " + QString::number(socketDescriptor));
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
                if(socket->bytesAvailable() < 2){   //  и не должен быть меньше 2-х байт
                    qDebug() << "Data < 2, break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Data not full";    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо
            QString str;    //  создаем переменную строки
            in >> str;  //  записываем в нее строку из объекта in, чтобы проверить содержимое
            if(str.startsWith("MESS:")){     //  если у нас есть подстрока-префикс "FILE:"
                Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера
                SendToClient(str.remove(0,5));      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
            } else {    //  отправляется файл
                SendToClient("Файл загружен");
                QFile file(str);     //  определяем файл
                in >> fileSize; //  считываем его название
                char *bytes = new char[fileSize];   //  выделяем байты под файл
                in >> bytes;    //  считываем байты
//                file.setFileName(newDirPath+str);     //  устанавливаем это название файлу
//                file.setFileName(str);

                /// !!! - str хранит в себе название файла, потому что поток данных QDataStream
                /// представляет собой стек. Изначально достав данные в переменную str оказалось,
                /// что оно хранит название файла

                if(file.open(QIODevice::WriteOnly)){
                    file.write(bytes, fileSize);    //  записываем файл
                    //  оформляем чат на стороне Сервера
                    Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+str+"\"");
                    delete[] bytes;
                    file.close();
                }
            }

            nextBlockSize = 0;
            break;

        }
    } else {
        Server::signalStatusServer("Something happened :(");    //  при ошибке чтения сообщения
    }
}

void Server::slotReadyFileRead()
{
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2); //  установка версии, чтобы не было ошибок
    if(in.status() == QDataStream::Ok){ //  если у нас нет ошибок в состоянии работы in
//        ui->statusbar->showMessage("reading...");
        QFile file;     //  определяем файл
        in >> fileName >> fileSize; //  считываем его название
        Server::signalStatusServer(fileName);
//        file.setFileName(fileName);     //  устанавливаем это название файлу
//        if(file.open(QIODevice::WriteOnly)){
//            in << fileSize;     //  записываем размер файла из объекта in
//            char* bytes = new char[fileSize];   //  выделяем байты под файл
//            in << bytes;    //  считываем байты
//            file.write(bytes, fileSize);    //  записываем файл
//            nextBlockSize = 0;
//            //  оформляем чат на стороне Сервера
//            Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file");
//            file.close();
//        }
        //SendToClient(str);  //  отправляем клиенту сообщение

    } else {
        Server::signalStatusServer("Something happened :(");    //  при ошибке чтения сообщения
    }
}

void Server::slotNewSaveDir(QString newDirPath)
{
    this->newDirPath = newDirPath;  //  установили новую директорию
}

void Server::SendToClient(QString str){
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    for(int i = 0; i < Sockets.size(); i++){    //  пробегаемся по всем сокетам и
        Sockets[i]->write(Data);    //  отправляем по соответствующему сокету данные
    }
}
