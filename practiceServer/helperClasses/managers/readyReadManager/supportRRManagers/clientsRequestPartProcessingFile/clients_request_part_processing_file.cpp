#include "clients_request_part_processing_file.h"

ClientsRequestPartProcessingFile::ClientsRequestPartProcessingFile()
{

}

void ClientsRequestPartProcessingFile::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->str;
}

void ClientsRequestPartProcessingFile::writeDataToStream(QDataStream &outStream)
{
    outStream << this->str;
}

void ClientsRequestPartProcessingFile::processData(QDataStream &inStream, QTcpSocket *socket)
{
    qDebug() << "ClientsRequestPartProcessingFile::processData:     str = " << str;  //  выводим в консоль
    emit signalStatusRRManager(str); //  выводим клиенту

    emit signalSendPartOfFile(socket);   //  вызываем соответствующий метод отправки
}

QString ClientsRequestPartProcessingFile::typeOfMessage()
{
    return QString("Request part of processing file");
}
