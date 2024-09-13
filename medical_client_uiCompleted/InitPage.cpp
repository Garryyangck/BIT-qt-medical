#include "InitPage.h"
#include <QDebug>

InitPage::InitPage(QObject *parent) : QObject(parent) {

    // 连接服务器
    QString ip = QString("127.0.0.1");
    QString port = QString("8888");
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress(ip), port.toShort());
}

void InitPage::doLogin(const QString &username, const QString &password, const bool &identity) {
    this->identity=identity;

    int role = identity ? 1 : 0;
    User *user = new User(username, password, role);
    Message msg(user, new User, QString(), MessageType::LOGIN);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this, user](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器对登录的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::LOGIN_SUCCESS) {
            this->user = user;
//            // 创建服务类，注册
//            if (user->role == 0) {
//                DoctorProfilePage doctorProfilePage(socket, user);
//                newEngine->rootContext()->setContextProperty("doctorProfilePage", &doctorProfilePage);
//            } else if (user->role == 1) {
//                PatientProfilePage patientProfilePage(socket, user);
//                newEngine->rootContext()->setContextProperty("patientProfilePage", &patientProfilePage);
//            }
            emit loginSuccess();

        } else if (msg.messageType == MessageType::LOGIN_FAILED) {
            emit errorOccurred("用户名或密码错误");
        }
    });
}

void InitPage::doRegister(const QString &username, const QString &password, const bool &identity) {
    int role = identity ? 1 : 0;
    User *user = new User(username, password, role);
    Message msg(user, new User, QString(), MessageType::REGISTER);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器对注册的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::REGISTER_SUCCESS) {
            emit registerSuccess();
        } else if (msg.messageType == MessageType::REGISTER_FAILED) {
            emit errorOccurred("用户已存在或服务器异常");
        }
    });
}

bool InitPage::getIdentity() {
    return identity;
}

QString InitPage::getUsername() {
    return user->username;
}

QString InitPage::getReceiver(){
    return receiver;
}

// ------------------------------------------患者使用的api------------------------------------------

void InitPage::getSchedule(QString department, QString date, QString doctor) {
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
            emit returnSchedule(list);
        }
    });
}

void InitPage::getDoctorProfile(QString doctorName) {
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
            emit returnDoctorProfile(parts);
//            QString name = parts[0];
//            QString gender = parts[1];
//            QString age = parts[2];
//            QString phone_number = parts[3];
//            QString department = parts[4];
        }
    });
}

void InitPage::doAppointment(QString department, QString date, QString doctor) {
    QString content = department + "_" + date + "_" + doctor;
    Message msg(user, new User, content, MessageType::APPOINT);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器doAppointment的响应";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::APPOINT_SUC) {
            emit appointResult(true);
        } else if (msg.messageType == MessageType::APPOINT_FAI) {
            emit appointResult(false);
        }
    });
}

void InitPage::getAllCase() {
    Message requestMsg(this->user, new User, QString(), MessageType::PAT_SEE_CASE);
    socket->write(Message::messageToByteArray(requestMsg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getAllCase的响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::PAT_SEE_CASE_SUC) {
            QStringList cases = msg.content.split("|");
            emit returnAllCase(cases); // 每一个元素的格式见下面
//                QString patient = parts[0];
//                QString doctor = parts[1];
//                QString date = parts[2];
//                QString department = parts[3];
//                QString symp = parts[4]; // 症状
//                QString diag = parts[5]; // 医嘱
//                QString medic = parts[6]; // 处方和缴费
        }
    });
}

void InitPage::getPatientProfile() {
    Message msg(user, new User, user->username, MessageType::GET_PATIENT_PROFILE);
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
            emit returnPatientInfo(parts);
        }
    });
}

void InitPage::updatePatientProfile(QString gender, QString age, QString phoneNumber) {
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
            emit updatePatientInfoResult(true);
        }
    });
}

void InitPage::submitHealth() {
    // 还没写
}

// ------------------------------------------通用的api------------------------------------------

void InitPage::getChats() {
    Message msg(user, new User, QString(), MessageType::GET_RECEIVER);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器返回的可聊天的对象";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::RET_RECEIVER) {
            QStringList receiver_num = msg.content.split("|"); // 元素的格式：receiver_num
            emit returnChats(receiver_num);
        }
    });
}

void InitPage::beginChat(QString receiver) {
    Message msg(user, new User(receiver, QString(), user->role == 0 ? 1 : 0), QString(), MessageType::GET_HISTORY);
    socket->write(Message::messageToByteArray(msg));
    this->receiver = receiver;

    connect(socket, &QTcpSocket::readyRead, this, [this, receiver](){ // 这里不能断开连接，直到手动退出页面
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器给chat的响应";
        msg.print();

        if (msg.messageType == MessageType::RECEIVE_MESSAGE) { // 收到消息
            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
            QString content = msg.content;
            emit receiveNewMessage(content); // 展示对方发来的信息

        } else if (msg.messageType == MessageType::SEND_SUCCESS)  { // 发送成功，对方在线
            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
            QString content = msg.content;
            emit sendSuccess(content); // 展示自己发给对方的消息

        } else if (msg.messageType == MessageType::RECEIVER_OFFLINE) { // 对方不在线
            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
            QString content = msg.content;
            emit receiverOffline(content); // 展示自己发给对方的消息

        } else if (msg.messageType == MessageType::RET_HISTORY) { // 返回历史记录
            QStringList histories = msg.content.split("|"); // 每一个history样式: content + "_" + chatDate + "_" + isRead + "_" + sender
            emit returnHistories(histories);

        } else if (msg.messageType == MessageType::UPDATE_ISREAD_SUCCESS) { // 服务器修改已读成功
            // 退出的时候，调用某一个接口，发出一个UPDATE_ISREAD的请求，然后接收到此请求后，断开connect连接
            disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);
        }
    });
}

void InitPage::sendMessage(QString content, QString receiver) {
    User *rcver = new User(receiver, QString(), user->role == 0 ? 1 : 0);
    Message msg(user, rcver, content, MessageType::SEND_MESSAGE);
    socket->write(Message::messageToByteArray(msg));
}

void InitPage::quitChatRoom(QString receiver) {
    Message msg(user, new User(receiver, QString(), user->role == 0 ? 1 : 0), QString(), MessageType::UPDATE_ISREAD);
    socket->write(Message::messageToByteArray(msg));
}

// ------------------------------------------医生使用的api------------------------------------------

void InitPage::getDoctorCase() {
    Message requestMsg(this->user, new User, QString(), MessageType::SEE_ALL_SCHEDULE);
    socket->write(Message::messageToByteArray(requestMsg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getDoctorCase的响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::SEE_ALL_SCHEDULE_SUC) {
            QStringList doctorCase = msg.content.split("|");
            emit returnDoctorCase(doctorCase);
//                QStringList parts = patientstr.split("_");
//                QString pu = parts[0];
//                QString du = parts[1];
//                QString date = parts[2];
//                QString dep = parts[3];
//                QString symp = parts[4];
//                QString diag = parts[5];
//                QString medic = parts[6];
        }
    });
}

void InitPage::updateDoctorCase(QString patientName, QString doctorName, QString date, QString department, QString symptoms, QString diagnosis, QString medicine) {
    QString content = patientName + "_" + doctorName + "_" + date + "_" + department +
                      "_" + symptoms + "_" + diagnosis + "_" + medicine ;
    Message msg(this->user, new User, content, MessageType::DOC_UPDATE_CASE);

    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器updateDoctorCase的响应：";
        msg.print();

        // disconnect，只要是涉及到按钮，都在connect里面disconnect
        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::DOC_UPDATE_CASE_SUC) {
            updateDoctorCaseResult(true);
        }
        else if(msg.messageType == MessageType::DOC_UPDATE_CASE_FAI) {
            updateDoctorCaseResult(false);
        }
    });
}

void InitPage::getDoctorProfile() {
    Message msg(user, new User, user->username, MessageType::GET_DOCTOR_PROFILE);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器getDoctorProfile响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::RET_DOCTOR_PROFILE)
        {
            QStringList parts = msg.content.split("_");
            emit returnDoctorInfo(parts);
//            QString name = parts[0];
//            QString gender = parts[1];
//            QString age = parts[2];
//            QString phone_number = parts[3];
//            QString department = parts[4];
        }
    });
}

void InitPage::updateDoctorProfile(QString gender, QString age, QString phoneNumber, QString department) {
    QString content = this->user->username + "_" + gender + "_" + age + "_" + phoneNumber + "_" + department;
    Message msg(this->user, new User, content, MessageType::CHANGE_INFO_D);

    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器updateDoctorProfile响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::CHANGE_INFO_D_SUCCESS) {
            emit updateDoctorInfoResult(true);
        }
        else if (msg.messageType == MessageType::CHANGE_INFO_D_FAILED) {
            emit updateDoctorInfoResult(false);
        }
    });
}
