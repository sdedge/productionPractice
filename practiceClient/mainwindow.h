#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>   //  класс сервера

#include <QFileDialog>  //  класс работы с диалоговыми окнами (выбор файла)
#include <QCompleter>
#include <QFileSystemModel>
#include <QFile>


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

    void on_sendFilePushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket; //  сокет соединения
    QByteArray Data;    //  передаваемые файлы

    QCompleter *completer;  //  автодополнение вводимого пути файла
    QFileSystemModel *fModel;   //  файловая модель

    void SendToServer(QString str); //  метод отправки на сервер сообщения
    void SendFileToServer(QString filePath);    //  метод отправки на сервер файла по его пути

    quint16 nextBlockSize;  //  переменная для хранения размера блока текста

public slots:
    void slotReadyRead();   //  слот готовности к чтению сообщений
};
#endif // MAINWINDOW_H
