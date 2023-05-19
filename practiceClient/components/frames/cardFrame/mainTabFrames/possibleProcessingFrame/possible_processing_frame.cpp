#include "possible_processing_frame.h"

PossibleProcessingFrame::PossibleProcessingFrame(MainWindow *ui)
{
    this->setObjectName("PossibleProcessingFrame");
    parentUi = ui;

    chooseProcessingComboBox = new QComboBox();
    chooseProcessingComboBox->setObjectName("chooseProcessingComboBox");
    chooseProcessingComboBox->setEnabled(false);

    chooseProcessingPushButton = new QPushButton("Выбрать");
    chooseProcessingPushButton->setObjectName("chooseProcessingPushButton");
    chooseProcessingPushButton->setEnabled(false);
}

void PossibleProcessingFrame::createInterface()
{
    QHBoxLayout *mainHLayout = new QHBoxLayout();
    this->setLayout(mainHLayout);

    mainHLayout->addWidget(chooseProcessingComboBox);
    mainHLayout->addWidget(chooseProcessingPushButton);
}

QMap<QString, QVariant> PossibleProcessingFrame::getValue()
{

}

void PossibleProcessingFrame::setValue(QVariant value)
{
    QMap<QString,QVariant> possibleProcessingData = value.toMap();

    chooseProcessingComboBox->clear();
    for(auto it = possibleProcessingData.begin(); it != possibleProcessingData.end(); it++){
        chooseProcessingComboBox->addItem(it.key(), it.value());
    }
}

void PossibleProcessingFrame::switchEnabledInteface()
{
    chooseProcessingComboBox->setEnabled(!chooseProcessingComboBox->isEnabled());
    chooseProcessingPushButton->setEnabled(!chooseProcessingPushButton->isEnabled());
}
