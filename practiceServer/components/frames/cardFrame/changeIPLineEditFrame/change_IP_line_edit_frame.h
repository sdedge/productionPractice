#ifndef LINEEDITFRAME_H
#define LINEEDITFRAME_H

#include "../I_cardframe.h"
#include "mainwindow.h"

#include <QLineEdit>
#include <QLabel>

class ChangeIPLineEditFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChangeIPLineEditFrame(MainWindow *parentUi);

    void createInterface() override;
    QVariant getValue() override;
    void setValue(QVariant value) override;

private:
    QLineEdit *changeIPLineEdit;
    QLabel *dataLabel;
    MainWindow *parentUi;
};

#endif // LINEEDITFRAME_H
