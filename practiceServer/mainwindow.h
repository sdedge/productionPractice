#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///  ========================    классы для работы сервера
#include <QTcpServer>           //  Работа с сервером
#include <QTcpSocket>           //  работа с сокетами
///  ========================
///
///  ========================   классы для работы с файлаи
#include <QFileDialog>          //  вызов диалогового окна для файлов
#include <QFile>                //  работа с файлами
#include <QDir>                 //  работа с директориями
///  ========================
///
///  ========================   классы для работы с json
#include <QJsonObject>          //  работа с json объектами
#include <QJsonArray>           //  работа с json массивами
#include <QJsonDocument>        //  работа с json документами
#include <QJsonValue>           //  работа с json значениями
#include <QJsonParseError>      //  парсинг json Ошибок
///  ========================
///
///  ========================   классы для работы с элементами формы
#include <QMainWindow>          //  работа главного окна
#include <QListWidgetItem>      //  список виджетов
#include <QAbstractScrollArea>  //  работа со скролящимися областями
#include <QMessageBox>          //  работа со всплывающими окнами
#include <QFileSystemModel>     //  модель файловой системы
#include <QTreeView>            //  отображение в виде дерева
///  ========================
///
///  ========================   свои классы
#include "components/frames/cardFrame/I_cardframe.h"    //  интерфейс работы с карточками настройки
#include "helperClasses/jsonPacker/json_packer.h"       //  упаковщик карточки в json вариант
#include "helperClasses/managers/workspaceManager/workspace_manager.h"  //  менеджер рабочего пространства
#include "server.h" //  работа с сервером

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
    WorkspaceManager *workspaceManager; //  данная сущность отвечает за работу с рабочей директорией

    I_CardFrame *m_selectWorkspaceFrame;
    I_CardFrame *m_possibleProcessingFrame;
    I_CardFrame *m_changePortLineEditFrame;
    I_CardFrame *m_maxConnectionSpinBoxFrame;

    JsonPacker m_jsonPacker;

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

    void updateUiComboBoxSlot(const QString &fileName);

signals:
    void signalNewWorkspaceFolder(QString); //  сигнал для установки новой рабочей папки
//    void signalNewSaveDir(QString);   //  сигнал для обработки директории сохранения
    void signalSocketDisplayed(QTcpSocket* displayedSocket);   //  сигнал для обработки уже отобразившихся сокетов
    void signalDisconnectSocket(int socketDiscriptor);  //  сигнал для принудительного удаления сокета
    void signalSaveSettings();
    void signalUpdatePossibleProcessing(QVariant newPossibleProcessingData);
};

#endif // MAINWINDOW_H
