#include "select_processor_frame.h"

SelectProcessorFrame::SelectProcessorFrame()
{
    dataLabel = new QLabel("Не выбран обработчик");
    chooseProcessorPushButton = new QPushButton("Выбрать обработчик");
}

void SelectProcessorFrame::createInterface()
{
    QHBoxLayout *mainHLayout = new QHBoxLayout();
    this->setLayout(mainHLayout);

    mainHLayout->addWidget(chooseProcessorPushButton);
    mainHLayout->addWidget(dataLabel);
}

QMap<QString, QVariant> SelectProcessorFrame::getValue()
{

}

void SelectProcessorFrame::setValue(QVariant value)
{

}

void SelectProcessorFrame::switchEnabledInteface()
{

}
