#include "change_port_line_edit_frame.h"

#include <QHBoxLayout>
#include <QMap>

ChangePortLineEditFrame::ChangePortLineEditFrame(MainWindow *parentUi)
{
    //  конструктор карточки
    this->setObjectName("Change IP lineEdit Frame");
    changePortLineEdit = new QLineEdit;
    changePortLineEdit->setPlaceholderText("1024-65535");
    changePortLineEdit->setMaximumWidth(95);
    changePortLineEdit->setEnabled(false);

    dataLabel = new QLabel;

    this->parentUi = parentUi;

    changePortLineEdit->setText("00000");
    dataLabel->setText("Сменить порт:");
}

void ChangePortLineEditFrame::createInterface()
{
    //  создание интерфейса
    QHBoxLayout *frameLayout = new QHBoxLayout();   //  слой выравнивания
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(changePortLineEdit);
}

void ChangePortLineEditFrame::enableInteface(){
    changePortLineEdit->setEnabled(true);
}

QMap<QString, QVariant> ChangePortLineEditFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлен новый порт: "+changePortLineEdit->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, changePortLineEdit->text());
    return valueMap;
}

void ChangePortLineEditFrame::setValue(QVariant value)
{
    changePortLineEdit->setText(value.toString());
}
