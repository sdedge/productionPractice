#ifndef READYREADMANAGER_H
#define READYREADMANAGER_H

///     Класс ReadyReadManager определяет зону ответственности за принимаемые сервером сообщения
///     Переменные:
///     messageManagers - хранит название обработчика и его указатель
///     clientsMessageManager - указатель на менеджер типа "Message"
///     nullManager - указатель на менеджер типа "No type"
///     Методы:
///     identifyMessage() - определяет тип сообщения и возвращает указатель на необходимого обработчика

///  ========================    классы проекта
#include "supportRRManagers/I_message_manager.h"    //  для работы с ссылками обработчиков
#include "supportRRManagers/nullManager/null_manager.h"    //  для определения менеджера "заглушки"
#include "supportRRManagers/clientsMessageManager/clients_message_manager.h"    //  для определения менеджера "Message"
///  ========================
///
///  ========================    классы для работы
#include <QObject>
#include <QMap>                 //  для хранения менеджеров

class ReadyReadManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString, I_MessageManager*> messageManagers;

    I_MessageManager *clientsMessageManager;
    I_MessageManager *nullManager;
public:
    ReadyReadManager();
    I_MessageManager* identifyMessage(QString message);
};

#endif // READYREADMANAGER_H
