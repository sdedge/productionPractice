#include "processing_manager.h"

#include <QDebug>

ProcessingManager::ProcessingManager()
{

}

QFileInfoList ProcessingManager::entryFiles(const QString &folderName)
{
    qDebug() << "ProcessingManager::entryFiles  " << folderName;
    QDir workWithDirectory;     //  создаем рабочую директорию
    workWithDirectory.cd(folderName); //  переходим в нужный каталог

    workWithDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir :: NoDotAndDotDot);     //  устанавливаем фильтр выводимых файлов/папок
    workWithDirectory.setSorting(QDir::Time | QDir::Reversed);  //  устанавливаем сортировку "от большему к меньшему"

    return workWithDirectory.entryInfoList();
}
