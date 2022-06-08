/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *connectToServerPushButton;
    QTextBrowser *textBrowser;
    QPushButton *sendMsgPushButton;
    QLineEdit *lineEdit;
    QLineEdit *filePathLineEdit;
    QPushButton *openFilePushButton;
    QLabel *filePathLabel;
    QPushButton *sendFilePushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(453, 539);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 431, 471));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        connectToServerPushButton = new QPushButton(gridLayoutWidget);
        connectToServerPushButton->setObjectName(QString::fromUtf8("connectToServerPushButton"));

        gridLayout->addWidget(connectToServerPushButton, 0, 0, 1, 2);

        textBrowser = new QTextBrowser(gridLayoutWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout->addWidget(textBrowser, 1, 0, 1, 2);

        sendMsgPushButton = new QPushButton(gridLayoutWidget);
        sendMsgPushButton->setObjectName(QString::fromUtf8("sendMsgPushButton"));

        gridLayout->addWidget(sendMsgPushButton, 2, 1, 1, 1);

        lineEdit = new QLineEdit(gridLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 0, 1, 1);

        filePathLineEdit = new QLineEdit(gridLayoutWidget);
        filePathLineEdit->setObjectName(QString::fromUtf8("filePathLineEdit"));

        gridLayout->addWidget(filePathLineEdit, 5, 0, 1, 1);

        openFilePushButton = new QPushButton(gridLayoutWidget);
        openFilePushButton->setObjectName(QString::fromUtf8("openFilePushButton"));

        gridLayout->addWidget(openFilePushButton, 5, 1, 1, 1);

        filePathLabel = new QLabel(gridLayoutWidget);
        filePathLabel->setObjectName(QString::fromUtf8("filePathLabel"));
        filePathLabel->setAlignment(Qt::AlignCenter);
        filePathLabel->setWordWrap(true);

        gridLayout->addWidget(filePathLabel, 4, 0, 1, 1);

        sendFilePushButton = new QPushButton(gridLayoutWidget);
        sendFilePushButton->setObjectName(QString::fromUtf8("sendFilePushButton"));

        gridLayout->addWidget(sendFilePushButton, 6, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 453, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        connectToServerPushButton->setText(QCoreApplication::translate("MainWindow", "Connect to Server", nullptr));
        sendMsgPushButton->setText(QCoreApplication::translate("MainWindow", "Send msg", nullptr));
        filePathLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter file path", nullptr));
        openFilePushButton->setText(QCoreApplication::translate("MainWindow", "Or open file", nullptr));
        filePathLabel->setText(QString());
        sendFilePushButton->setText(QCoreApplication::translate("MainWindow", "Send file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
