#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

///  ========================   классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
///
///  ========================   классы для работы с файлами
#include <QFile>                //  для работы с файлами
#include <QDir>                 //  для работы с директорией
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
///  ========================
///
///  ========================   дополнения
#include <QVector>              //  класс вектора для хранения созданных сокетов
#include <QTime>                //  время
#include <QMap>                 //  определение глоссария для приходящих данных сокета
#include <QRandomGenerator>     //  генератор случайных чисел
///  ========================
///
///  ========================   классы проекта
#include "helperClasses/managers/processingManager/processing_manager.h"    //  класс для распределения файлов на обработчиков
#include "helperClasses/managers/readyReadManager/ready_read_manager.h"     //  класс для работы слота ReadyRead
#include "helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h"    //  класс для работы с обработчиками сообщений

class Server : public QTcpServer{   //  создание класса сервера, унаследованного от QTcpServer'a
    Q_OBJECT
public:
    Server(bool &server_started);
    QTcpSocket *socket;
    int generatedServerPort = QRandomGenerator::global()->bounded(1024, 65535);    //  автоматически генерируем порт

private:
    QMap<QTcpSocket*, QString> mapSockets;  //  структура из сокета-ключа и возможной обработки-значения
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    QMap<QString,QString> mapRequest;   //  определяем глоссарий запросов к сторонам
    QMap<QString, QVariant> possibleProcessing;   //  определяем возможные обработки с приставкой и её человеческим описанием

    qint64 nextBlockSize;   //  блок нового сообщения

    QFile *file;    //  определяем файл
    char *bytes = {0};     //  массив байт данных
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    int blockData = 1000000;  //  размер данных
    QString someone;    //  имя отправителя

    QString workspaceFolder = "";   //  путь до рабочей директории
    QString entryFolder = ""; //  путь до папки для файлов извне
    QString storageFolder = "";  //  путь до папки с приходящей обработанной информацией от клиентов
    QString expectationFolder = "";
    QString delimiter = "<font color = black><\\font><br>=======================";  //  создаем разделитель для сообщений
    QString JSONSettingFilePath = "";

    QFileSystemWatcher *fileSystemWatcher;

    ProcessingManager *processingManager;
    ReadyReadManager *readyReadManager;

    void SendPossibleProcessing(QTcpSocket* socket, QMap<QString, QVariant> possibleProcessingData);  //  функция передачи возможных обработок

    void SendToAllClients(QString typeOfMsg, QString str);      //  функция для передачи данных всем клиентам
    void SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str);       //  функция для передачи данных одному клиенту

    void SendFileToClient(QString filePath);    //  функция отправки файл (начало)
    void SendPartOfFile();      //  функция отправки части файла (продолжение)

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений и файлов
    void slotDisconnect();  //  обработчик отключившихся клиентов
    void slotNewWorkspaceFolder(QString newFolderPath);  //  обработчик новой директории
    void slotEntryFolderChanged(const QString & fileName);  //  обработчик изменений в директории
    void slotSocketDisplayed(QTcpSocket* displayedSocket);  //  обработчик для размещенного сокета
    void slotDisconnectSocket(int socketDiscriptorToDelete);    //  обработчик для принудительного удаления сокета
    void slotSetJSONSettingFilePath(QString JSONSettingsFilePath);   //  обработчик установки пути к JSON файлу настроек
    void slotUpdatePossibleProcessing(QVariant newPossibleProcessingData);

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
    void signalAddSocketToListWidget(QTcpSocket* socketToAdd);     //  слот для добавления сокета в clientsListWidget
    void signalDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);  //  слот для удаления сокета из clientsListWidget при его отключении
//    void signalChatServer(QString);     //  слот для обработки чата сервераы
};

#endif // SERVER_H
