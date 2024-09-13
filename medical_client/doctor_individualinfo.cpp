#include "doctor_individualinfo.h"
#include "ui_doctor_individualinfo.h"

doctor_individualinfo::doctor_individualinfo(QTcpSocket *socket, User *user, QWidget *parent):
    QWidget(parent),
    ui(new Ui::doctor_individualinfo)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    Message msg(user, new User, user->username, MessageType::GET_DOCTOR_PROFILE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::RET_DOCTOR_PROFILE)
        {
            QStringList parts = msg.content.split("_");
            QString name = parts[0];
            QString gender = parts[1];
            QString age = parts[2];
            QString phone_number = parts[3];
            QString department = parts[4];
            ui->gender->setText(gender);
            ui->age->setText(age);
            ui->phonenumber->setText(phone_number);
            ui->department->setText(department);
        }
    });
}

doctor_individualinfo::~doctor_individualinfo()
{
    delete ui;
}

void doctor_individualinfo::on_doctor_info_sure_clicked()
{
    // 判空，拼接字符串的地方，都要判空
    // 主要是客户端：用户输入可能为空，需要判断
    // 其实服务端不需要判空，因为服务端的数据是从数据库查出来的
    QString gen_ = Utils::checkBlank(ui->gender->text());
    QString age_ = Utils::checkBlank(ui->age->text());
    QString pho_ = Utils::checkBlank(ui->phonenumber->text());
    QString dep_ = Utils::checkBlank(ui->department->text());
    QString content = this->user->username + "_" + gen_ +
                                "_" + age_ + "_" + pho_ + "_" + dep_;

    Message msg(this->user, new User, content, MessageType::CHANGE_INFO_D);

    // 发送注册请求
    socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::CHANGE_INFO_D_SUCCESS)
        {
            QMessageBox:: information(0, QString("提示"), QString("修改成功"));
            DoctorProfile *dp = new DoctorProfile(socket, user);
            dp->setWindowTitle("医生界面");
            dp->show();
            this->hide();
        }
        else if(msg.messageType == MessageType::CHANGE_INFO_D_FAILED)
        {
            QMessageBox:: information(0, QString("提示"), QString("修改异常"));
        }
    });
}

void doctor_individualinfo::on_doctor_info_return_clicked()
{
    DoctorProfile *dp = new DoctorProfile(socket, user);
    dp->setWindowTitle("医生界面");
    dp->show();
    this->hide();
}
