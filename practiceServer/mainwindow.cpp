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

    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);  //  связка для отображения статуса сервера, вывод в консоль
    connect(server, &Server::signalAddSocketToListWidget, this, &MainWindow::slotAddSocketToListWidget);    //  связка для отображения добавления клиентов в clientsListWidget
    connect(server, &Server::signalDeleteSocketFromListWidget, this, &MainWindow::slotDeleteSocketFromListWidget);  //  связка для удаления сокета из clientsListWidget
    connect(this, &MainWindow::signalNewSaveDir, server, &Server::slotNewSaveDir);  //  связка для отображения новой директории
    connect(this, &MainWindow::signalSocketDisplayed, server, &Server::slotSocketDisplayed);    //  связка для отправки подключившемуся сокету список доступных обработок
    //    connect(server, &Server::signalAddTreatmentToPossibleTreatmentsComboBox, this, &MainWindow::slotAddTreatmentToPossibleTreatmentsComboBox);  //  связка для добавления нового вида обработки в PossibleTreatmentsComboBox

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    ui->possibleTreatmetsComboBox->addItem("Дублирование информации");
    ui->possibleTreatmetsComboBox->addItem("Утроение информации");
//    ui->clientsListWidget->children()->setContextMenuPolicy(Qt::CustomContextMenu);     //  создаем к меню контекстное меню
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)   //  обработчик состояния
{
    qDebug() << status; //  вывод в консоль статуса
    ui->infoAboutServerTextEdit->append(delimiter+QTime::currentTime().toString()+" | <font color = black><\\font>"+status);    //  и также в textEdit
}

void MainWindow::slotAddSocketToListWidget(QTcpSocket *socketToAdd)
{
    //  TODO:   сделать обращение к clientsListWidget и добавление данных с сокета
    ui->clientsListWidget->addItem("User desc: "+QString::number(socketToAdd->socketDescriptor())+" | IP: "+socketToAdd->localAddress().toString());
//    qDebug() << QString::number(socketToAdd->socketDescriptor()) << socketToAdd->localAddress().toString();
    MainWindow::signalSocketDisplayed(socketToAdd);
}

void MainWindow::slotDeleteSocketFromListWidget(QTcpSocket *socketToDelete)
{
    qDebug() << "User desc :"+QString::number(socketToDelete->socketDescriptor())+" | IP: "+socketToDelete->localAddress().toString();
    for(int i = 0; i < ui->clientsListWidget->count(); i++){    //  перебираем все элементы clietnsListWidget
        //  ↓↓↓ Если текст элемента совпадает с удаляемым сокетом, ....
        if(ui->clientsListWidget->item(i)->text() == "User desc :"+QString::number(socketToDelete->socketDescriptor())+" | IP: "+socketToDelete->localAddress().toString()){
            QListWidgetItem* itemSocketToDelete = ui->clientsListWidget->takeItem(i);   //  ...., то удаляем из clientsListWidget сокет
            delete itemSocketToDelete;  //  но он останется в памяти, поэтому его надо удалить вручную по совету документации
            break;
        }
    }

}

void MainWindow::slotAddTreatmentToPossibleTreatmentsComboBox(QString treatmentToAdd)
{
    ui->possibleTreatmetsComboBox->addItem(treatmentToAdd);
    qDebug() << treatmentToAdd;
}

//void MainWindow::slotChatServer(QString message)    //  обработчик чата
//{
//    ui->ChatTextBrowser->append(QTime::currentTime().toString()+" | <font color = black><\\font>"+message);
//}


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
