#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QTcpSocket *socket, User *user, QString receiver, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    this->socket = socket;
    this->user = user;
    this->receiver = QString(receiver);
    ui->setupUi(this);

    this->setWindowTitle(QString("即时聊天平台"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    // 显示聊天对象
    ui->recerverLabel->setText(QString("您正在和 ") + getFullname(receiver, user->role == 0 ? 1 : 0) + QString(" 沟通："));

    // 请求服务器，获取历史信息
    Message msg(user, new User(receiver, QString(), user->role == 0 ? 1 : 0), QString(), MessageType::GET_HISTORY);
    socket->write(Message::messageToByteArray(msg));

    // 监听收到信息，注意判断消息类型，此时可能收到多种消息，注意这里必须disconnect
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        msg.print();

        if (msg.messageType == MessageType::RECEIVE_MESSAGE) { // 收到消息
            // 判断一下是否是当前我正在聊天的用户发给我的消息
            if (msg.content.split("_")[3] == this->receiver) {
                // 收到的消息显示到对话框里
                QString text = ui->chatTextEdit->toPlainText();
                text.append(getTextLine(msg.content));
                ui->chatTextEdit->setText(text);
            } else { // 不是当前正在聊天的对象发给我的

            }

        } else if (msg.messageType == MessageType::SEND_SUCCESS)  { // 发送成功，对方在线
            // 自己发的消息显示到对话框里
            QString text = ui->chatTextEdit->toPlainText();
            text.append(getTextLine(msg.content));
            ui->chatTextEdit->setText(text);

        } else if (msg.messageType == MessageType::RECEIVER_OFFLINE) { // 对方不在线
            QMessageBox::information(0, QString("对方不在线"), QString("对方不在线，我们会在对方登录时转发您的消息"));

            // 自己发的消息显示到对话框里
            QString text = ui->chatTextEdit->toPlainText();
            text.append(getTextLine(msg.content));
            ui->chatTextEdit->setText(text);

        } else if (msg.messageType == MessageType::RET_HISTORY) { // 返回历史记录
            QStringList lines = msg.content.split("|");
            QString text = ui->chatTextEdit->toPlainText();
            for(const QString &line : lines) {
                text.append(getTextLine(line));
            }
            ui->chatTextEdit->setText(text);

        } else if (msg.messageType == MessageType::UPDATE_ISREAD_SUCCESS) { // 服务器修改已读成功
            // 在这里disconnect
            disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);
            this->close();
        }
    });

    // 回车键直接发送
    connect(ui->msgLineEdit, &QLineEdit::returnPressed, this, &Chat::on_sendButton_clicked);
}

Chat::~Chat()
{
    // 发送退出的请求
    Message msg(user, new User, QString(), MessageType::QUIT);
    socket->write(Message::messageToByteArray(msg));
    delete ui;
}

void Chat::on_returnButton_clicked()
{
    emit returnToProfile();

    // 退出的时候，告诉服务器自己已读，即把自己历史记录里面的所有消息设置为已读
    Message msg(user, new User(receiver, QString(), user->role == 0 ? 1 : 0), QString(), MessageType::UPDATE_ISREAD);
    socket->write(Message::messageToByteArray(msg));

    // disconnect和close移到上面判断接收消息类型里面去了
}

void Chat::on_sendButton_clicked()
{
    QString content = ui->msgLineEdit->text();
    ui->msgLineEdit->setText(QString());
    if (content.trimmed() == QString("")) {
        QMessageBox::warning(0, QString("发送失败"), QString("至少写点什么再发吧"));
        return;
    }

    // 构造Message对象，只需发送就可以了，不需要在这里connect，因为已经在构造函数里面connect了
    User *rcver = new User(receiver, QString(), user->role == 0 ? 1 : 0);
    Message msg(user, rcver, content, MessageType::SEND_MESSAGE);
    socket->write(Message::messageToByteArray(msg));
}

QString Chat::getFullname(QString username, int role) {
    QString fullname = QString("");
    fullname.append(username);
    fullname.append(QString(" ("));
    QString role_ = role == 0 ? QString("医生") : QString("患者");
    fullname.append(role_);
    fullname.append(QString(") "));
    return fullname;
}

QString Chat::getTextLine(QString line) {
    if (line == QString())
        return line;

    QStringList parts = line.split("_");
    QString content = parts[0];
    QString chatDate = parts[1];
    int isRead = parts[2].toInt();
    QString sender = parts[3];
    int role = sender == this->user->username ? this->user->role : (this->user->role == 0 ? 1 : 0);

    QString textLine;

    textLine.append(getFullname(sender, role).append(QString("：")).append(content)
                    .append(isRead == 0 && user->username != sender ? QString("(未读)") : QString()).append(QChar('\n'))
                    .append(QString("------------------------------------")).append(chatDate)
                    .append(QChar('\n')));

    return textLine;
}

QString Chat::getLine(QString content, QString chatDate, QString isRead, QString sender) {
    return content + QString("_") + chatDate + QString("_") + isRead +QString("_") + sender;
}
