#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
#include <QVector>        //    класс вектора для хранения созданных сокетов

class Server : public QTcpServer{   //  создание класса сервера, унаследованного от QTcpServer'a
    Q_OBJECT
public:
    Server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    void SendToClient(QString str);    //  функция для передачи данных клиенту

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений
};

#endif // SERVER_H
