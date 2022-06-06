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

    QTcpSocket *socket;

private:
    Ui::MainWindow *ui;
    QTcpServer* server;

    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    void SendToClient(QString str);    //  функция для передачи данных клиенту

    quint16 nextBlockSize;

public slots:
    void slotStatusServer(QString status);

//    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotNewConnection();
    void slotReadyRead();   //  обработчик полученных от клиента сообщений

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера

};

#endif // MAINWINDOW_H
