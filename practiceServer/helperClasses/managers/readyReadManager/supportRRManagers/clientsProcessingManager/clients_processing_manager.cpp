#include "clients_processing_manager.h"

ClientsProcessingManager::ClientsProcessingManager()
{

}

void ClientsProcessingManager::readDataFromStream(QDataStream &inStream)
{

}

void ClientsProcessingManager::writeDataToStream(QDataStream &outStream)
{

}

void ClientsProcessingManager::processData(QDataStream &inStream, QTcpSocket *socket)
{

}

QString ClientsProcessingManager::typeOfMessage()
{
    return QString("Set processing on client");
}
