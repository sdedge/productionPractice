#ifndef MAXCONNECTIONSPINBOXFRAME_H
#define MAXCONNECTIONSPINBOXFRAME_H

#include "../I_cardframe.h"

#include <QSpinBox>
#include <QLabel>

class MaxConnectionSpinBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    MaxConnectionSpinBoxFrame();

    void createInterface() override;
    QVariant getValue() override;

    QSpinBox *maxConnectionSpinBox;
    QLabel *dataLabel;
};

#endif // MAXCONNECTIONSPINBOXFRAME_H
