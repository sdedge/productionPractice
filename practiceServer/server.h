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

    void SendToClient(QString typeOfMsg, QString str);    //  функция для передачи данных клиенту

    qint64 nextBlockSize;   //  блок нового сообщения

    QFile *file;    //  определяем файл
    char *bytes = {0};     //  массив байт данных
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    int blockData = 10000;  //  размер данных

    QString newDirPath; //  путь новой директории
    QString delimiter = "<font color = black><\\font><br>=======================";  //  создаем разделитель для сообщений

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений и файлов
    void slotNewSaveDir(QString newDirPath);  //  обработчик новой директории

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
};

#endif // SERVER_H
