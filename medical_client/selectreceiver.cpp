#include "selectreceiver.h"
#include "ui_selectreceiver.h"

SelectReceiver::SelectReceiver(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectReceiver)
{
    this->socket = socket;
    this->user = user;
    ui->setupUi(this);

    this->setWindowTitle(QString("选人页面"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    // 构造请求查看可聊天的Message
    Message msg(user, new User, QString(), MessageType::GET_RECEIVER);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器返回的可聊天的对象";
        msg.print();

        // 关闭connect，防止抢夺其它地方的消息
        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::RET_RECEIVER) {
            // 服务器返回的聊天人选receivers username，以“_”分隔
            if (msg.content.trimmed() == QString()) {
                ui->receiverComboBox->addItem(QString("没有联系人"));
                ui->receiverTextEdit->setText(QString("没有联系人"));
                return;
            }

            QStringList receivers = msg.content.split("_");
            QString receiverRole = this->user->role == 0 ? QString("患者") : QString("医生");
            for (const QString &receiver : receivers) {
                if (!table.contains(receiver)) {
                    table.insert(receiver, 0);
                } else {
                    table[receiver]++;
                }
            }
            receivers = table.keys();
            QString text = ui->receiverTextEdit->toPlainText();
            for (const QString &receiver : receivers) {
                QString line;
                line.append(receiver);
                line.append(QString("  "));
                line.append(receiverRole);
                text.append(line).append(QChar('\n'));
                ui->receiverComboBox->addItem(line);
            }
            ui->receiverTextEdit->setText(text);
        }
    });

    // 回车直接发送
    connect(ui->msgLineEdit, &QLineEdit::returnPressed, this, &SelectReceiver::on_confirmButton_clicked);
}

SelectReceiver::~SelectReceiver()
{
    delete ui;
}

void SelectReceiver::on_quitButton_clicked()
{
    emit returnToProfile();
    this->close();
}

void SelectReceiver::on_confirmButton_clicked()
{
    QString receiver = ui->msgLineEdit->text();
    if (!table.contains(receiver)) {
        QMessageBox::critical(0, "选择失败", "您要联系的对象不存在");
        ui->msgLineEdit->setText(QString());
        return;
    } else {
        emit completeSelect(receiver);
        this->close();
    }
}

void SelectReceiver::on_receiverComboBox_activated(const QString &receiver)
{
    ui->msgLineEdit->setText(receiver.split("  ")[0]);
    ui->msgLineEdit->setFocus();
}
