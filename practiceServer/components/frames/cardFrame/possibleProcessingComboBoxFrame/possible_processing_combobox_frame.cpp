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

QMap<QString, QVariant> PossibleProcessingComboBoxFrame::getValue()
{
    QMap<QString, QVariant> comboBoxData;   //  переменная для данных выпадающего списка
    for(int i = 0; i < possibleProcessingComboBox->count(); i++){
        comboBoxData[possibleProcessingComboBox->itemText(i)] = possibleProcessingComboBox->itemData(i);
    }

    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлены новые обработки! <br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, comboBoxData);
    return valueMap;
}

void PossibleProcessingComboBoxFrame::setValue(QVariant value)
{
    possibleProcessingComboBox->clear();
    QMap<QString, QVariant> comboBoxValue = value.toMap();

    for(auto valueMap = comboBoxValue.begin(); valueMap != comboBoxValue.end(); valueMap++){
        possibleProcessingComboBox->addItem(valueMap.key(), valueMap.value());
    }
}

