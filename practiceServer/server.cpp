#include "server.h"

Server::Server(){
    if(this->listen(QHostAddress::Any, 2323)){
//        ui->label->setText("Server start");
        emit signalStatusServer("Server start");
        qDebug() << "start";
    } else {
//        ui->label->setText("Something happened :(");
//        emit signalStatusServer();
    }
    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
//    ui->statusbar->showMessage("new client");
    qDebug() << "new client on " << socketDescriptor;
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok){
//        ui->statusbar->showMessage("reading...");
        while(true){
            if(nextBlockSize == 0){
                if(socket->bytesAvailable() < 2){
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize){
                break;
            }
            QString str;
            in >> str;
            nextBlockSize = 0;
            SendToClient(str);
            break;
        }
    } else {
//        ui->statusbar->showMessage("Something happened :(");
    }
}

void Server::SendToClient(QString str){
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    for(int i = 0; i < Sockets.size(); i++){
        Sockets[i]->write(Data);
    }
}
