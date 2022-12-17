#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>   //  класс сервера

/// =========================   Для работы с файлами
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>  //  класс работы с диалоговыми окнами (выбор файла)
/// =========================
///
/// =========================   Для работы автодополнения ввода
#include <QCompleter>   //  дополняет строку при вводе пути к файлу
#include <QFileSystemModel> //  работа с файловой моделью
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
/// =========================
///
/// =========================   Разные дополнения
#include <QTime>    //  для отображения времени отправки
#include <QMap>     //  определение глоссария для приходящих данных сокета
/// =========================

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectToServerPushButton_clicked();    //  нажатие на кнопку "Connect to server"

    void on_sendMsgPushButton_clicked();   //  нажатие на кнопку "Send"

    void on_lineEdit_returnPressed();   //  событие нажатия клавиши Enter

    void on_openFilePushButton_clicked();   //  нажатие на кнопку "Or open file"

    void on_sendFilePushButton_clicked();   //  нажатие на кнопку "Send file"

    void on_chooseTreatmentPushButton_clicked();    //  нажатие на кнопку выбора типа обработки

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket; //  сокет соединения
    QByteArray Data;    //  передаваемые файлы

    QMap<QString,QString> mapRequest;   //  определяем глоссарий запросов к сторонам
    QMap<QString,QString> possibleTreatments;   //  определяем возможные обработки с приставкой и её человеческим описанием

    QCompleter *completer;  //  автодополнение вводимого пути файла
    QFileSystemModel *fModel;   //  файловая модель

    int fileSize;   //  размер файла
    QString fileName;   //  его название
    QFile *file;     //  сам файлик
    char *bytes = {0};     //  массив байт данных
    int blockData = 1000000;  //  размер данных
    QString rawInformationDirectory = "C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\folderForIncomingFromServerRawInformation";    //  папка для приходящих от сервера сырых данных на обработку
    QString delimiter = "<font color = black><\\font><br>=======================";  //  создаем разделитель для сообщений

    void SendTextToServer(QString str); //  метод отправки на сервер сообщения
    void SendFileToServer(QString filePath);    //  метод отправки на сервер файла по его пути
    void SendPartOfFile();  //  метод отправки части файла
    void SendToServer(QString typeOfMsg, QString str);  //  отправка служебных сообщений серверу

    void BlockingInterface();   //  функция блокировки интерфейса, пока снова не подключимся к хосту

    qint64 nextBlockSize;  //  переменная для хранения размера блока текста

    QFileSystemWatcher *fileSystemWatcher;

public slots:
    void slotReadyRead();   //  слот готовности к чтению сообщений
    void slotFolderForRawInformationChanged(const QString & fileName);  //  обработчик изменений в директории
};
#endif // MAINWINDOW_H
