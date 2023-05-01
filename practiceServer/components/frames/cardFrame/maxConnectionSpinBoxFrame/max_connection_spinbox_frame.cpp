#include "max_connection_spinbox_frame.h"

MaxConnectionSpinBoxFrame::MaxConnectionSpinBoxFrame(MainWindow *parentUi)
{
    this->setObjectName("Max Connection Frame");

    maxConnectionSpinBox = new QSpinBox();
    maxConnectionSpinBox->setMaximumWidth(40);

    dataLabel = new QLabel();

    this->parentUi = parentUi;

    dataLabel->setText("Макс. подключений:");
}

void MaxConnectionSpinBoxFrame::createInterface()
{
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(maxConnectionSpinBox);
}

QMap<QString, QVariant> MaxConnectionSpinBoxFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлен новый IP: "+QString::number(maxConnectionSpinBox->value())+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, maxConnectionSpinBox->value());
    return valueMap;
}

void MaxConnectionSpinBoxFrame::setValue(QVariant value)
{
    maxConnectionSpinBox->setValue(value.toInt());
}
