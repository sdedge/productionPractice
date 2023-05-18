#include "null_manager.h"

NullManager::NullManager()
{

}

void NullManager::readDataFromStream(QDataStream &inStream)
{

}

void NullManager::writeDataToStream(QDataStream &outStream)
{

}

void NullManager::processData(QDataStream &inStream)
{
    return;
}

QString NullManager::typeOfMessage()
{
    return QString("No type");
}
