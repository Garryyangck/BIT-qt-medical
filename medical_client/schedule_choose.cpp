#include "schedule_choose.h"
#include "ui_schedule_choose.h"

schedule_choose::schedule_choose(QTcpSocket *socket, User *user, QString dep, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::schedule_choose)
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

            ui->choose_schedule->clear();

            QStringList filtered;
            for(const QString &item : depList)
            {
                if(!item.trimmed().isEmpty()) filtered.append(item);
            }
            ui->choose_schedule->addItems(filtered);
        }
    });

}

schedule_choose::~schedule_choose()
{
    disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);
    delete ui;
}

void schedule_choose::on_returnbutton_clicked()
{
    department_choose *depc = new department_choose(socket, user);
    depc->setWindowTitle(QString("选择科室"));
    depc->show();
    delete this;
}

void schedule_choose::on_choose_schedule_activated(const QString &schedule_date)
{
    QString content = schedule_date +"_" + this->dep;
    Message msg(this->user, new User, content, MessageType::GET_SCHEDULE_BY_DEP_DATE);

    // 发送注册请求
    this->socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(this->socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::GET_SCHEDULE_BY_DEP_DATE_SUC)
        {
            QStringList list = msg.content.split("|");

            ui->see_schedule->clear();
            for (const QString &schedule : list) {
                QStringList parts = schedule.split("_");
                QString doctor_name = parts[0];
                QString department = parts[1];
                QString schedule_date = parts[2];
                QString available_num = parts[3];
                QString line = doctor_name + " " + department + " " + schedule_date + " 剩余" + available_num + QString(QChar('\n'));
                ui->see_schedule->append(line);
            }
        }
    });

}

void schedule_choose::on_surebutton_clicked()
{
    QString date = ui->choose_schedule->currentText();
    QString docname = ui->inputdocname->text();
    if (date == QString("")) {
        QMessageBox::warning(0, QString("挂号失败"), QString("时间必须选择"));
        return;
    }
    if (docname == QString(""))
    {
        QMessageBox::warning(0, QString("挂号失败"), QString("医生必须选择"));
    }
    QString content = this->dep + "_" + date + "_" + docname;

    Message msg(this->user, new User, content, MessageType::APPOINT);

    // 发送注册请求
    socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::APPOINT_SUC) {
            QMessageBox:: information(0, QString("提示"), QString("挂号成功"));
            PatientProfile *pp = new PatientProfile(socket, user);
            pp->setWindowTitle("患者界面");
            pp->show();
            this->hide();
        } else if (msg.messageType == MessageType::APPOINT_FAI) {
            QMessageBox:: information(0, QString("提示"), QString("挂号失败，当前时间下没有该医生"));
        }
    });
}

