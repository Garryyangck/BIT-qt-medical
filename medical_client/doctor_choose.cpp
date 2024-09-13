#include "doctor_choose.h"
#include "ui_doctor_choose.h"

doctor_choose::doctor_choose(QTcpSocket *socket, User *user, QString dep, QWidget *parent):
    QWidget(parent),
    ui(new Ui::doctor_choose)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;
    this->dep = dep;
    QString content = this->dep;

    Message msg(this->user, new User, content, MessageType::GET_DATE_BY_DEP);

    // 发送注册请求
    this->socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(this->socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::GET_DATE_BY_DEP_SUC)
        {
            QStringList depList = msg.content.split("|");
            depList.removeDuplicates();

            ui->choose_doctor->clear();

            QStringList filtered;
            for(const QString &item : depList)
            {
                if(!item.trimmed().isEmpty()) filtered.append(item);
            }
            ui->choose_doctor->addItems(filtered);
        }
    });
}

doctor_choose::~doctor_choose()
{
    delete ui;
}

void doctor_choose::on_returnbutton_clicked()
{
    department_choose *dep = new department_choose(socket, user);
    dep->setWindowTitle(QString("选择科室"));
    dep->show();
    this->hide();
}
