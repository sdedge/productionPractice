#include "chat_frame.h"

ChatFrame::ChatFrame(MainWindow *ui)
{
    this->setObjectName("ChatFrame");
    parentUi = ui;

    callMeLineEdit = new QLineEdit();
    callMeLineEdit->setObjectName("callMeLineEdit");
    callMeLineEdit->setPlaceholderText("Меня зовут..");
    callMeLineEdit->setEnabled(false);

    chatTextBrowser = new QTextBrowser();
    chatTextBrowser->setObjectName("chatTextBrowser");
    chatTextBrowser->setEnabled(false);

    messageLineEdit = new QLineEdit();
    messageLineEdit->setObjectName("messageLineEdit");
    messageLineEdit->setPlaceholderText("Введите сообщение: ");
    messageLineEdit->setEnabled(false);
    //  поскольку нужен один и тот же функционал, разные сигналы подключаются к одному слоту
    connect(messageLineEdit, &QLineEdit::returnPressed, this, &ChatFrame::slotSendMessagePushButton);

    sendMessagePushButton = new QPushButton("Отправить сообщение");
    sendMessagePushButton->setObjectName("sendMessagePushButton");
    sendMessagePushButton->setEnabled(false);
    connect(sendMessagePushButton, &QPushButton::clicked, this, &ChatFrame::slotSendMessagePushButton);
}

void ChatFrame::createInterface()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    this->setLayout(mainVLayout);

    QFrame *messageFrame = new QFrame();
    QHBoxLayout *messageHLayout = new QHBoxLayout();
    messageFrame->setLayout(messageHLayout);
    messageHLayout->addWidget(messageLineEdit);
    messageHLayout->addWidget(sendMessagePushButton);

    mainVLayout->addWidget(callMeLineEdit);
    mainVLayout->addWidget(chatTextBrowser);
    mainVLayout->addWidget(messageFrame);
}

QMap<QString, QVariant> ChatFrame::getValue()
{

}

void ChatFrame::setValue(QVariant value)
{
    chatTextBrowser->append(QTime::currentTime().toString()+" | "+value.toString());
}

void ChatFrame::switchEnabledInteface()
{
    callMeLineEdit->setEnabled(!callMeLineEdit->isEnabled());
    chatTextBrowser->setEnabled(!chatTextBrowser->isEnabled());
    messageLineEdit->setEnabled(!messageLineEdit->isEnabled());
    sendMessagePushButton->setEnabled(!sendMessagePushButton->isEnabled());
}

void ChatFrame::slotSendMessagePushButton()
{
    QString text = "MESS:"+messageLineEdit->text();
    QString senderName = callMeLineEdit->text();
    emit parentUi->signalSendTextToServer(text, senderName);
    messageLineEdit->clear();
}
