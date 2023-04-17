#include "max_connection_spinbox_frame.h"

#include <QHBoxLayout>

MaxConnectionSpinBoxFrame::MaxConnectionSpinBoxFrame(MainWindow *parentUi)
{
    this->setObjectName("Max Connection Frame");

    maxConnectionSpinBox = new QSpinBox();
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

QVariant MaxConnectionSpinBoxFrame::getValue()
{
    return QString::number(maxConnectionSpinBox->value());
}

void MaxConnectionSpinBoxFrame::setValue(QVariant value)
{
    maxConnectionSpinBox->setValue(value.toInt());
}
