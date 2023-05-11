#ifndef NULLMANAGER_H
#define NULLMANAGER_H

///     Класс NullManager реализует интерфейс I_MessageManager
///     обозначает отсутствие обработчика сообщений (заглушку)
///     Переменные:
///     Методы:
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс

class NullManager : public I_MessageManager
{
    Q_OBJECT
public:
    NullManager();
    void processData(QDataStream &in) override;
    QString typeOfMessage() override;
};

#endif // NULLMANAGER_H
