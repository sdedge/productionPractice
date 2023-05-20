#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    clientsMessageManager = new ClientsMessageManager();
    connect(clientsMessageManager, &ClientsMessageManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(clientsMessageManager, &ClientsMessageManager::signalSendToAllClientsRRManager, this, &ReadyReadManager::slotSendToAllClientsRRManager);

    clientsFileManager = new ClientsFileManager();
    connect(clientsFileManager, &ClientsFileManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(clientsFileManager, &ClientsFileManager::signalSendToOneRRManager, this, &ReadyReadManager::slotSendToOneRRManager);

    nullManager = new NullManager();
    messageManagers[clientsMessageManager->typeOfMessage()] = clientsMessageManager;
    messageManagers[clientsFileManager->typeOfMessage()] = clientsFileManager;
}

I_MessageManager *ReadyReadManager::identifyMessage(QString typeOfMess)
{
    /// Создать список всех typeOfMess и пробегаться по нему
    /// При совпадении создавать new конструктор()
    /// иначе возвращать nullManager
    for(auto it = messageManagers.begin(); it != messageManagers.end(); it++){
        if(it.key() == typeOfMess){
            return it.value();
        }
    }
    return nullManager;
}

void ReadyReadManager::setEntryFolder(QString &entryFolder)
{
    clientsFileManager->setEntryFolderName(entryFolder);
}

void ReadyReadManager::slotStatusRRManager(QString status)
{
    qDebug() << "ReadyReadManager::slotStatusRRManager:     " << status;
    emit signalStatusRRManagerServer(status);
}

void ReadyReadManager::slotSendToAllClientsRRManager(QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToAllClientsRRManager:     " << typeOfMsg << " | " << str;
    emit signalSendToAllClientsServer(typeOfMsg, str);
}

void ReadyReadManager::slotSendToOneRRManager(QTcpSocket *socket, QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToOneRRManager:     " << typeOfMsg << " | " << str;
    emit signalSendToOneRRManager(socket, typeOfMsg, str);
}
