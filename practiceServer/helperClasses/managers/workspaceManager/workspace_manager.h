#ifndef WORKSPACEMANAGER_H
#define WORKSPACEMANAGER_H

///     Класс WorkspaceManager определяет зону ответственности за рабочую папку
///     Конструктор принимает абсолютный путь папки, над которой требуется менеджмент
///     Переменные:
///     rootFolder - абсолютный путь рабочей директории
///     Методы:
///     createWorkspaceFolders() - создаёт структуру папки и возвращает отчёт в консоль
///     saveSettings() - принимает json объект и создаёт из него файл.json, возвращая отчёт в консоль

#include <QDir>             //  для работы с директориями
#include <QJsonDocument>    //  для создания json документов
#include <QJsonObject>      //  для использования переменных типа QJsonObject

class WorkspaceManager
{
public:
    WorkspaceManager(QString incomingRootFolder);

    QString createWorkspaceFolders();
    QString saveSettings(QJsonObject m_currentJsonObject);

private:
    QString rootFolder;
};

#endif // WORKSPACEMANAGER_H
