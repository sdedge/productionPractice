#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    serverMessageManager = new ServerMessageManager();
    connect(serverMessageManager, &ServerMessageManager::signalMessageRRManager, this, &ReadyReadManager::slotMessageRRManager);

    possibleProcessingManager = new PossibleProcessingManager();
    connect(possibleProcessingManager, &PossibleProcessingManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(possibleProcessingManager, &PossibleProcessingManager::signalSetCBDataRRManager, this, &ReadyReadManager::slotSetCBDataRRManager);
//    clientsFileManager = new ClientsFileManager();
//    connect(clientsFileManager, &ClientsFileManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
//    connect(clientsFileManager, &ClientsFileManager::signalSendToOneRRManager, this, &ReadyReadManager::slotSendToOneRRManager);

    nullManager = new NullManager();
    messageManagers[serverMessageManager->typeOfMessage()] = serverMessageManager;
    messageManagers[possibleProcessingManager->typeOfMessage()] = possibleProcessingManager;
//    messageManagers[clientsFileManager->typeOfMessage()] = clientsFileManager;
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
//    clientsFileManager->setEntryFolderName(entryFolder);
}

void ReadyReadManager::slotMessageRRManager(QString message)
{
    qDebug() << "ReadyReadManager::slotMessageRRManager:     " << message;
    emit signalMessageRRManagerClient(message);
}

void ReadyReadManager::slotStatusRRManager(QString status)
{
    qDebug() << "ReadyReadManager::slotStatusRRManager:     " << status;
    emit signalStatusRRManagerClient(status);
}

void ReadyReadManager::slotSetCBDataRRManager(QMap<QString, QVariant> &possibleProcessingData)
{
    qDebug() << "ReadyReadManager::slotSetCBDataRRManager:     possibleProcessingData.size():" << possibleProcessingData.size();
    emit signalSetCBData(possibleProcessingData);
}

void ReadyReadManager::slotSendToAllClientsRRManager(QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToAllClientsRRManager:     " << typeOfMsg << " | " << str;
    emit signalSendToAllClientsServer(typeOfMsg, str);
}

void ReadyReadManager::slotSendToOneRRManager(QTcpSocket *socket, QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToOneRRManager:     " << typeOfMsg << " | " << str;
    emit signalSendToAllClientsServer(typeOfMsg, str);
}
