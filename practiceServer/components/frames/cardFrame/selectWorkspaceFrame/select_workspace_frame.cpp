#include "select_workspace_frame.h"

#include <QHBoxLayout>
#include <QFileDialog>

SelectWorkspaceFrame::SelectWorkspaceFrame(MainWindow *parentUi)
{
    this->setObjectName("Choose workspace directory Frame");
    chooseWorkspaceDirPushButton = new QPushButton("Выбрать рабочую папку");

    dataLabel = new QLabel();
    dataLabel->setText("Нет рабочей папки!");
    dataLabel->setObjectName("Data Label");

    connect(chooseWorkspaceDirPushButton, &QPushButton::clicked, parentUi, &MainWindow::on_chooseWorkspaceDirPushButton_clicked);
}

void SelectWorkspaceFrame::createInterface(){
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(chooseWorkspaceDirPushButton);
    frameLayout->addWidget(dataLabel);
}

QVariant SelectWorkspaceFrame::getValue(){
    return dataLabel->text();
}
