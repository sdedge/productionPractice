#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //  обозначаем рабочую директорию
    QDir workingDirectory;
    workingDirectory.setPath("C:\\Users\\dvetr\\OneDrive\\Рабочий стол\\folderForIncomingFromServerRawInformation");
//    workingDirectory.setPath("D:\\TEST_DELETELATER");


    //  сверху фильтры, чтобы можно было получить все файлы
    workingDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    workingDirectory.setSorting(QDir::Size | QDir::Reversed);

    //  получаем список из пока одного файла
    QFileInfoList list = workingDirectory.entryInfoList();
    //  и устанавливаем переменную для работы с файлом
    QFile fileToProcess(list.at(0).absoluteFilePath());

    //  делаем строку из абсолютного пути, нового названия и старого названия файла
    QFile newProcessedFile(list.at(0).absolutePath()+"/processed_"+list.at(0).fileName());
    qDebug() << "new name: " << list.at(0).absolutePath()+"/processed_"+list.at(0).fileName();
    QByteArray block;

    if(fileToProcess.open(QIODevice::ReadOnly)){
        block = fileToProcess.read(fileToProcess.size());

        fileToProcess.close();
        qDebug() << "fileToProcess read";
    } else {
        qDebug() << "fileToProcess no read";
    }

    if(newProcessedFile.open(QIODevice::Append)){
        newProcessedFile.write(block, fileToProcess.size());
        newProcessedFile.write(block, fileToProcess.size());

        newProcessedFile.close();
        qDebug() << "newProcessedFile double";
    } else {
        qDebug() << "newProcessedFile no append";
    }
//   QString oldName = fileToProcess.fileName();
//   if(fileToProcess.rename("processed_"+list.at(0).fileName())){
//       qDebug() << 1;
//   } else {
//       qDebug() << 0;
//       qDebug() << "processed_"+list.at(0).fileName();
//   }

//   fileToProcess.setFileName("processed_"+list.at(0).fileName());
//   qDebug() << fileToProcess.fileName();

    return a.exec();
}
