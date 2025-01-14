#ifndef CLIENTSPROCESSINGMANAGER_H
#define CLIENTSPROCESSINGMANAGER_H

///     Класс ClientsProcessingManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с обработками
///     Переменные:
///     Методы:
///     readDataFromStream() - чтение данных с потока
///     writeDataFromStream() - запись данных в поток
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс

class ClientsProcessingManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientsProcessingManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;
signals:
    void signalStatusRRManager(QString status);
    void signalSendToAllClientsRRManager(QString typeOfMsg, QString str);
};

#endif // CLIENTSPROCESSINGMANAGER_H
