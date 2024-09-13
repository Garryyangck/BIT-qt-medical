#include "PatientProfilePage.h"

PatientProfilePage::PatientProfilePage(QTcpSocket *socket, User *user, QObject *parent) : QObject(parent)
{
    this->socket = socket;
    this->user = user;
}

void PatientProfilePage::getSchedule(QString department, QString date, QString doctor) {
    QString content = department + "_" + doctor + "_" + date;
    Message msg(user, new User, content, MessageType::GET_SCHEDULE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getSchedule的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::RET_SCHEDULE) {
            QStringList list = msg.content.split("|");
            for (const QString &line : list) {
                QStringList parts = line.split("_");
                QString doctor_name = parts[0];
                QString department = parts[1];
                QString schedule_date = parts[2];
                QString available_num = parts[3];

                // 前端展示每一条schedule的逻辑
                // emit ...
            }
        }
    });
}

void PatientProfilePage::getDoctorProfile(QString doctorName) {
    Message msg(user, new User, doctorName, MessageType::GET_DOCTOR_PROFILE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getDoctorProfile的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::RET_DOCTOR_PROFILE) {
            QStringList parts = msg.content.split("_");
            QString name = parts[0];
            QString gender = parts[1];
            QString age = parts[2];
            QString phone_number = parts[3];
            QString department = parts[4];

            // 前端显示医生详情页面的逻辑
            // emit ...
        }
    });
}

void PatientProfilePage::doAppointment(QString department, QString date, QString doctor) {
    QString content = department + "_" + doctor + "_" + date;
    Message msg(user, new User, content, MessageType::APPOINT);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器doAppointment的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::APPOINT_SUC) {
            // 挂号成功的逻辑
            // emit ...
        } else if (msg.messageType == MessageType::APPOINT_FAI) {
            // 挂号失败的逻辑
            // emit ...
        }
    });
}

void PatientProfilePage::getAllCase() {
    Message requestMsg(this->user, new User, QString(), MessageType::PAT_SEE_CASE);
    socket->write(Message::messageToByteArray(requestMsg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getAllCase的响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::PAT_SEE_CASE_SUC) {
            QStringList patientsee = msg.content.split("|");
            for (const QString &seestr : patientsee) {
                QStringList parts = seestr.split("_");
                QString patient = parts[0];
                QString doctor = parts[1];
                QString date = parts[2];
                QString department = parts[3];
                QString symp = parts[4]; // 症状
                QString diag = parts[5]; // 医嘱
                QString medic = parts[6]; // 处方和缴费

                // 显示患者所有病例的逻辑
                // emit ...
            }
        }
    });
}

void PatientProfilePage::getPatientProfile() {
    Message msg(user, new User, QString(), MessageType::GET_PATIENT_PROFILE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getPatientProfile的响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::RET_PATIENT_PROFILE)
        {
            QStringList parts = msg.content.split("_");
            QString name = parts[0];
            QString gender = parts[1];
            QString age = parts[2];
            QString phone_number = parts[3];

            // 前端在编辑个人信息页面，首先显示自己的详情的逻辑
            // emit ...
        }
    });
}

void PatientProfilePage::updatePatientProfile(QString gender, QString age, QString phoneNumber) {
    QString content = this->user->username + "_" + gender + "_" + age + "_" + phoneNumber;
    Message msg(this->user, new User, content, MessageType::CHANGE_INFO_P);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器updatePatientProfile的响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::CHANGE_INFO_P_SUCCESS) {
            // 个人信息修改成功的逻辑
            // emit ...
        } else if (msg.messageType == MessageType::CHANGE_INFO_P_FAILED) {
            // 个人信息修改失败的逻辑
            // emit ...
        }
    });
}

void PatientProfilePage::submitHealth() {
    // 还没写
}


