#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    clientsMessageManager = new ClientsMessageManager();
    nullManager = new NullManager();

    messageManagers[clientsMessageManager->typeOfMessage()] = clientsMessageManager;
}

I_MessageManager *ReadyReadManager::identifyMessage(QString message)
{
    for(auto it = messageManagers.begin(); it != messageManagers.end(); it++){
        if(it.key() == message){
            return it.value();
        }
    }
    return nullManager;
}
