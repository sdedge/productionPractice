#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool server_started = false;    //  изначально наш сервер выключен

    server = new Server(server_started);    //  создаем сервер, передавая его состояние, которое изменится в конструкторе

    if(server_started)  //  проверка состояния "true"
        {
            ui->infoAboutServerTextEdit->append("Сервер запущен");  //  уведомление
        }
        else
        {
            ui->infoAboutServerTextEdit->append("Сервер не запущен");   //  уведомление
        }

    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);
    connect(this, &MainWindow::signalNewSaveDir, server, &Server::slotNewSaveDir);

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)   //  обработчик состояния
{
    qDebug() << status; //  вывод в консоль статуса
    ui->infoAboutServerTextEdit->append("<font color = black><\\font>"+status);    //  и также в textEdit
}


void MainWindow::on_chooseSaveDirPushButton_clicked()   //  по нажатию на "Choose save directory"
{
    QString dirPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");  //  выбираем папку
    if(!dirPath.isEmpty()){
        //  указываем в статусе сервера, что была изменена директория. HTML тут работает, пользуемся
        ui->infoAboutServerTextEdit->append("<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлена новая директория сохранения: "+dirPath+"<br/><font color = red>!!!<\\font>");
        ui->dirPathLabel->setText(dirPath); //  для наглядности выводим путь в dirPathLabel
        emit signalNewSaveDir(dirPath);
    }
}

