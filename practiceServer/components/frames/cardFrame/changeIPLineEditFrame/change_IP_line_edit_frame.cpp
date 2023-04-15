#include "change_IP_line_edit_frame.h"

#include <QHBoxLayout>

ChangeIPLineEditFrame::ChangeIPLineEditFrame()
{
    this->setObjectName("Change IP lineEdit Frame");
    changeIPLineEdit = new QLineEdit;
    dataLabel = new QLabel;

    changeIPLineEdit->setText("127.0.0.1");
    dataLabel->setText("Сменить IP:");
}

void ChangeIPLineEditFrame::createInterface()
{
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(changeIPLineEdit);
}

QVariant ChangeIPLineEditFrame::getValue()
{
    return changeIPLineEdit->text();
}
