#include "workspace_manager.h"

WorkspaceManager::WorkspaceManager()
{

}

QString WorkspaceManager::createWorkspaceFolders()
{
    QDir().mkdir(settingsFolder);
    QDir().mkdir(dataFolder);
    QDir().mkdir(entryFolder);
    QDir().mkdir(expectationFolder);
    QDir().mkdir(storageFolder);

    return QString("<hr/>Рабочая папка организована!");
}

QString WorkspaceManager::saveSettings(QJsonObject m_currentJsonObject)
{
    return m_settingsManager->setSettings(m_currentJsonObject);
}

void WorkspaceManager::setRootFolder(QString incomingRootFolder)
{
    this->rootFolder = incomingRootFolder;

    this->settingsFolder = incomingRootFolder+"/Settings";
    this->dataFolder = incomingRootFolder+"/Data";
    this->entryFolder = incomingRootFolder+"/Data/Entry";
    this->expectationFolder = incomingRootFolder+"/Data/Expectation";
    this->storageFolder = incomingRootFolder+"/Data/Storage";

    this->workspaceWatcher = new QFileSystemWatcher();

    workspaceWatcher->addPath(settingsFolder);
    qDebug() << settingsFolder << workspaceWatcher->directories();
    connect(workspaceWatcher, &QFileSystemWatcher::directoryChanged, this, &WorkspaceManager::workspaceFileChanged);

    m_settingsManager = new SettingsManager(settingsFolder);
}

void WorkspaceManager::workspaceFileChanged(const QString &fileName)
{
    WorkspaceManager::updateUiComboBoxSignal(fileName);
}
