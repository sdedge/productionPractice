#include "update_processing_dialog.h"

UpdateProcessingDialog::UpdateProcessingDialog(MainWindow *parentUi)
{
    this->setObjectName("Update processing Dialog");
    this->parentUi = parentUi;

    dataLabel = new QLabel();
    jsonDataTextEdit = new QTextEdit();

    openJsonFilePushButton = new QPushButton("Открыть");
    connect(openJsonFilePushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_openJsonFilePushButton_clicked);

    closeDialogPushButton = new QPushButton("Закрыть");
    connect(closeDialogPushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_closeDialogPushButton_clicked);

    applyDataPushButton = new QPushButton("Подтвердить");
    connect(applyDataPushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_applyDataPushButton_clicked);
}

void UpdateProcessingDialog::createInterface()
{
    QVBoxLayout *frameLayout = new QVBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(openJsonFilePushButton);
    frameLayout->addWidget(jsonDataTextEdit);
    frameLayout->addWidget(closeDialogPushButton);
    frameLayout->addWidget(applyDataPushButton);
}

QVariant UpdateProcessingDialog::getValue()
{
    return 0;
}

QString UpdateProcessingDialog::on_openJsonFilePushButton_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Json File"),
                                                    QString(),
                                                    tr("JSON (*.json)"));
    QFileInfo fileInfo(openFileName);
    QDir::setCurrent(fileInfo.path());

    QFile jsonFile(openFileName);
    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return QString("Невозможно открыть файл");
        qDebug() << "Невозможно открыть файл";
    }
    QByteArray saveData = jsonFile.readAll();

    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));

    m_currentJsonObject = jsonDocument.object();

    this->jsonDataTextEdit->clear();
    //  выгружаем данные с json файла в форматированном виде
    this->jsonDataTextEdit->setText(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));

    jsonFile.close();

    qDebug() << openFileName;
    qDebug() << QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented);
    return openFileName;
}

void UpdateProcessingDialog::on_closeDialogPushButton_clicked()
{
    this->close();
}

QString UpdateProcessingDialog::on_applyDataPushButton_clicked()
{
    return QString("");
}
