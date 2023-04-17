#include "possible_processing_combobox_frame.h"

#include <QHBoxLayout>

PossibleProcessingComboBoxFrame::PossibleProcessingComboBoxFrame(MainWindow *parentUi)
{
    this->setObjectName("Possible processing Frame");

    possibleProcessingComboBox = new QComboBox();

    //  TODO:   сделать подгрузку из JSON
    possibleProcessingComboBox->addItem("Дублирование информации", "double");
    possibleProcessingComboBox->addItem("Утроение информации", "triple");

    dataLabel = new QLabel();

    this->parentUi = parentUi;

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

void PossibleProcessingComboBoxFrame::setValue(QVariant value)
{
    QMap<QString, QVariant> comboBoxValue = value.toMap();

    for(auto valueMap = comboBoxValue.begin(); valueMap != comboBoxValue.end(); valueMap++){
        possibleProcessingComboBox->addItem(valueMap.key(), valueMap.value());
    }
}

