#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
#include <QVector>        //    класс вектора для хранения созданных сокетов

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

class Server : public QTcpServer{   //  создание класса сервера, унаследованного от QTcpServer'a
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    void SendToClient();    //  функция для передачи данных клиенту

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений
};

#endif // MAINWINDOW_H
