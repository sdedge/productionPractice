#ifndef JSONPARSER_H
#define JSONPARSER_H

///     Класс JsonParser определяет, ликвидная ли json запись
///     Методы:
///     isJson() - возвращает булевое значение, парся данные в формате QByteArray

///  ========================    классы для работы с директориями
#include <QJsonParseError>
#include <QJsonDocument>

class JsonParser : public QJsonParseError
{
    Q_OBJECT
public:
    JsonParser();

    bool isJson(QByteArray &jsonData);
};

#endif // JSONPARSER_H
