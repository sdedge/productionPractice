#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
#include <QVector>        //    класс вектора для хранения созданных сокетов

#include <QFileDialog>

#include <QListWidgetItem>

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
    Server* server;     //  создаем экземпляр сервера

    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером
    QString delimiter = "<font color = black><\\font>=======================<br>";  //  создаем разделитель для сообщений

    void SendToClient(QString str);    //  функция для передачи данных клиенту

    quint16 nextBlockSize;  //  размер блока сообщения

public slots:
    void slotStatusServer(QString status);  //  обработчик статуса сервера

    void slotAddSocketToListWidget(QTcpSocket* socketToAdd);    //  обработчик отображения добавляемых сокетов

    void slotDeleteSocketFromListWidget(QTcpSocket* socketToDelete);    //  обработчик удаления отключившихся сокетов

    void slotAddTreatmentToPossibleTreatmentsComboBox(QString treatmentToAdd);  //  обработчик добавления новой обработки

    void slotShowContextMenu(const QPoint &pos);   //  обработчик контекстного меню

    void slotDisconnectClient();
//    void slotChatServer(QString message);   //  обработчик чата на стороне сервера

private slots:
    void on_chooseSaveDirPushButton_clicked();  //  по нажатию на "Choose save directory"

    void on_clientsListWidget_customContextMenuRequested(const QPoint &pos);

signals:
    void signalNewSaveDir(QString);   //  слот для обработки директории сохранения
    void signalSocketDisplayed(QTcpSocket* displayedSocket);   //  сигнал для обработки уже отобразившихся сокетов
    void signalDisconnectSocket(int socketDiscriptor);  //  сигнал для принудительного удаления сокета
};

#endif // MAINWINDOW_H
