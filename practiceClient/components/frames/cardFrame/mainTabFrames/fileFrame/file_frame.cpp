#include "file_frame.h"

FileFrame::FileFrame(MainWindow *ui)
{
    this->setObjectName("FileFrame");
    parentUi = ui;

    filePathLabel = new QLabel();
    filePathLabel->setObjectName("filePathLabel");

    filePathLineEdit = new QLineEdit();
    filePathLineEdit->setObjectName("filePathLineEdit");
    filePathLineEdit->setPlaceholderText("Путь до файла");
    filePathLineEdit->setEnabled(false);

    openFilePushButton = new QPushButton("Открыть проводник");
    openFilePushButton->setObjectName("openFilePushButton");
    openFilePushButton->setEnabled(false);

    sendFilePushButton = new QPushButton("Отправить файл");
    sendFilePushButton->setObjectName("sendFilePushButton");
    sendFilePushButton->setEnabled(false);
}

void FileFrame::createInterface()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    this->setLayout(mainVLayout);

    QFrame *filePathFrame = new QFrame();
    QHBoxLayout *filePathHLayout = new QHBoxLayout();
    filePathFrame->setLayout(filePathHLayout);
    filePathHLayout->addWidget(filePathLineEdit);
    filePathHLayout->addWidget(openFilePushButton);

    mainVLayout->addWidget(filePathLabel);
    mainVLayout->addWidget(filePathFrame);
    mainVLayout->addWidget(sendFilePushButton);
}

QMap<QString, QVariant> FileFrame::getValue()
{

}

void FileFrame::setValue(QVariant value)
{
    filePathLabel->setText(value.toString());
}

void FileFrame::switchEnabledInteface()
{
    filePathLabel->clear();
    filePathLineEdit->setEnabled(!filePathLineEdit->isEnabled());
    openFilePushButton->setEnabled(!openFilePushButton->isEnabled());
    sendFilePushButton->setEnabled(!sendFilePushButton->isEnabled());
}
