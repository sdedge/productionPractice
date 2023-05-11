#ifndef FILEMANAGER_H
#define FILEMANAGER_H

///     Класс FileManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с файлами
///     Переменные:
///     Методы:
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================

class FileManager : public I_MessageManager
{
    Q_OBJECT
public:
    FileManager();
    void processData(QDataStream &in) override;
    QString typeOfMessage() override;
};

#endif // FILEMANAGER_H
