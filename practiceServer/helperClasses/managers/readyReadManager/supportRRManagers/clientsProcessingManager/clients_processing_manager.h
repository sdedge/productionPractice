#ifndef CLIENTSPROCESSINGMANAGER_H
#define CLIENTSPROCESSINGMANAGER_H

///     Класс ClientsProcessingManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с обработками
///     Переменные:
///     Методы:
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс

class ClientsProcessingManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientsProcessingManager();
    void processData(QDataStream &in) override;
    QString typeOfMessage() override;
};

#endif // CLIENTSPROCESSINGMANAGER_H
