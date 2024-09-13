#include "message.h"

Message::Message()
{
    this->sender = new User;
    this->receiver = new User;
    this->content = QString("");
    this->messageType = MessageType();
}

Message::Message(User *sender, User *receiver, QString content, MessageType messageType) {
    this->sender = sender;
    this->receiver = receiver;
    this->content = QString(content);
    this->messageType = messageType;
}

QByteArray Message::messageToByteArray(const Message &message) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);

    // 序列化User对象，这里简化为只序列化username, password, role
    stream << message.sender->username << message.sender->password << message.sender->role;
    stream << message.receiver->username << message.receiver->password << message.receiver->role;
    stream << message.content << static_cast<int>(message.messageType);

    return byteArray;
}

Message Message::byteArrayToMessage(const QByteArray &byteArray) {
    QDataStream stream(byteArray);
    User *sender = new User();
    User *receiver = new User();
    QString content;
    int messageTypeInt;

    // 反序列化User对象
    stream >> sender->username >> sender->password >> sender->role;
    stream >> receiver->username >> receiver->password >> receiver->role;
    stream >> content >> messageTypeInt;
    MessageType messageType = static_cast<MessageType>(messageTypeInt);

    return Message(sender, receiver, content, messageType);

}

void Message::print() {
    qDebug() << sender->username << sender->password << sender->role;
    qDebug() << receiver->username << receiver->password << receiver->role;
    qDebug() << content;
    qDebug() << messageType;
}

Message Message::serverReplyMessage(MessageType messageType) {
    return Message(new User, new User, QString(), messageType);
}
