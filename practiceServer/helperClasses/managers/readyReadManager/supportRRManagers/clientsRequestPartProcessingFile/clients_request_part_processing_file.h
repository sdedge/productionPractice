#ifndef CLIENTSREQUESTPARTPROCESSINGFILE_H
#define CLIENTSREQUESTPARTPROCESSINGFILE_H

///     Класс ClientsRequestPartFileManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с запросами на части файлов
///     Переменные:
///     str - сообщение в консоль
///     Методы:
///     readDataFromStream() - чтение данных с потока
///     writeDataFromStream() - запись данных в поток
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс

class ClientsRequestPartProcessingFile : public I_MessageManager
{
    Q_OBJECT
public:
    ClientsRequestPartProcessingFile();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;
signals:
    void signalStatusRRManager(QString status);
    void signalSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);
    void signalSendPartOfFile(QTcpSocket* socket);

private:
    QString str;

protected:
    friend QDataStream &operator >> (QDataStream &in, ClientsRequestPartProcessingFile &clientsRPPFManager);
    friend QDataStream &operator << (QDataStream &out, ClientsRequestPartProcessingFile &clientsRPPFManager);

};

#endif // CLIENTSREQUESTPARTPROCESSINGFILE_H
