#include "change_IP_line_edit_frame.h"

#include <QHBoxLayout>
#include <QMap>

ChangeIPLineEditFrame::ChangeIPLineEditFrame(MainWindow *parentUi)
{
    //  конструктор карточки
    this->setObjectName("Change IP lineEdit Frame");
    changeIPLineEdit = new QLineEdit;
    dataLabel = new QLabel;

    this->parentUi = parentUi;

    changeIPLineEdit->setText("127.0.0.1");
    dataLabel->setText("Сменить IP:");
}

void ChangeIPLineEditFrame::createInterface()
{
    //  создание интерфейса
    QHBoxLayout *frameLayout = new QHBoxLayout();   //  слой выравнивания
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(changeIPLineEdit);
}

QMap<QString, QVariant> ChangeIPLineEditFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлен новый IP: "+changeIPLineEdit->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, changeIPLineEdit->text());
    return valueMap;
}

void ChangeIPLineEditFrame::setValue(QVariant value)
{
    changeIPLineEdit->setText(value.toString());
}
