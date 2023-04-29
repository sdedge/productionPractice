#include "workspace_manager.h"

WorkspaceManager::WorkspaceManager(QString incomingRootFolder)
{
    this->rootFolder = incomingRootFolder;
}

QString WorkspaceManager::createWorkspaceFolders()
{
    QDir().mkdir(rootFolder+"/Settings");
    QDir().mkdir(rootFolder+"/Data");
    QDir().mkdir(rootFolder+"/Data/Entry");
    QDir().mkdir(rootFolder+"/Data/Expectation");
    QDir().mkdir(rootFolder+"/Data/Storage");

    return QString("<hr/>Рабочая папка организована!");
}

QString WorkspaceManager::saveSettings(QJsonObject m_currentJsonObject)
{
    QString saveFileName = "serverSettings.json";
    QFileInfo fileInfo(rootFolder+"/Settings/"+saveFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return QString("<hr/>Не удалось сохранить настройки");
    }

    // Записываем текущий объект Json в файл
    jsonFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();

    return QString("<hr/>Файл настроек сохранён");
}
