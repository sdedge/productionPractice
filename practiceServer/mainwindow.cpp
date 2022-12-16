#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractScrollArea>
#include <QMessageBox>

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
    connect(this, &MainWindow::signalDisconnectSocket, server, &Server::slotDisconnectSocket);  //  связка для принудительного удаления сокета
    //    connect(server, &Server::signalAddTreatmentToPossibleTreatmentsComboBox, this, &MainWindow::slotAddTreatmentToPossibleTreatmentsComboBox);  //  связка для добавления нового вида обработки в PossibleTreatmentsComboBox

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    ui->possibleTreatmetsComboBox->addItem("Дублирование информации");
    ui->possibleTreatmetsComboBox->addItem("Утроение информации");

    // устанавливаем специальную политику отображения меню
    ui->clientsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // ждем сигнала для отображения меню
    connect(ui->clientsListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)));
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
    ui->clientsListWidget->addItem("User desc:"+QString::number(socketToAdd->socketDescriptor())+" | IP: "+socketToAdd->localAddress().toString());
//    qDebug() << QString::number(socketToAdd->socketDescriptor()) << socketToAdd->localAddress().toString();
    MainWindow::signalSocketDisplayed(socketToAdd);
}

void MainWindow::slotDeleteSocketFromListWidget(QTcpSocket *socketToDelete)
{
    qDebug() << "User desc :"+QString::number(socketToDelete->socketDescriptor())+" | IP: "+socketToDelete->localAddress().toString();
    for(int i = 0; i < ui->clientsListWidget->count(); i++){    //  перебираем все элементы clietnsListWidget
        //  ↓↓↓ Если текст элемента совпадает с удаляемым сокетом, ....
        if(ui->clientsListWidget->item(i)->text() == "User desc:"+QString::number(socketToDelete->socketDescriptor())+" | IP: "+socketToDelete->localAddress().toString()){
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

void MainWindow::slotShowContextMenu(const QPoint &pos)
{
    QPoint globalPos;
    if (sender()->inherits("QAbstractScrollArea"))
      globalPos = dynamic_cast<QAbstractScrollArea*>(sender())->viewport()->mapToGlobal(pos);
    else
      globalPos = dynamic_cast<QWidget*>(sender())->mapToGlobal(pos);

    // Создаем меню
    QMenu menu;
    // Создаем пункт меню
    QAction* action1 = new QAction(QString::fromUtf8("Отключить"), this);
    // добавляем пункт в меню
    menu.addAction(action1);
}

void MainWindow::slotDisconnectClient()
{
    //  определяем выбранную строчку
    int row = ui->clientsListWidget->selectionModel()->currentIndex().row();
    // проверяем, действительно ли выбрали
    if(row >= 0){
        //  если да, то выводим предупреждение
        if (QMessageBox::warning(this,
                                 QString::fromUtf8("Отключение сокета"),
                                 QString::fromUtf8("Вы уверены, что хотите отключить клиента?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            //  при отказе выходим
            return;
        } else {
            //  иначе берем дескриптор сокета
            QString socketText = ui->clientsListWidget->currentItem()->text().remove(0, ui->clientsListWidget->currentItem()->text().indexOf(":")+2);
            socketText = socketText.remove(4, socketText.length());

            //  и отправляем запрос на сервер, чтобы по нему удалили сокет
            MainWindow::signalDisconnectSocket(socketText.toInt());
            qDebug() << "descriptor to delete: " << socketText;
        }
    }
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

void MainWindow::on_clientsListWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "contextMenu clicked";

    //  Создаем объект контекстного меню
    QMenu* menu = new QMenu(this);
    //  Создаём действия для контекстного меню
    QAction* disconnectClient = new QAction(QString::fromUtf8("Отключить"), this);
    //  Подключаем СЛОТы обработчики для действий контекстного меню
    connect(disconnectClient, SIGNAL(triggered()), this, SLOT(slotDisconnectClient()));     // Обработчик вызова диалога редактирования
    //  Устанавливаем действия в меню
    menu->addAction(disconnectClient);
    //  Вызываем контекстное меню
    menu->popup(ui->clientsListWidget->viewport()->mapToGlobal(pos));
}

