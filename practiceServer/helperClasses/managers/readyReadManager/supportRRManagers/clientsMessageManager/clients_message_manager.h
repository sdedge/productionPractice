#ifndef CLIENTSMESSAGEMANAGER_H
#define CLIENTSMESSAGEMANAGER_H

///     Класс ClientsMessageManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с обычными сообщениями клиентов
///     Переменные:
///     Методы:
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс

class ClientsMessageManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientsMessageManager();
    void processData(QDataStream &in) override;
    QString typeOfMessage() override;
};

#endif // CLIENTSMESSAGEMANAGER_H
