#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include <QDebug>

#include "user.h"
#include "messagetype.h"

class Message
{
public:
    Message();
    Message(User *sender, User *receiver, QString content, MessageType messageType);

    static QByteArray messageToByteArray(const Message &message);

    static Message byteArrayToMessage(const QByteArray &byteArray);

    static Message serverReplyMessage(MessageType messageType);

    void print();

    User *sender;
    User *receiver;
    QString content;
    MessageType messageType;
};

#endif // MESSAGE_H
