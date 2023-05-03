#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

///     Класс SettingsManager определяет зону ответственности за папку настроек
///     Конструктор принимает абсолютный путь папки настроек, над которой требуется менеджмент
///     Переменные:
///     rootFolder - абсолютный путь папки настроек
///     Методы:
///     saveSettings() - принимает json объект и создаёт из него файл.json, возвращая отчёт в консоль
///     Приватные слоты:
///     slotSettingsFolderChanged() - возвращает название файла, который изменился

///  ========================    классы проекта
///
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    SettingsManager(QString rootFolder);

    QString setSettings(QJsonObject currentJsonObject);

private:
    QString rootFolder;
    QString serverSettingsFileName;
    QString possibleProcessingFileName;
    QFileSystemWatcher *settingsFolderWatcher;
};

#endif // SETTINGSMANAGER_H
