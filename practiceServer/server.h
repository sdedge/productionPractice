#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
#include <QVector>        //    класс вектора для хранения созданных сокетов

#include <QFile>

class Server : public QTcpServer{   //  создание класса сервера, унаследованного от QTcpServer'a
    Q_OBJECT
public:
    Server(bool &server_started);
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    void SendToClient(QString str);    //  функция для передачи данных клиенту

    quint16 nextBlockSize;
    int fileSize;   //  размер файла
    QString fileName;   //  его название

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений
    void slotReadyFileRead();   //  обработчик полученных от клиента файлов

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
};

#endif // SERVER_H
