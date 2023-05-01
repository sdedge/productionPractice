#ifndef UPDATEPROCESSINGDIALOG_H
#define UPDATEPROCESSINGDIALOG_H

///     Класс UpdateProcessingDialog реализует интерфейс I_DialogWindow
///     Переменные:
///     jsonDataTextEdit - поле для отображения содержимого json файла
///     dataLabel - метка с подсказкой/данными для наглядности
///     openJsonFilePushButton - кнопка для открытия json файла
///     closeDialogPushButton - кнопка для закрытия диалогового окна
///     applyDataPushButton - кнопка для подтверждения новых данных
///     m_currentJsonObject - json объект для работы с файлом
///     Методы:
///     createInterface() - создание графического представления
///     getValue() - возвращает QVariant данные
///     Приватные слоты:
///     on_openJsonFilePushButton_clicked() - возвращает путь до файла с json данными
///     on_closeDialogPushButton_clicked() - закрывает текущее окно
///     on_applyDataPushButton_clicked() - возвращает сообщение о записи данных в ранее открытый файл

///  ========================    заголовочные файлы проекта
#include "components/dialogs/I_dialogwindow.h"  //  реализуемый интерфейс
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QTextEdit>            //  текстовое поле
#include <QLabel>               //  метка для подсказки
#include <QPushButton>          //  кнопки
#include <QVBoxLayout>          //  вертикальное выравнивание
///  ========================
///
///  ========================    класс для работы с файлами
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>

class UpdateProcessingDialog : public I_DialogWindow
{
public:
    UpdateProcessingDialog(MainWindow *parentUi);

    void createInterface() override;
    QVariant getValue() override;

private:
    MainWindow *parentUi;
    QTextEdit *jsonDataTextEdit;
    QLabel *dataLabel;
    QPushButton *openJsonFilePushButton;
    QPushButton *closeDialogPushButton;
    QPushButton *applyDataPushButton;

    QJsonObject m_currentJsonObject;

private slots:
    QString on_openJsonFilePushButton_clicked();
    void on_closeDialogPushButton_clicked();
    QString on_applyDataPushButton_clicked();
};

#endif // UPDATEPROCESSINGDIALOG_H
