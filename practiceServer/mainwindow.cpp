#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool server_started = false;

    server = new Server(server_started);

//    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);

    /// не будет ли дублирование сервера, если я тут делаю
    /// server = new Server
    /// а в main.cpp создаю сам сервер через Server s
    /// ?

    if(server->listen(QHostAddress::Any, 2323)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
//        ui->label->setText("Server start");
        emit signalStatusServer("Server start");
        qDebug() << "start";
    } else {
//        ui->label->setText("Something happened :(");
//        emit signalStatusServer();
    }

    if(server_started)
        {
            ui->textEdit->append("Сервер запущен");
        }
        else
        {
            ui->textEdit->append("Сервер не запущен");
        }

    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)
{
    qDebug() << status;
    ui->textEdit->append(status);
}

