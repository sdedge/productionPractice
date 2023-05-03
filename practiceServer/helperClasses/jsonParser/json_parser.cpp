#include "json_parser.h"

JsonParser::JsonParser()
{

}

bool JsonParser::isJson(QByteArray &jsonData)
{
    QJsonParseError parserError;
    //  при создании документа происходит парсинг данных
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parserError);

    //  если парсер нашёл любую ошибку
    if(parserError.error != 0){
        return false;
    }

    return true;
}
