#ifndef I_MESSAGEMANAGER_H
#define I_MESSAGEMANAGER_H

///     Класс I_MessageManager является интерфейсом для обработчиков сообщений слота сервера ReadyRead
///     Методы:
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

#include <QObject>  //  для создания объекта

class I_MessageManager : public QObject
{
    Q_OBJECT
public:
    I_MessageManager();
    virtual void processData(QDataStream &in) = 0;
    virtual QString typeOfMessage() = 0;
};

#endif // I_MESSAGEMANAGER_H
