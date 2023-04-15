#ifndef COMBOBOXFRAME_H
#define COMBOBOXFRAME_H

#include "../I_cardframe.h"

#include <QComboBox>
#include <QLabel>

class PossibleProcessingComboBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    PossibleProcessingComboBoxFrame();

    void createInterface() override;
    QVariant getValue() override;

private:
    QComboBox *possibleProcessingComboBox;
    QLabel *dataLabel;
};

#endif // COMBOBOXFRAME_H
