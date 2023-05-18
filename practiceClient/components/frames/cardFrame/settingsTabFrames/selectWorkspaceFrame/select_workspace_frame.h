#ifndef SELECTWORKSPACEFRAME_H
#define SELECTWORKSPACEFRAME_H

#include "../../I_cardframe.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class SelectWorkspaceFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectWorkspaceFrame();

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

private:
    QPushButton *chooseWorkspaceDirPushButton;
    QLabel *dataLabel;
};

#endif // SELECTWORKSPACEFRAME_H
