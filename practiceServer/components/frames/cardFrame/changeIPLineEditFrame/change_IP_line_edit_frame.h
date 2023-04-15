#ifndef LINEEDITFRAME_H
#define LINEEDITFRAME_H

#include "../I_cardframe.h"

#include <QLineEdit>
#include <QLabel>

class ChangeIPLineEditFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChangeIPLineEditFrame();

    void createInterface() override;
    QVariant getValue() override;

private:
    QLineEdit *changeIPLineEdit;
    QLabel *dataLabel;
};

#endif // LINEEDITFRAME_H
