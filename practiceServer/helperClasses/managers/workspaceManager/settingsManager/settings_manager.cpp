#include "settings_manager.h"

SettingsManager::SettingsManager(QString rootFolder)
{
    this->rootFolder = rootFolder;  //  путь/Settings

    serverSettingsFileName = rootFolder+"/"+"serverSettings.json";
    possibleProcessingFileName = rootFolder+"/"+"possibleProcessing.json";
}

QString SettingsManager::setSettings(QJsonObject currentJsonObject)
{
    QFileInfo fileProcessingInfo(possibleProcessingFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileProcessingInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile possibleProcessingFile(possibleProcessingFileName);

    //  создание пустой json структуры внутри файла
    if(!possibleProcessingFile.open(QIODevice::WriteOnly)){
        return QString("<hr/>Не удалось сохранить список обработок");
    }
    QJsonObject m_currentJsonObject;
    //  m_currentJsonObject - это просто { }
    m_currentJsonObject.insert("Data", m_currentJsonObject);
    possibleProcessingFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    possibleProcessingFile.close();

    QFileInfo fileInfo(serverSettingsFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(serverSettingsFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return QString("<hr/>Не удалось сохранить настройки");
    }

    // Записываем текущий объект Json в файл
    jsonFile.write(QJsonDocument(currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();

    settingsfilesWatcher = new QFileSystemWatcher();

    settingsfilesWatcher->addPath(possibleProcessingFileName);

    connect(settingsfilesWatcher, &QFileSystemWatcher::fileChanged, this, &SettingsManager::processingFileChanged);

    return QString("<hr/>Настройки сохранены");
}

void SettingsManager::processingFileChanged(const QString &fileName)
{
    emit processingFileChangedSignal(fileName);
}
