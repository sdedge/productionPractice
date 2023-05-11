#include "clients_message_manager.h"
#include <QDebug>

ClientsMessageManager::ClientsMessageManager()
{

}

void ClientsMessageManager::processData(QDataStream &in)
{
    QString str;    //  создаем переменную строки
    in >> str;  //  записываем в нее строку из объекта in, чтобы проверить содержимое
//    Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера

//    SendToAllClients(mapRequest["001"],"<font color = black><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5)+delimiter);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
    qDebug() << "ClientsMessageManager::processData:        " << str;
}

QString ClientsMessageManager::typeOfMessage()
{
    return QString("Message");
}
