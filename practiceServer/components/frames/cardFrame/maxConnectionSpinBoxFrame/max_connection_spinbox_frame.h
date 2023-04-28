#ifndef MAXCONNECTIONSPINBOXFRAME_H
#define MAXCONNECTIONSPINBOXFRAME_H

#include "../I_cardframe.h"
#include "mainwindow.h"

#include <QSpinBox>
#include <QLabel>

class MaxConnectionSpinBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    MaxConnectionSpinBoxFrame(MainWindow *parentUi);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;

private:
    QString consoleMessage;
    QSpinBox *maxConnectionSpinBox;
    QLabel *dataLabel;
    MainWindow *parentUi;
};

#endif // MAXCONNECTIONSPINBOXFRAME_H
