#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();  //  при подключении создаем новый сокет

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);   //  при отключении сокет сам удалится

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    completer = new QCompleter(this);   //  создаем completer

    fModel = new QFileSystemModel();    //  создаем модель системы
    fModel->setRootPath(QDir::currentPath() + "\\");    //  устанавливаем разделитель

    completer->setModel(fModel);    //  completer'у устанавливаем модель

    completer->setCompletionMode(QCompleter::CompletionMode(0));    //  подсказки во всплывающем окне (2 - как выделенный текст, идущий далее)
    completer->setModelSorting(QCompleter::ModelSorting(2));    //  НЕ чувствительно к регистру (0 -  не сортировать, 1 - чувствительно к регистру)

    ui->filePathLineEdit->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectToServerPushButton_clicked()
{
    //  TODO: сделать программным способом задание значений для подключения

    socket->connectToHost("127.0.0.1", 2323);   //  подключение к серверу (локальный адрес + порт такой же, как у сервера)
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;   //  пока сообщение оправлено, мы не можем определить размер блока
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    socket->write(Data);

    ui->lineEdit->clear();  //  чистим lineEdit после отправки сообщения
}

void MainWindow::SendFileToServer(QString filePath)
{
    Data.clear();
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly)){
        ui->filePathLabel->setText("File open");
    }
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
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
            QString str;
            in >> str;  //  выводим в переменную сообщение
            nextBlockSize = 0;  //  обнуляем размер блока для последующего
            ui->textBrowser->append(str);   //  выводим полученное сообщение на экран
        }
    } else {
        ui->textBrowser->append("Error connection");
    }
}


void MainWindow::on_sendMsgPushButton_clicked() //  по нажатию на "Send msg"
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()    //  сообщение также отправится, если нажать клавишу Enter
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_openFilePushButton_clicked()    //  по нажатию на "Or open file"
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла", "C:\\");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    ui->filePathLabel->setText(filePath);   //  устанавливаем путь в label для наглядности
}


void MainWindow::on_sendFilePushButton_clicked()    //  по нажатию на "Send file"
{
    ui->filePathLabel->setText(ui->filePathLineEdit->text());   //  для наглядности устанавливаем в label путь к файлу
    ui->filePathLineEdit->clear();  //  очищаем поле ввода пути файла после выбора нажатии отправки
    SendFileToServer(QString(ui->filePathLabel->text()));    //  передаем функции этот же текст
}

