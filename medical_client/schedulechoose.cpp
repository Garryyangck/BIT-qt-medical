#include <QTcpSocket>
#include <QString>
#include <message.h>

#include "schedulechoose.h"
#include "ui_schedulechoose.h"
#include "appointmentchoose.h"
#include "user.h"

schedulechoose::schedulechoose(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::schedulechoose)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;
}

schedulechoose::~schedulechoose()
{
    delete ui;
}

void schedulechoose::on_schedule_return_clicked()
{
    appointmentchoose *appcho = new appointmentchoose(socket, user);
    appcho->setWindowTitle("挂号检索");
    appcho->show();
    this->hide();
}

void schedulechoose::on_schedule_sure_clicked()
{
    Message requestMsg(this->user, new User, QString(), MessageType::SCHDULE_SEARCH);
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::SCHDULE_SEARCH_SUC)
        {
            QHash<QString, QStringList> table = Utils::decompressJsonToQHash(msg.content);

            ui->schedit->clear();
            QStringList keys = table.keys();
            for (const QString& key : keys) {
                QString line;
                line.append(key + QString(": ")).append(QChar('\n'));
                QStringList schedules = table[key];
                for (const QString &schedule : schedules) {
                    QStringList parts = schedule.split("_");
                    QString doctor_name = parts[0];
                    QString department = parts[1];
                    QString schedule_date = parts[2];
                    QString available_num = parts[3];
                    line.append(doctor_name + " " + department + " " + schedule_date + " 剩余" + available_num).append(QChar('\n'));
                }
                line.append(QChar('\n'));
                ui->schedit->append(line);
            }
        }
    });
}
