#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    server = new Server;

//    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);

    /// не будет ли дублирование сервера, если я тут делаю
    /// server = new Server
    /// а в main.cpp создаю сам сервер через Server s
    /// ?
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)
{
//    ui->label->setText(status);
}



