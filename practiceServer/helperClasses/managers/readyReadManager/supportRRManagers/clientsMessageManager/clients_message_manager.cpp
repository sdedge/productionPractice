#include "clients_message_manager.h"

ClientsMessageManager::ClientsMessageManager()
{

}

void ClientsMessageManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->message;
    inStream >> this->senderName;
}

void ClientsMessageManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->message;
    outStream << this->senderName;
}

void ClientsMessageManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    inStream >> this->message;
    inStream >> this->senderName;
    emit signalStatusRRManager("User "+senderName+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+message);     //  оформляем чат на стороне Сервера
    emit signalSendToAllClientsRRManager("Message","<font color = black><\\font>User "+senderName+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+message.remove(0,5));      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
    qDebug() << "ClientsMessageManager::processData:        " << message << "from" << senderName;
}

QString ClientsMessageManager::typeOfMessage()
{
    return QString("Message");
}

QDataStream &operator >>(QDataStream &in, ClientsMessageManager &clientsMessageManager){
    in >> clientsMessageManager.message;
    in >> clientsMessageManager.senderName;
    return in;
}

QDataStream &operator <<(QDataStream &out, ClientsMessageManager &clientsMessageManager){
    out << clientsMessageManager.message;
    out << clientsMessageManager.senderName;
    return out;
}
