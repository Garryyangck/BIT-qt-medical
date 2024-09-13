#include "utils.h"

Utils::Utils(QObject *parent) : QObject(parent)
{

}

QString Utils::blankMsg = QString("未填");

QString Utils::checkBlank(QString line) {
    if (line.trimmed() == QString()) {
        return QString(blankMsg);
    } else {
       return line;
    }
}

// 将 QHash 压缩为 JSON 字符串
QString Utils::compressQHashToJson(const QHash<QString, QStringList>& hash) {
    QJsonObject jsonObject;
    for (auto it = hash.constBegin(); it != hash.constEnd(); ++it) {
        QJsonArray jsonArray;
        for (const QString& value : it.value()) {
            jsonArray.append(value);
        }
        jsonObject.insert(it.key(), jsonArray);
    }
    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

// 将 JSON 字符串解压缩为 QHash
QHash<QString, QStringList> Utils::decompressJsonToQHash(const QString& jsonString) {
    QHash<QString, QStringList> decompressedHash;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();
        for (auto it = jsonObject.constBegin(); it != jsonObject.constEnd(); ++it) {
            QStringList list;
            QJsonArray jsonArray = it.value().toArray();
            for (const QJsonValue& value : jsonArray) {
                list.append(value.toString());
            }
            decompressedHash.insert(it.key(), list);
        }
    }
    return decompressedHash;
}
