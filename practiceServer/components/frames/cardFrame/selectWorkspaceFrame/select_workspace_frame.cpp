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

    this->parentUi = parentUi;

    connect(chooseWorkspaceDirPushButton, &QPushButton::clicked, parentUi, &MainWindow::on_chooseWorkspaceDirPushButton_clicked);
}

void SelectWorkspaceFrame::createInterface(){
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(chooseWorkspaceDirPushButton);
    frameLayout->addWidget(dataLabel);
}

QMap<QString, QVariant> SelectWorkspaceFrame::getValue(){
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлена новая директория сохранения: "+dataLabel->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, dataLabel->text());
    return valueMap;
}

void SelectWorkspaceFrame::setValue(QVariant value){
    dataLabel->setText(value.toString());
    emit parentUi->signalNewWorkspaceFolder(value.toString());  //  вызываем сигнал для установки значения на стороне сервера
}
