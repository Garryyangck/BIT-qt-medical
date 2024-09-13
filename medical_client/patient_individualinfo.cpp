#include "patient_individualinfo.h"
#include "ui_patient_individualinfo.h"

patient_individualinfo::patient_individualinfo (QTcpSocket *socket, User *user, QWidget *parent):
    QWidget(parent),
    ui(new Ui::patient_individualinfo)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    Message msg(user, new User, user->username, MessageType::GET_PATIENT_PROFILE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::RET_PATIENT_PROFILE)
        {
            QStringList parts = msg.content.split("_");
            QString name = parts[0];
            QString gender = parts[1];
            QString age = parts[2];
            QString phone_number = parts[3];
            ui->gender->setText(gender);
            ui->age->setText(age);
            ui->phonenumber->setText(phone_number);
        }
    });
}

patient_individualinfo::~patient_individualinfo()
{
    delete ui;
}

void patient_individualinfo::on_patient_info_return_clicked()
{
    PatientProfile *pp = new PatientProfile(socket, user);
    pp->setWindowTitle(QString("患者界面"));
    pp->show();
    this->hide();
}

void patient_individualinfo::on_patient_info_sure_clicked()
{
    QString gen_ = Utils::checkBlank(ui->gender->text());
    QString age_ = Utils::checkBlank(ui->age->text());
    QString pho_ = Utils::checkBlank(ui->phonenumber->text());
    QString content = this->user->username + "_" + gen_ +
                                "_" + age_ + "_" + pho_;

    Message msg(this->user, new User, content, MessageType::CHANGE_INFO_P);

    // 发送注册请求
    socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::CHANGE_INFO_P_SUCCESS)
        {
            QMessageBox:: information(0, QString("提示"), QString("修改成功"));
            PatientProfile *pp = new PatientProfile(socket, user);
            pp->setWindowTitle("患者界面");
            pp->show();
            this->hide();
        }
        else if(msg.messageType == MessageType::CHANGE_INFO_P_FAILED)
        {
            QMessageBox:: information(0, QString("提示"), QString("修改异常"));
        }
    });
}
