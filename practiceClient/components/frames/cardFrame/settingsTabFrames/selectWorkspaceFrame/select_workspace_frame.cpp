#include "select_workspace_frame.h"

SelectWorkspaceFrame::SelectWorkspaceFrame()
{
    dataLabel = new QLabel("Не выбрана рабочая папка");

    chooseWorkspaceDirPushButton = new QPushButton("Выбрать папку");
}

void SelectWorkspaceFrame::createInterface()
{
    QHBoxLayout *mainHLayout = new QHBoxLayout();
    this->setLayout(mainHLayout);

    mainHLayout->addWidget(chooseWorkspaceDirPushButton);
    mainHLayout->addWidget(dataLabel);
}

QMap<QString, QVariant> SelectWorkspaceFrame::getValue()
{

}

void SelectWorkspaceFrame::setValue(QVariant value)
{

}

void SelectWorkspaceFrame::switchEnabledInteface()
{

}
