#include "settings_manager.h"

SettingsManager::SettingsManager(QString rootFolder)
{
    this->rootFolder = rootFolder;  //  путь/Settings

    serverSettingsFileName = "serverSettings.json";
    possibleProcessingFileName = "possibleProcessing.json";

    QFileInfo fileInfo(rootFolder+"/"+serverSettingsFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(serverSettingsFileName);

    QFile possibleProcessingFile(rootFolder+"/"+possibleProcessingFileName);

    //  создание пустой json структуры внутри файла
    possibleProcessingFile.open(QIODevice::WriteOnly);
    QJsonObject m_currentJsonObject;
    possibleProcessingFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    possibleProcessingFile.close();
}

QString SettingsManager::setSettings(QJsonObject currentJsonObject)
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
    jsonFile.write(QJsonDocument(currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();

    return QString("<hr/>Файл настроек сохранён");
}
