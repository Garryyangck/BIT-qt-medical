#include "mythread.h"

MyThread::MyThread(QTcpSocket *socket, User *user, QThread *parent) : QThread(parent)
{
    this->socket = socket;
    this->user = user;
}

void MyThread::run() {
    qDebug() << "子线程开始run";
    connect(socket, &QTcpSocket::readyRead, this, &MyThread::clientManageSlot);
}

void MyThread::clientManageSlot() {
    qDebug() << "clientManageSlot";
    QByteArray ba = socket->readAll();
    Message msg = Message::byteArrayToMessage(ba);

    qDebug() << "线程处理的请求";
    msg.print();

    if (msg.messageType == MessageType::GET_RECEIVER) { // 查看可聊天用户
        // 真实情况下使用下面这条
//        qDebug() << "username: " << msg.sender->username;
//        qDebug() << "role: " << msg.sender->role;
        QStringList receivers = DBUtils::getReceivers(msg.sender->username, msg.sender->role);

        // 构建content
        QHash<QString, int> receiverTable;
        for(const QString &receiver : receivers) {
            QString content;
            content.append(receiver);
            content.append(QString("_"));
            content.append(QString::number(DBUtils::getUnreadNum(msg.sender->username, receiver)));
            if (!receiverTable.contains(content)) {
                receiverTable.insert(content, 0);
            }
        }
        QStringList res; // name_num|name_num|name_num
        QStringList keys = receiverTable.keys();
        for(const QString &receiver : keys) {
            res.append(receiver);
        }

        // 构建Message
        Message returnMsg(new User, new User, res.join("|"), MessageType::RET_RECEIVER);
        socket->write(Message::messageToByteArray(returnMsg));

    } else if (msg.messageType == MessageType::SEND_MESSAGE) { // 即时通讯的信息，只需转发给主线程即可
        emit sendToMainThread(msg);

    } else if (msg.messageType == MessageType::QUIT) { // 用户退出的信息，只需转发给主线程即可
        emit sendToMainThread(msg);

    } else if (msg.messageType == MessageType::GET_HISTORY) { // 历史记录
        QStringList historys = DBUtils::getChatMessage(msg.sender->username, msg.receiver->username);
        QString content;

        // 不同的行，以“|”分隔开
        if (historys.size() > 0) {
            for(const QString &history : historys) {
                content.append(history).append("|");
            }
            content.chop(1);
        }

        // 构造Message
        Message msg(new User, new User, content, MessageType::RET_HISTORY);
        socket->write(Message::messageToByteArray(msg));

    } else if (msg.messageType == MessageType::UPDATE_ISREAD) { // 客户端退出即时通讯系统，此时需要更新为已读
        DBUtils::updateIsRead(msg.sender->username, msg.receiver->username);
        socket->write(Message::messageToByteArray(Message::serverReplyMessage(MessageType::UPDATE_ISREAD_SUCCESS)));
    }

    else if (msg.messageType == MessageType::CHANGE_INFO_D){ // 修改医生个人信息
        // 获取gender, age, phoneNumber
        QString content = msg.content;
        QStringList parts = content.split("_");
        QString name = parts[0];
        QString gender = parts[1];
        QString age = parts[2];
        QString phoneNumber = parts[3];
        QString department = parts[4];
        if(DBUtils::updateDoctorProfile(name, gender, age, phoneNumber, department))
        {
            Message returnMsg = Message::serverReplyMessage(MessageType::CHANGE_INFO_D_SUCCESS);
            socket->write(Message::messageToByteArray(returnMsg));
        }else{
            Message returnMsg = Message::serverReplyMessage(MessageType::CHANGE_INFO_D_FAILED);
            socket->write(Message::messageToByteArray(returnMsg));
        }
    }

    else if (msg.messageType == MessageType::CHANGE_INFO_P){ // 修改病人个人信息
        // 获取gender, age, phoneNumber
        QString content = msg.content;
        QStringList parts = content.split("_");
        QString name = parts[0];
        QString gender = parts[1];
        QString age = parts[2];
        QString phoneNumber = parts[3];
        if(DBUtils::updatePatientProfile(name, gender, age, phoneNumber))
        {
            Message returnMsg = Message::serverReplyMessage(MessageType::CHANGE_INFO_P_SUCCESS);
            socket->write(Message::messageToByteArray(returnMsg));
        }else{
            Message returnMsg = Message::serverReplyMessage(MessageType::CHANGE_INFO_P_FAILED);
            socket->write(Message::messageToByteArray(returnMsg));
        }
    }

    else if (msg.messageType == MessageType::SEE_DOCTOR_INFO) // 病人查看自己的医生
    {
        QStringList qslt = DBUtils::getPatient_Doctor(msg.sender->username);
        QString doctor_uname = qslt.join("|");
        Message returnmsg(msg.sender, new User, doctor_uname, MessageType::SEE_D_SUCCESS);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::SEE_ALL_SCHEDULE)
    { // 查看医生的所有挂号信息
        QStringList qslt = DBUtils::getAppointmentCase(msg.sender->username, 0);
        QString doctor_uname = qslt.join("|");
        Message returnmsg(msg.sender, new User, doctor_uname, MessageType::SEE_ALL_SCHEDULE_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::DOC_SEE_CASE)
    { // 医生查看自己还没有诊断的病历
        QStringList qslt = DBUtils::getAppointmentCase(msg.sender->username, 0);
        QStringList undoList;
        for (const QString& case_ : qslt) {
            int isDialogue = case_.split("_")[7].toInt();
            if (isDialogue == 0) {
                undoList.append(case_);
            }
        }
        QString doctorsee = undoList.join("|");
        Message returnmsg(msg.sender, new User, doctorsee, MessageType::DOC_SEE_CASE_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::DOC_UPDATE_CASE){ // 医生修改病历
        // 获取信息
        QString content = msg.content;
        QStringList parts = content.split("_");
        QString pu = parts[0];
        QString du = parts[1];
        QString date = parts[2];
        QString dep = parts[3];
        QString symp = parts[4];
        QString diag = parts[5];
        QString medic = parts[6];
        if (DBUtils::updateAppointmentCase(pu, du, date, dep, symp, diag, medic))
        {
            Message returnMsg = Message::serverReplyMessage(MessageType::DOC_UPDATE_CASE_SUC);
            socket->write(Message::messageToByteArray(returnMsg));
        } else {
            Message returnMsg = Message::serverReplyMessage(MessageType::DOC_UPDATE_CASE_FAI);
            socket->write(Message::messageToByteArray(returnMsg));
        }
    }

    else if (msg.messageType == MessageType::PAT_SEE_CASE)
    { // 患者查看自己的病历
        QStringList qslt = DBUtils::getAppointmentCase(msg.sender->username, 1);
        QString doctorsee = qslt.join("|");
        Message returnmsg(msg.sender, new User, doctorsee, MessageType::PAT_SEE_CASE_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::SCHDULE_SEARCH)
    { // 按时间查看医生排班
        QHash<QString, QStringList> qh = DBUtils::getScheduleDate();
        // 将QHash压缩为JSON字符串
        Message returnmsg(msg.sender, new User, Utils::compressQHashToJson(qh), MessageType::SCHDULE_SEARCH_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::DEPART_SEARCH)
    { // 按科室查看医生排班
        QHash<QString, QStringList> qh = DBUtils::getScheduleDepartment();
        // 将QHash压缩为JSON字符串
        Message returnmsg(msg.sender, new User, Utils::compressQHashToJson(qh), MessageType::DEPART_SEARCH_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if (msg.messageType == MessageType::GET_DOCTOR_PROFILE) { // 获取医生详情
        QString doctorProfile = DBUtils::getDoctorProfile(msg.content);
        Message returnMsg(new User, new User, doctorProfile, MessageType::RET_DOCTOR_PROFILE);
        socket->write(Message::messageToByteArray(returnMsg));
    }

    else if (msg.messageType == MessageType::GET_PATIENT_PROFILE) { // 获取患者详情
        QString doctorProfile = DBUtils::getPatientProfile(msg.content);
        Message returnMsg(new User, new User, doctorProfile, MessageType::RET_PATIENT_PROFILE);
        socket->write(Message::messageToByteArray(returnMsg));
    }

    else if(msg.messageType == MessageType::GET_ALL_DEPART)
    { // 查看所有科室
        QStringList qslt = DBUtils::getAllDepartments();
        QString departs = qslt.join("|");
        Message returnmsg(msg.sender, new User, departs, MessageType::GET_ALL_DEPART_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if(msg.messageType == MessageType::GET_DATE_BY_DEP)
    { // 在科室中查找所有的时间
        QStringList qslt = DBUtils::getAllDates(msg.content);
        QString departs = qslt.join("|");
        Message returnmsg(msg.sender, new User, departs, MessageType::GET_DATE_BY_DEP_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if(msg.messageType == MessageType::GET_SCHEDULE_BY_DEP_DATE)
    { // 在科室和时间下查找所有schedule
        QString content = msg.content;
        QStringList parts = content.split("_");
        QString date = parts[0];
        QString dep = parts[1];
        QStringList res = DBUtils::getDoctorFromDate(date, dep);
        Message returnmsg(msg.sender, new User, res.join("|"), MessageType::GET_SCHEDULE_BY_DEP_DATE_SUC);
        socket->write(Message::messageToByteArray(returnmsg));
    }

    else if(msg.messageType == MessageType::APPOINT)
    { // 挂号
        QString content = msg.content;
        QStringList parts = content.split("_");
        QString dep = parts[0];
        QString date = parts[1];
        QString docname = parts[2];

        if (DBUtils::getAvailableNum(docname, dep, date) > 0 &&
                DBUtils::insertAppointmentCase(this->user->username, docname, date, dep, Utils::blankMsg, Utils::blankMsg, Utils::blankMsg, 0) &&
                DBUtils::updateScheduleDecreaseNum(docname, dep, date)) {
            Message returnMsg = Message::serverReplyMessage(MessageType::APPOINT_SUC);
            socket->write(Message::messageToByteArray(returnMsg));
        } else {
            Message returnMsg = Message::serverReplyMessage(MessageType::APPOINT_FAI);
            socket->write(Message::messageToByteArray(returnMsg));
        }
    }

    else if(msg.messageType == MessageType::GET_SCHEDULE) { // 前端需要的接口，查询schedule
        QStringList parts = msg.content.split("_");
        QString department = parts[0];
        QString doctor = parts[1];
        QString date = parts[2];
        QString content;
        QStringList list;
        if (doctor == DBUtils::ALL_DOCTOR && date == DBUtils::ALL_DATE) {
            list = DBUtils::getSchedule(department);
        } else if (doctor == DBUtils::ALL_DOCTOR) {
            list = DBUtils::getScheduleByDate(department, date);
        } else if (date == DBUtils::ALL_DATE) {
            list = DBUtils::getScheduleByDoctor(department, doctor);
        } else {
            list = DBUtils::getScheduleByDateDoctor(department, date, doctor);
        }
        content = list.join("|");
        Message msg(new User, new User, content, MessageType::RET_SCHEDULE);
        socket->write(Message::messageToByteArray(msg));
    }
}
