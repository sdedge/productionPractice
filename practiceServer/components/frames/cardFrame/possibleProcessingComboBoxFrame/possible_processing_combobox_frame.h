#ifndef COMBOBOXFRAME_H
#define COMBOBOXFRAME_H

#include "../I_cardframe.h"
#include "mainwindow.h"

#include <QComboBox>
#include <QLabel>

class PossibleProcessingComboBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    PossibleProcessingComboBoxFrame(MainWindow *parentUi);

    void createInterface() override;
    QVariant getValue() override;
    void setValue(QVariant value) override;

private:
    QComboBox *possibleProcessingComboBox;
    QLabel *dataLabel;
    MainWindow *parentUi;
};

#endif // COMBOBOXFRAME_H
