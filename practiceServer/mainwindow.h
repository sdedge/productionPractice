#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
#include <QVector>        //    класс вектора для хранения созданных сокетов

#include <QFileDialog>

#include "server.h"

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
    Server* server;

    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    void SendToClient(QString str);    //  функция для передачи данных клиенту

    quint16 nextBlockSize;

public slots:
    void slotStatusServer(QString status);

private slots:
    void on_chooseSaveDirPushButton_clicked();

signals:
    void signalNewSaveDir(QString);   //  слот для обработки директории сохранения
};

#endif // MAINWINDOW_H
