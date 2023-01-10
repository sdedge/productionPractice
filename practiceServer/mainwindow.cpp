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
            ui->infoAboutServerTextEdit->append("Сервер запущен на "+server->serverAddress().toString()+" IP и "+QString::number(server->generatedServerPort)+" порту");  //  уведомление
        }
        else
        {
            ui->infoAboutServerTextEdit->append("Сервер не запущен");   //  уведомление
            return;
        }

    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);  //  связка для отображения статуса сервера, вывод в консоль
    connect(server, &Server::signalAddSocketToListWidget, this, &MainWindow::slotAddSocketToListWidget);    //  связка для отображения добавления клиентов в clientsListWidget
    connect(server, &Server::signalDeleteSocketFromListWidget, this, &MainWindow::slotDeleteSocketFromListWidget);  //  связка для удаления сокета из clientsListWidget
    connect(this, &MainWindow::signalNewSaveDir, server, &Server::slotNewSaveDir);  //  связка для отображения новой директории
    connect(this, &MainWindow::signalSocketDisplayed, server, &Server::slotSocketDisplayed);    //  связка для отправки подключившемуся сокету список доступных обработок
    connect(this, &MainWindow::signalDisconnectSocket, server, &Server::slotDisconnectSocket);  //  связка для принудительного удаления сокета
    connect(this, &MainWindow::signalSetJSONSettingFilePath, server, &Server::slotSetJSONSettingFilePath);  //  связка для установки пути к JSON файлу настроек
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
    qDebug() << "MainWindow::slotStatusServer:      " << status; //  вывод в консоль статуса
    ui->infoAboutServerTextEdit->append(delimiter+QTime::currentTime().toString()+" | <font color = black><\\font>"+status);    //  и также в textEdit
}

void MainWindow::slotAddSocketToListWidget(QTcpSocket *socketToAdd)
{
    //  TODO:   сделать обращение к clientsListWidget и добавление данных с сокета
    ui->clientsListWidget->addItem("User desc:"+QString::number(socketToAdd->socketDescriptor())+" | IP: "+socketToAdd->localAddress().toString());
//    qDebug() << QString::number(socketToAdd->socketDescriptor()) << socketToAdd->localAddress().toString();
    MainWindow::signalSocketDisplayed(socketToAdd);
}

void MainWindow::slotDeleteSocketFromListWidget(QMap<QTcpSocket *, QString> mapSockets)
{
//    qDebug() << "User desc :"+QString::number(mapSockets->socketDescriptor())+" | IP: "+mapSockets->localAddress().toString();
//    for(int i = 0; i < ui->clientsListWidget->count(); i++){    //  перебираем все элементы clietnsListWidget
//        //  ↓↓↓ Если текст элемента совпадает с удаляемым сокетом, ....
//        if(ui->clientsListWidget->item(i)->text() == "User desc:"+QString::number(mapSockets->socketDescriptor())+" | IP: "+mapSockets->localAddress().toString()){
//            QListWidgetItem* itemSocketToDelete = ui->clientsListWidget->takeItem(i);   //  ...., то удаляем из clientsListWidget сокет
//            delete itemSocketToDelete;  //  но он останется в памяти, поэтому его надо удалить вручную по совету документации
//            break;
//        }
//    }
    ui->clientsListWidget->clear();
    for(auto itemSocket = mapSockets.begin(); itemSocket != mapSockets.end(); itemSocket++){
        ui->clientsListWidget->addItem("User desc:"+QString::number(itemSocket.key()->socketDescriptor())+" | IP: "+itemSocket.key()->localAddress().toString());
    }

}

void MainWindow::slotAddTreatmentToPossibleTreatmentsComboBox(QString treatmentToAdd)
{
    ui->possibleTreatmetsComboBox->addItem(treatmentToAdd);
    qDebug() << "MainWindow::slotAddTreatmentToPossibleTreatmentsComboBox:      " << treatmentToAdd;
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
            QString socketText = ui->clientsListWidget->currentItem()->text().remove(0, ui->clientsListWidget->currentItem()->text().indexOf(":")+1);
            socketText = socketText.remove(4, socketText.length());

            //  и отправляем запрос на сервер, чтобы по нему удалили сокет
            MainWindow::signalDisconnectSocket(socketText.toInt());
            qDebug() << "MainWindow::slotDisconnectClient:      descriptor to delete: " << socketText;
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
        qDebug() << "MainWindow::on_chooseSaveDirPushButton_clicked:        on_chooseSaveDirPushButton_clicked || " << dirPath << "set like text to dirPathLabel";
    }
}

void MainWindow::on_clientsListWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "MainWindow::on_clientsListWidget_customContextMenuRequested:       contextMenu clicked";

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


void MainWindow::on_openJSONSettingsFilePushButton_clicked()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла", "C:\\");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    QFile fileJSONSetting;
    fileJSONSetting.setFileName(filePath);
    fileJSONSetting.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = fileJSONSetting.readAll();
    fileJSONSetting.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8(), &error);
    qDebug() << "MainWindow::on_openJSONSettingsFilePushButton_clicked:     Error: " << error.errorString() << error.offset << error.error;

    QJsonObject root = document.object();
    QString keyObject = "";
    QString valueObject = "";
    for (int i = 0; i < root.size(); i++) {
        keyObject = root.keys().at(i);
        if(keyObject.contains("Label")){
            QLabel *label = ui->mainInfoTab->findChild<QLabel *>(keyObject);
            valueObject = root.value(root.keys().at(0)).toString();
            label->setText(valueObject);
            emit signalNewSaveDir(valueObject);
            ui->infoAboutServerTextEdit->append("<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлена новая директория сохранения: "+valueObject+"<br/><font color = red>!!!<\\font>");
            qDebug() << "MainWindow::on_openJSONSettingsFilePushButton_clicked:     " << valueObject << "set like text to dirPathLabel";
        }
    }
}


void MainWindow::on_saveSettingsPushButton_clicked()
{

    m_currentJsonObject.insert("dirPathLabel", ui->dirPathLabel->text());

    // Выводим текст всего Json объекта в консоль для проверки
    qDebug() << "on_saveSettingsPushButton_clicked:     " << QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented);

    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                            tr("Save Json File"),
                                                            QString(),
                                                            tr("JSON (*.json)"));
    QFileInfo fileInfo(saveFileName);   // С помощью QFileInfo
    QDir::setCurrent(fileInfo.path());  // установим текущую рабочую директорию, где будет файл, иначе может не заработать
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }

    // Записываем текущий объект Json в файл
    jsonFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();   // Закрываем файл
}

