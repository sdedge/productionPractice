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
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;

private:
    QString consoleMessage;
    QComboBox *possibleProcessingComboBox;
    QLabel *dataLabel;
    MainWindow *parentUi;
};

#endif // COMBOBOXFRAME_H
