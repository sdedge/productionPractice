#include "possible_processing_manager.h"

#include <QDataStream>

PossibleProcessingManager::PossibleProcessingManager()
{

}

void PossibleProcessingManager::readDataFromStream(QDataStream &inStream)
{

}

void PossibleProcessingManager::writeDataToStream(QDataStream &outStream)
{

}

void PossibleProcessingManager::processData(QDataStream &inStream)
{
    inStream >> this->possibleProcessingData;
    emit signalStatusRRManager("Отправка "+QString::number(possibleProcessingData.size())+" обработок");     //  оформляем чат на стороне клиента
    emit signalSetCBDataRRManager(possibleProcessingData);
    qDebug() << "ServerMessageManager::processData:        possibleProcessingData.size():" << possibleProcessingData.size();
}

QString PossibleProcessingManager::typeOfMessage()
{
    return QString("Possible processing ComboBox data");
}
