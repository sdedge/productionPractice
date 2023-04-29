#ifndef WORKSPACEMANAGER_H
#define WORKSPACEMANAGER_H

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

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
