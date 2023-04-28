#include "json_packer.h"

JsonPacker::JsonPacker()
{

}

QJsonValue JsonPacker::getJsonVersionValue(I_CardFrame *item)
{
    //  метод item->getValue() возвращает QMap<QString, QVariant>, состоящий из одного элемента
    //  .first() возвращает первое значение (QVariant)

    if(QString(item->metaObject()->className()).contains("ComboBox")){
        //  картеж из текста : данные
        m_settingsCardJsonValue = item->getValue().first().toJsonObject();

        return m_settingsCardJsonValue;
    }

    if(QString(item->metaObject()->className()).contains("SpinBox")){
        m_settingsCardJsonValue = item->getValue().first().toInt();

        return m_settingsCardJsonValue;
    }

    //  Если у нас ничего не подошло, то возвращаемое значение имеет формат строки
    return item->getValue().first().toString();
}
