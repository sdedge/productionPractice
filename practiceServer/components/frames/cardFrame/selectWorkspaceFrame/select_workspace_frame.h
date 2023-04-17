#ifndef SELECTWORKSPACEFRAME_H
#define SELECTWORKSPACEFRAME_H

#include "../I_cardframe.h"
#include "mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QMainWindow>

class SelectWorkspaceFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectWorkspaceFrame(MainWindow *parentUi);

    void createInterface() override;
    QVariant getValue() override;
    void setValue(QVariant value) override;

private:
    QPushButton *chooseWorkspaceDirPushButton;
    QLabel *dataLabel;
    MainWindow *parentUi;

};

#endif // CHOOSESAVEDIRFRAME_H
