#ifndef FILEFRAME_H
#define FILEFRAME_H

#include "../../I_cardframe.h"
#include "mainwindow.h"         //  родительский ui
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class FileFrame : public I_CardFrame
{
    Q_OBJECT
public:
    FileFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

private:
    QLabel *filePathLabel;
    QLineEdit *filePathLineEdit;
    QPushButton *openFilePushButton;
    QPushButton *sendFilePushButton;
    MainWindow *parentUi;
};

#endif // FILEFRAME_H
