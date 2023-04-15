#include "possible_processing_combobox_frame.h"

#include <QHBoxLayout>

PossibleProcessingComboBoxFrame::PossibleProcessingComboBoxFrame()
{
    this->setObjectName("Possible processing Frame");

    possibleProcessingComboBox = new QComboBox();

    //  TODO:   сделать подгрузку из JSON
    possibleProcessingComboBox->addItem("Дублирование информации", "double");
    possibleProcessingComboBox->addItem("Утроение информации", "triple");

    dataLabel = new QLabel();

    dataLabel->setText("Возможные обработки:");
}

void PossibleProcessingComboBoxFrame::createInterface()
{
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(possibleProcessingComboBox);
}

QVariant PossibleProcessingComboBoxFrame::getValue()
{
    QMap<QString, QVariant> comboBoxData;
    for(int i = 0; i < possibleProcessingComboBox->count(); i++){
        comboBoxData[possibleProcessingComboBox->itemText(i)] = possibleProcessingComboBox->itemData(i);
    }
    return QVariant(comboBoxData);
}
