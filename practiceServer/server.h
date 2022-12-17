#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

///  ========================    классы для работы сервера
#include <QTcpServer>
#include <QTcpSocket>
///  ========================
///
///  ========================   классы для работы с файлами
#include <QFile>    //  для работы с файлами
#include <QDir>     //  для работы с директорией
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
///  ========================
///
///  ========================   дополнения
#include <QVector>        //    класс вектора для хранения созданных сокетов
#include <QTime>    //  время
#include <QMap>     //  определение глоссария для приходящих данных сокета



class Server : public QTcpServer{   //  создание класса сервера, унаследованного от QTcpServer'a
    Q_OBJECT
public:
    Server(bool &server_started);
    QTcpSocket *socket;

private:
//    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QMap<QTcpSocket*, QString> mapSockets;  //  структура из сокета-ключа и возможной обработки-значения
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    QMap<QString,QString> mapRequest;   //  определяем глоссарий запросов к сторонам
    QMap<QString,QString> possibleTreatments;   //  определяем возможные обработки с приставкой и её человеческим описанием

    void SendPossibleTreatments(QTcpSocket* socket);  //  функция передачи возможных обработок

    void SendToAllClients(QString typeOfMsg, QString str);      //  функция для передачи данных всем клиентам
    void SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str);       //  функция для передачи данных одному клиенту

    void SendFileToClient(QString filePath);    //  функция отправки файл (начало)
    void SendPartOfFile();      //  функция отправки части файла (продолжение)

    qint64 nextBlockSize;   //  блок нового сообщения

    QFile *file;    //  определяем файл
    char *bytes = {0};     //  массив байт данных
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    int blockData = 1000000;  //  размер данных
    QString someone;    //  имя отправителя

    QString newDirPath = "C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\"; //  путь новой директории, по умолчанию рабочий стол
    QString folderForRawInformation = "C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\folderForRawInformation";  //  путь к папке с приходящей обработанной информацией от клиентов
    QString delimiter = "<font color = black><\\font><br>=======================";  //  создаем разделитель для сообщений
    QString JSONSettingFilePath = "";

    QFileSystemWatcher *fileSystemWatcher;

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений и файлов
    void slotDisconnect();  //  обработчик отключившихся клиентов
    void slotNewSaveDir(QString newDirPath);  //  обработчик новой директории
    void slotFolderForRawInformationChanged(const QString & fileName);  //  обработчик изменений в директории
    void slotSocketDisplayed(QTcpSocket* displayedSocket);  //  обработчик для размещенного сокета
    void slotDisconnectSocket(int socketDiscriptorToDelete);    //  обработчик для принудительного удаления сокета
    void slotSetJSONSettingFilePath(QString JSONSettingsFilePath);   //  обработчик установки пути к JSON файлу настроек

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
    void signalAddSocketToListWidget(QTcpSocket* socketToAdd);     //  слот для добавления сокета в clientsListWidget
    void signalDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);  //  слот для удаления сокета из clientsListWidget при его отключении
    void signalAddTreatmentToPossibleTreatmentsComboBox(QString treatmentToAdd); 
//    void signalChatServer(QString);     //  слот для обработки чата сервераы
};

#endif // SERVER_H
