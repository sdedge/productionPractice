#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectToServerPushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 2323);
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);

    ui->lineEdit->clear();  //  чистим lineEdit после отправки сообщения
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
//        QString str;
//        in >> str;
//        ui->textBrowser->append(str);

        while(true){
            if(nextBlockSize == 0){
                qDebug() << "nextBlockSize == 0";
                if(socket->bytesAvailable() < 2){
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize){
                qDebug() << "Data not full";
                break;
            }
            QString str;
            in >> str;
            nextBlockSize = 0;
            ui->textBrowser->append(str);

        }
    } else {
        ui->textBrowser->append("Error connection");
    }
}


void MainWindow::on_sendPushButton_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

