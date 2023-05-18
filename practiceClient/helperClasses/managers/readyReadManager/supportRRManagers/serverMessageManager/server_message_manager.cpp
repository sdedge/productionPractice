#include "server_message_manager.h"

ServerMessageManager::ServerMessageManager()
{

}

void ServerMessageManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->message;
}

void ServerMessageManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->message;
}

void ServerMessageManager::processData(QDataStream &inStream)
{
    inStream >> this->message;
    emit signalMessageRRManager(message);     //  оформляем чат на стороне клиента
    qDebug() << "ServerMessageManager::processData:        " << message;
}

QString ServerMessageManager::typeOfMessage()
{
    return QString("Message");
}

QDataStream &operator >>(QDataStream &in, ServerMessageManager &serverMessageManager){
    in >> serverMessageManager.message;
    return in;
}

QDataStream &operator <<(QDataStream &out, ServerMessageManager &serverMessageManager){
    out << serverMessageManager.message;
    return out;
}
