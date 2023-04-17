#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
///
///  ========================   классы для работы с файлаи
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
///  ========================
///
///  ========================   классы для работы с элементами формы
#include <QListWidgetItem>
#include <QAbstractScrollArea>
#include <QMessageBox>
///  ========================
///
///  ========================   свои классы
#include "components/frames/cardFrame/I_cardframe.h"

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

    I_CardFrame *m_selectWorkspaceFrame;
    I_CardFrame *m_possibleProcessingFrame;
    I_CardFrame *m_changeIPLineEditFrame;
    I_CardFrame *m_maxConnectionSpinBoxFrame;

    QString delimiter = "<font color = black><\\font>=======================<br>";  //  создаем разделитель для сообщений

    QJsonObject m_currentJsonObject;    // Текущий json объект, с которым производится работа
    QJsonValue m_currentJsonValue;
    QJsonArray m_currentJsonArray;

    quint16 nextBlockSize;  //  размер блока сообщения

public slots:
    void slotStatusServer(QString status);  //  обработчик статуса сервера

    void slotAddSocketToListWidget(QTcpSocket* socketToAdd);    //  обработчик отображения добавляемых сокетов

    void slotDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);    //  обработчик удаления отключившихся сокетов

    void slotAddTreatmentToPossibleTreatmentsComboBox(QString treatmentToAdd);  //  обработчик добавления новой обработки

    void slotShowContextMenu(const QPoint &pos);   //  обработчик контекстного меню

    void slotDisconnectClient();
//    void slotChatServer(QString message);   //  обработчик чата на стороне сервера

    void on_chooseWorkspaceDirPushButton_clicked();  //  по нажатию на "Choose save directory"

private slots:
    void on_clientsListWidget_customContextMenuRequested(const QPoint &pos);

    void on_openJSONSettingsFilePushButton_clicked();

//    void on_saveSettingsPushButton_clicked();

    void on_saveSettingsPushButton_clicked();

signals:
    void signalNewSaveDir(QString);   //  слот для обработки директории сохранения
    void signalSocketDisplayed(QTcpSocket* displayedSocket);   //  сигнал для обработки уже отобразившихся сокетов
    void signalDisconnectSocket(int socketDiscriptor);  //  сигнал для принудительного удаления сокета
    void signalSetJSONSettingFilePath(QString); //  слот для установки пути к JSON файлу настроек
    void signalSaveSettings();
};

#endif // MAINWINDOW_H
