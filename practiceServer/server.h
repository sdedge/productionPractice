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
    QVector <QTcpSocket*> Sockets;   //  вектор, предназначенный под сокеты
    QByteArray Data;    //  то, что будет путешествовать между клиентом и сервером

    QMap<QString,QString> mapRequest;   //  определяем глоссарий запросов к сторонам

    void SendToAllClients(QString typeOfMsg, QString str);      //  функция для передачи данных всем клиентам
    void SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str);       //  функция для передачи данных одному клиенту

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

    QFileSystemWatcher *fileSystemWatcher;

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений и файлов
    void slotDisconnect();  //  обработчик отключившихся клиентов
    void slotNewSaveDir(QString newDirPath);  //  обработчик новой директории
    void slotFolderForRawInformationChanged(const QString & fileName);  //  обработчик изменений в директории

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
    void signalAddSocketToListWidget(QTcpSocket* socketToAdd);     //  слот для добавления сокета в clientsListWidget
    void signalDeleteSocketFromListWidget(QTcpSocket* socketToDelete);  //  слот для удаления сокета из clientsListWidget при его отключении
//    void signalChatServer(QString);     //  слот для обработки чата сервераы
};

#endif // SERVER_H
