#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //  обозначаем рабочую директорию
    QDir workingDirectory;
//    workingDirectory.setPath("C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\folderForIncomingFromServerRawInformation");
    workingDirectory.setPath("D:\\TEST_DELETELATER");


    //  сверху фильтры, чтобы можно было получить все файлы
    workingDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    workingDirectory.setSorting(QDir::Size | QDir::Reversed);

    //  получаем список из пока одного файла
    QFileInfoList list = workingDirectory.entryInfoList();
    //  и устанавливаем переменную для работы с файлом
    QFile fileToProcess(list.at(0).absoluteFilePath());
    QByteArray block;

    if(fileToProcess.open(QIODevice::ReadOnly)){
        block = fileToProcess.read(fileToProcess.size());

        fileToProcess.close();
        qDebug() << "read";
    } else {
        qDebug() << "no read";
    }

    if(fileToProcess.open(QIODevice::Append)){
        fileToProcess.write(block, fileToProcess.size());

        fileToProcess.close();
        qDebug() << "double";
    } else {
        qDebug() << "no append";
    }
   QString oldName = fileToProcess.fileName();
   if(fileToProcess.rename("processed_"+list.at(0).fileName())){
       qDebug() << 1;
   } else {
       qDebug() << 0;
       qDebug() << "processed_"+list.at(0).fileName();
   }

    return a.exec();
}
