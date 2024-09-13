#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);

    static QString blankMsg;

    static QString checkBlank(QString line);

    // 将 QHash 压缩为 JSON 字符串
    static QString compressQHashToJson(const QHash<QString, QStringList>& hash);

    // 将 JSON 字符串解压缩为 QHash
    static QHash<QString, QStringList> decompressJsonToQHash(const QString& jsonString);

signals:

};

#endif // UTILS_H
