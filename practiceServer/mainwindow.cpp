#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);

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
    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)
{
//    ui->label->setText(status);
}

//void MainWindow::incomingConnection(qintptr socketDescriptor){
void MainWindow::slotNewConnection(){
//    socket = new QTcpSocket;    //  создание нового сокета
    QTcpSocket* socket = server->nextPendingConnection();
//    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, индентифицирующее поток ввода-вывода)

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении клиента сервер удалит сокет при первой же возможности

    Sockets.push_back(socket);  //  помещаем сокет в контейнер
//    ui->statusbar->showMessage("new client");
//    qDebug() << "new client on " << socketDescriptor;
    qDebug() << "new client on ";
}

void MainWindow::slotReadyRead(){
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2); //  установка версии, чтобы не было ошибок
    if(in.status() == QDataStream::Ok){ //  если у нас нет ошибок в состоянии работы in
//        ui->statusbar->showMessage("reading...");
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "nextBlockSize == 0";
                if(socket->bytesAvailable() < 2){   //  и не должен быть меньше 2-х байт
                    qDebug() << "Data < 2, break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Data not full";    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо
            QString str;    //  т.к. у нас пока чат, то сервер и клиент будут обмениваться только текстом
            in >> str;  //  записываем в нее строку из объекта in
            nextBlockSize = 0;
            SendToClient(str);  //  отправляем клиенту сообщение
            break;
        }
    } else {
//        ui->statusbar->showMessage("Something happened :(");
    }
}

void MainWindow::SendToClient(QString str){
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    for(int i = 0; i < Sockets.size(); i++){    //  пробегаемся по всем сокетам и
        Sockets[i]->write(Data);    //  отправляем по соответствующему сокету данные
    }
}

