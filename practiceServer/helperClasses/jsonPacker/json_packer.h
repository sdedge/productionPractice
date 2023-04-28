#ifndef JSONPACKER_H
#define JSONPACKER_H

#include <QJsonValue>
#include <QJsonObject>
#include "../../components/frames/cardFrame/I_cardframe.h"

class JsonPacker
{
public:
    JsonPacker();

    QJsonValue getJsonVersionValue(I_CardFrame *item);

private:
    QJsonValue m_settingsCardJsonValue;
};

#endif // JSONPACKER_H
