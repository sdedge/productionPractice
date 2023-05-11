#include "null_manager.h"

NullManager::NullManager()
{

}

void NullManager::processData(QDataStream &in)
{
    return;
}

QString NullManager::typeOfMessage()
{
    return QString("No type");
}
