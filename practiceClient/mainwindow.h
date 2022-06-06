#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>   //  класс сервера

#include <QFileDialog>  //  класс работы с диалоговыми окнами (выбор файла)


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
    void on_connectToServerPushButton_clicked();

    void on_sendPushButton_clicked();

    void on_lineEdit_returnPressed();

    void on_openFilePushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;

    void SendToServer(QString str);

    quint16 nextBlockSize;  //  переменная для хранения размера блока текста

public slots:
    void slotReadyRead();
};
#endif // MAINWINDOW_H
