#include "dbutils.h"

QString DBUtils::DBName = QString("medical.db");

QString DBUtils::DATE_FORMAT = QString("yyyy-MM-dd HH:mm:ss");

QString DBUtils::DATE_FORMAT_ONLY_DATE = QString("yyyy-MM-dd");

QString DBUtils::ALL_DATE = QString("全部日期");

QString DBUtils::ALL_DOCTOR = QString("全部医生");

DBUtils::DBUtils(QObject *parent) : QObject(parent)
{

}

bool DBUtils::connect(const QString &dbName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QMessageBox::critical(0, QString("connect异常"), db.lastError().text());
        return false;
    }
    return true;
}

bool DBUtils::login(QString username, QString password, int role) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select password from user where username = :username and role = :role");
        query.prepare(sql);
        query.bindValue(":username", username);
        query.bindValue(":role", role);
        if (query.exec()) { // 登录成功
            while(query.next()) {
                QString pwd = query.value(0).toString();
                if (pwd == password) {
                    return true;
                }
            }
        } else {
            QMessageBox::critical(0, QString("login异常"), query.lastError().text());
            return false;
        }
    }
    return false;
}

bool DBUtils::signUp(QString username, QString password, int role, QString gender, QString age, QString phoneNumber) {
    if (connect(DBUtils::DBName)) {
        // 先往user表中插入
        QSqlQuery query;
        QString sql = QString("insert into user(username, password, role) values(:username, :password, :role)");
        query.prepare(sql);
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":role", role);
        if (!query.exec()) {
            QMessageBox::critical(0, QString("signUp异常"), query.lastError().text());
            return false;
        }

        // 查出刚插入的userId
        int userId = -1;
        sql = QString("select id from user where username = :username and password = :password and role = :role");
        query.prepare(sql);
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":role", role);
        if (!query.exec()) {
            QMessageBox::critical(0, QString("signUp异常"), query.lastError().text());
            return false;
        } else {
            while(query.next()) {
                userId = query.value(0).toInt();
            }
        }
        if (userId < 0) {
            QMessageBox::critical(0, QString("signUp异常"), query.lastError().text());
            return false;
        }

        // 插入 patient 或 doctor
        if (role == 0) { // 医生
            sql = QString("insert into doctor(user_id, name, gender, age, phone_number) "
                          "values(:user_id, :name, :gender, :age, :phone_number)");
            query.prepare(sql);
            query.bindValue(":user_id", userId);
            query.bindValue(":name", username);
            query.bindValue(":gender", gender);
            query.bindValue(":age", age);
            query.bindValue(":phone_number", phoneNumber);
            if (!query.exec()) {
                QMessageBox::critical(0, QString("signUp异常"), query.lastError().text());
                return false;
            }
        } else if (role == 1) { // 患者
            sql = QString("insert into patient(user_id, name, gender, age, phone_number) "
                          "values(:user_id, :name, :gender, :age, :phone_number)");
            query.prepare(sql);
            query.bindValue(":user_id", userId);
            query.bindValue(":name", username);
            query.bindValue(":gender", gender);
            query.bindValue(":age", age);
            query.bindValue(":phone_number", phoneNumber);
            if (!query.exec()) {
                QMessageBox::critical(0, QString("signUp异常"), query.lastError().text());
                return false;
            }
        }

        return true;
    }
    return false;
}

QStringList DBUtils::getReceivers(QString username, int role) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        if (role == 0) { // 你是医生，查患者
            QString sql = QString("select patient_name from appointment_case where doctor_name = :doctor_name");
            query.prepare(sql);
            query.bindValue(":doctor_name", username);
            if (query.exec()) {
                QStringList receivers;
                while(query.next()) {
                    receivers.append(query.value(0).toString());
                }
                return receivers;
            } else {
                QMessageBox::critical(0, QString("getReceivers异常"), query.lastError().text());
                return QStringList();
            }
        } else if (role == 1) { // 你是患者，查医生
            QString sql = QString("select doctor_name from appointment_case where patient_name = :patient_name");
            query.prepare(sql);
            query.bindValue(":patient_name", username);
            if (query.exec()) {
                QStringList receivers;
                while(query.next()) {
                    receivers.append(query.value(0).toString());
                }
                return receivers;
            } else {
                QMessageBox::critical(0, QString("getReceivers异常"), query.lastError().text());
                return QStringList();
            }
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllPatients() {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select name from patient");
        query.prepare(sql);
        if (query.exec()) {
            QStringList patients;
            while(query.next()) {
                patients.append(query.value(0).toString());
            }
            return patients;
        } else {
            QMessageBox::critical(0, QString("getAllPatients异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllDoctors() {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select name from doctor");
        query.prepare(sql);
        if (query.exec()) {
            QStringList doctors;
            while(query.next()) {
                doctors.append(query.value(0).toString());
            }
            return doctors;
        } else {
            QMessageBox::critical(0, QString("getAllDoctors异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

int DBUtils::getUnreadNum(QString receiverName, QString senderName) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select count(*) from chat_message where receiver_name = :receiver_name and sender_name = :sender_name and isRead = 0");
        query.prepare(sql);
        query.bindValue(":receiver_name", receiverName);
        query.bindValue(":sender_name", senderName);
        if (query.exec()) {
            int num = 0;
            while(query.next()) {
                num = query.value(0).toInt();
                return num;
            }
        } else {
            QMessageBox::critical(0, QString("getUnreadNum异常"), query.lastError().text());
        }
    }
    return -1;
}

QStringList DBUtils::getChatMessage(QString username, QString chatter) {
    if (connect(DBUtils::DBName)) {
        QStringList res;
        QSqlQuery query;
        // 对方发给我的消息
        QString sql = QString("select content, chat_date, isRead, sender_name from chat_message where receiver_name = :receiver_name and sender_name = :sender_name");
        query.prepare(sql);
        query.bindValue(":receiver_name", username);
        query.bindValue(":sender_name", chatter);
        if (query.exec()) {
            while(query.next()) {
                QString content = query.value(0).toString();
                QString chat_date = query.value(1).toString();
                QString isRead = query.value(2).toString();
                QString sender = query.value(3).toString();
                QString line = content + QString("_") + chat_date + QString("_") + isRead + QString("_") + sender;
                res.append(line);
            }
        } else {
            QMessageBox::critical(0, QString("getChatMessage异常"), query.lastError().text());
        }
        // 我发给对方的消息
        query.prepare(sql);
        query.bindValue(":receiver_name", chatter);
        query.bindValue(":sender_name", username);
        if (query.exec()) {
            while(query.next()) {
                QString content = query.value(0).toString();
                QString chat_date = query.value(1).toString();
                QString isRead = query.value(2).toString();
                QString sender = query.value(3).toString();
                QString line = content + QString("_") + chat_date + QString("_") + isRead + QString("_") + sender;
                res.append(line);
            }
        } else {
            QMessageBox::critical(0, QString("getChatMessage异常"), query.lastError().text());
        }
        // 按照时间顺序排序
        qSort(res.begin(), res.end(), [](QString s1, QString s2) -> int {
            QDateTime t1 = QDateTime::fromString(s1.split(QString("_"))[1], DATE_FORMAT);
            QDateTime t2 = QDateTime::fromString(s2.split(QString("_"))[1], DATE_FORMAT);
            return t1 < t2;
        });
        return res;
    }
    return QStringList();
}

bool DBUtils::insertChatMessage(QString sender, QString receiver, QDateTime chatDate, QString content, int isRead) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("insert into chat_message(sender_name, receiver_name, chat_date, content, isRead)"
                              "values(:sender_name, :receiver_name, :chat_date, :content, :isRead)");
        query.prepare(sql);
        query.bindValue(":sender_name", sender);
        query.bindValue(":receiver_name", receiver);
        query.bindValue(":chat_date", chatDate.toString(DATE_FORMAT));
        query.bindValue(":content", content);
        query.bindValue(":isRead", isRead);
        if (!query.exec()) {
            QMessageBox::critical(0, QString("insertChatMessage异常"), query.lastError().text());
            return false;
        }
    }
    return true;
}

bool DBUtils::updateIsRead(QString username, QString chatter) {
    if (connect(DBUtils::DBName)) {
        QStringList res;
        QSqlQuery query;
        // 修改对方发给我的消息
        QString sql = QString("update chat_message set isRead = 1 where receiver_name = :receiver_name and sender_name = :sender_name");
        query.prepare(sql);
        query.bindValue(":receiver_name", username);
        query.bindValue(":sender_name", chatter);
        if (!query.exec()) {
            QMessageBox::critical(0, QString("updateIsRead异常"), query.lastError().text());
        }
        return true;
    }
    return false;
}

//按日期查看医生
QHash<QString,QStringList> DBUtils::getScheduleDate(){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select doctor_name, department, schedule_date, available_num from schedule order by schedule_date ASC");
        query.prepare(sql);
        if(query.exec()){
            QHash<QString,QStringList> scheduleDate_information;
            while(query.next()){
                //把每天的每个医生信息连接成字符串
                QString information=query.value(0).toString()+"_"+
                                     query.value(1).toString()+"_"+
                                     query.value(2).toString()+"_"+
                                     query.value(3).toString();
                //把每天的日期变成字符串
                QString scheduleDate=query.value(2).toString();
                //如果日期已经存在哈希表中，追加信息到该日对应的列表上
                if(scheduleDate_information.contains(scheduleDate)){
                    scheduleDate_information[scheduleDate].append(information);
                }
                //否则创建新的时间列表QStringList再添加信息
                else{
                    QStringList schedule;
                    schedule.append(information);
                    scheduleDate_information[scheduleDate]=schedule;
                }
            }
            return scheduleDate_information;
        }else{
            QMessageBox::critical(0,QString("getScheduleDate异常"),query.lastError().text());
            return QHash<QString,QStringList>();
        }
    }
    return QHash<QString,QStringList>();
}

//按科室查看医生
QHash<QString,QStringList> DBUtils::getScheduleDepartment(){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select doctor_name, department, schedule_date, available_num "
                            "from schedule "
                            "order by department");
        query.prepare(sql);
        if(query.exec()){
            QHash<QString,QStringList> scheduleDepartment_information;
            while(query.next()){
                //把每个科室每个医生的信息连接成字符串
                QString information=query.value(0).toString()+"_"+
                                     query.value(1).toString()+"_"+
                                     query.value(2).toString()+"_"+
                                     query.value(3).toString();
                //把每个科室转化为字符串形式
                QString scheduleDepartment=query.value(1).toString();
                //如果科室已经存在哈希表中，追加信息到该科室对应列表上
                if(scheduleDepartment_information.contains(scheduleDepartment)){
                    scheduleDepartment_information[scheduleDepartment].append(information);
                }
                //否则创建新的部门列表
                else{
                    QStringList department;
                    department.append(information);
                    scheduleDepartment_information[scheduleDepartment]=department;
                }
            }
            return scheduleDepartment_information;
        }else{
            QMessageBox::critical(0,QString("getScheduleDepartment异常"),query.lastError().text());
            return QHash<QString,QStringList>();
        }
    }
    return QHash<QString,QStringList>();
}

bool DBUtils::updateScheduleDecreaseNum(QString doctorName, QString department, QString date) {
    if (connect(DBUtils::DBName)) {
        QStringList res;
        QSqlQuery query;
        // 减少剩余的号数量
        QString sql = QString("update schedule set available_num = available_num - 1 where doctor_name = :doctor_name and department = :department and schedule_date = :schedule_date");
        query.prepare(sql);
        query.bindValue(":doctor_name", doctorName);
        query.bindValue(":department", department);
        query.bindValue(":schedule_date", date);
        if (!query.exec()) {
            QMessageBox::critical(0, QString("updateScheduleDecreaseNum异常"), query.lastError().text());
        }
        return true;
    }
    return false;
}

QStringList DBUtils::getAllDepartments()
{ // 获得所有科室
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select department from schedule");
        query.prepare(sql);
        if (query.exec()) {
            QStringList doctors;
            while(query.next()) {
                doctors.append(query.value(0).toString());
            }
            return doctors;
        } else {
            QMessageBox::critical(0, QString("getAllDepartments异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getDoctorFromDate(QString date, QString dep)
{ // 根据时间检索医生信息
    if(connect(DBUtils::DBName)){
//        QDateTime datt = QDateTime::fromString(dat, DATE_FORMAT_ONLY_DATE);
//        qDebug()<<"Date:"<<dat;
        QSqlQuery query;
        QString sql=QString("select doctor_name, department, schedule_date, available_num from schedule where department = :depa and schedule_date = :date order by schedule_date ASC");
        query.prepare(sql);
        query.bindValue(":depa", dep);
        query.bindValue(":date", date);
        if (query.exec()) {
            QStringList res;
            while(query.next()) {
                QString doctor_name = query.value(0).toString();
                QString department = query.value(1).toString();
                QString schedule_date = query.value(2).toString();
                QString available_num = query.value(3).toString();
                QString line = doctor_name + "_" + department + "_" + schedule_date + "_" + available_num;
                res.append(line);
            }
            return res;
        } else {
            QMessageBox::critical(0,QString("getDoctorFromSchedule异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getScheduleByDateDoctor(QString department, QString date, QString doctor) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select doctor_name, department, schedule_date, available_num from schedule where doctor_name = :doctor_name and department = :department and schedule_date = :schedule_date");
        query.prepare(sql);
        query.bindValue(":doctor_name", doctor);
        query.bindValue(":department", department);
        query.bindValue(":schedule_date", date);
        if (query.exec()) {
            QStringList res;
            while(query.next()) {
                QString doctor_name = query.value(0).toString();
                QString department = query.value(1).toString();
                QString schedule_date = query.value(2).toString();
                QString available_num = query.value(3).toString();
                QString line = doctor_name + "_" + department + "_" + schedule_date + "_" + available_num;
                res.append(line);
            }
            return res;
        } else {
            QMessageBox::critical(0, QString("getScheduleByDateDoctor异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getScheduleByDate(QString department, QString date) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select doctor_name, department, schedule_date, available_num from schedule where department = :department and schedule_date = :schedule_date");
        query.prepare(sql);
        query.bindValue(":department", department);
        query.bindValue(":schedule_date", date);
        if (query.exec()) {
            QStringList res;
            while(query.next()) {
                QString doctor_name = query.value(0).toString();
                QString department = query.value(1).toString();
                QString schedule_date = query.value(2).toString();
                QString available_num = query.value(3).toString();
                QString line = doctor_name + "_" + department + "_" + schedule_date + "_" + available_num;
                res.append(line);
            }
            return res;
        } else {
            QMessageBox::critical(0, QString("getScheduleByDateDoctor异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getScheduleByDoctor(QString department, QString doctor) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select doctor_name, department, schedule_date, available_num from schedule where doctor_name = :doctor_name and department = :department");
        query.prepare(sql);
        query.bindValue(":doctor_name", doctor);
        query.bindValue(":department", department);
        if (query.exec()) {
            QStringList res;
            while(query.next()) {
                QString doctor_name = query.value(0).toString();
                QString department = query.value(1).toString();
                QString schedule_date = query.value(2).toString();
                QString available_num = query.value(3).toString();
                QString line = doctor_name + "_" + department + "_" + schedule_date + "_" + available_num;
                res.append(line);
            }
            return res;
        } else {
            QMessageBox::critical(0, QString("getScheduleByDateDoctor异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getSchedule(QString department) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select doctor_name, department, schedule_date, available_num from schedule where department = :department");
        query.prepare(sql);
        query.bindValue(":department", department);
        if (query.exec()) {
            QStringList res;
            while(query.next()) {
                QString doctor_name = query.value(0).toString();
                QString department = query.value(1).toString();
                QString schedule_date = query.value(2).toString();
                QString available_num = query.value(3).toString();
                QString line = doctor_name + "_" + department + "_" + schedule_date + "_" + available_num;
                res.append(line);
            }
            return res;
        } else {
            QMessageBox::critical(0, QString("getScheduleByDateDoctor异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllDates(QString dep)
{ // 根据科室，获取所有可行的日期
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select schedule_date from schedule where department = :depa");
        query.prepare(sql);
        query.bindValue(":depa", dep);
        if (query.exec()) {
            QStringList scheduleList;
            while(query.next()) {
                QDateTime dateTime = query.value(0).toDateTime();  // 获取 QDateTime 类型数据
                QString dateStr = dateTime.toString(DBUtils::DATE_FORMAT_ONLY_DATE);
                scheduleList.append(dateStr);
            }
            return scheduleList;
        } else {
            QMessageBox::critical(0, QString("getAllSchedules异常"), query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

int DBUtils::getAvailableNum(QString doctorName, QString department, QString date) {
    if (connect(DBUtils::DBName)) {
        QSqlQuery query;
        QString sql = QString("select available_num from schedule where doctor_name = :doctor_name and department = :department and schedule_date = :schedule_date");
        query.prepare(sql);
        query.bindValue(":doctor_name", doctorName);
        query.bindValue(":department", department);
        query.bindValue(":schedule_date", date);
        if (query.exec()) {
            int res = 0;
            while(query.next()) {
                res = query.value(0).toInt();
                return res;
            }
            return res;
        } else {
            QMessageBox::critical(0, QString("getAllSchedules异常"), query.lastError().text());
            return -1;
        }
    }
    return -1;
}

//用账号获取就诊表
QStringList DBUtils::getAppointmentCase(QString username,int role){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql;
        switch (role) {
            case 0:
                sql=QString("select * "
                            "from appointment_case "
                            "where doctor_name = :username");
                break;
            case 1:
                sql=QString("select * "
                            "from appointment_case "
                            "where patient_name = :username");
                break;
            default:
                QMessageBox::critical(0,QString("getAppointmentCase异常"),query.lastError().text());
                return QStringList();
                break;
        }
        query.prepare(sql);
        query.bindValue(":username",username);
        if(query.exec()){
            QStringList appointmentCase;
            while(query.next()){
                QString information=query.value("patient_name").toString()+"_"+
                                    query.value("doctor_name").toString()+"_"+
                                    query.value("appointment_date").toDate().toString("yyyy-MM-dd")+"_"+
                                    query.value("department").toString()+"_"+
                                    query.value("symptoms").toString()+"_"+
                                    query.value("diagnosis").toString()+"_"+
                                    query.value("medicine").toString()+"_"+
                                    query.value("isDialogue").toString(); // 多了一个字段，表示医生是否诊断该病例
                appointmentCase.append(information);
            }
            return appointmentCase;
        }else{
            QMessageBox::critical(0,QString("getAppointmentCase异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

//判断是否成功更新
bool DBUtils::updateAppointmentCase(QString patientName, QString doctorName, QString appointmentDate, QString department, QString symptoms, QString diagnosis, QString medicine){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("update appointment_case "
                            "set symptoms = :symptoms, "
                            "diagnosis = :diagnosis, "
                            "medicine = :medicine, "
                            "isDialogue = 1 "
                            "where patient_name = :patient_name "
                            "and doctor_name = :doctor_name "
                            "and appointment_date = :appointment_date "
                            "and department = :department "); // 该接口只有医生能调，因此可以认为只要调用该接口，就完成了诊断，因此这里直接把isDialogue改为1

        query.prepare(sql);
        query.bindValue(":symptoms",symptoms);
        query.bindValue(":diagnosis",diagnosis);
        query.bindValue(":medicine",medicine);
        query.bindValue(":patient_name",patientName);
        query.bindValue(":doctor_name",doctorName);
        query.bindValue(":appointment_date",appointmentDate);
        query.bindValue(":department",department);
        if(query.exec()){
            if(query.numRowsAffected()>0){
                return true;
            }
        }
        else{
            QMessageBox::critical(0,QString("update异常"),query.lastError().text());
            return false;
        }
    }
    return false;
}

//判断是否成功插入病历表
bool DBUtils::insertAppointmentCase(QString patientName, QString doctorName, QString appointmentDate, QString department, QString symptoms, QString diagnosis, QString medicine, int isDialogue){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("insert into appointment_case "
                            "(patient_name,doctor_name,appointment_date,department,symptoms,diagnosis,medicine,isDialogue) "
                            "values(:patient_name,:doctor_name,:appointment_date,:department,:symptoms,:diagnosis,:medicine,:isDialogue)");
        query.prepare(sql);
        query.bindValue(":patient_name",patientName);
        query.bindValue(":doctor_name",doctorName);
        query.bindValue(":appointment_date",appointmentDate);
        query.bindValue(":department",department);
        query.bindValue(":symptoms",symptoms);
        query.bindValue(":diagnosis",diagnosis);
        query.bindValue(":medicine",medicine);
        query.bindValue(":isDialogue",isDialogue);
        if(query.exec()){
            return true;
        }
        else{
            QMessageBox::critical(0,QString("insertAppointmentCase异常"),query.lastError().text());
            return false;
        }
    }
    return false;
}

//获取医生信息
QString DBUtils::getDoctorProfile(QString username){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select name, gender, age, phone_number, department "
                            "from doctor "
                            "where name = :doctor_name");
        query.prepare(sql);
        query.bindValue(":doctor_name",username);
        if(query.exec()){
            while(query.next()){
                QString name=query.value("name").toString();
                if(name==username){
                    QString information=query.value("name").toString()+"_"+
                                        query.value("gender").toString()+"_"+
                                        query.value("age").toString()+"_"+
                                        query.value("phone_number").toString()+"_"+
                                        query.value("department").toString();
                    return information;
                }
            }
        }
        else{
            QMessageBox::critical(0,QString("getDoctorProfile异常"),query.lastError().text());
            return QString();
        }
    }
    return QString();
}

//获取病人信息
QString DBUtils::getPatientProfile(QString username){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select name, gender, age, phone_number "
                            "from patient "
                            "where name = :patient_name");
        query.prepare(sql);
        query.bindValue(":patient_name",username);
        if(query.exec()){
            while(query.next()){
                QString information=query.value("name").toString()+"_"+
                                        query.value("gender").toString()+"_"+
                                        query.value("age").toString()+"_"+
                                        query.value("phone_number").toString();
                    return information;
            }
        }
        else{
            QMessageBox::critical(0,QString("getPatientProfile异常"),query.lastError().text());
            return QString();
        }
    }
    return QString();
}

//修改医生信息
bool DBUtils::updateDoctorProfile(QString name, QString gender, QString age, QString phone_number, QString department){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("update doctor "
                            "set gender = :gender, "
                            "age = :age, "
                            "phone_number = :phone_number, "
                            "department = :department "
                            "where name = :name");
        query.prepare(sql);
        query.bindValue(":name",name);
        query.bindValue(":gender",gender);
        query.bindValue(":age",age);
        query.bindValue(":phone_number",phone_number);
        query.bindValue(":department",department);
        if(query.exec()){
            if(query.numRowsAffected()>0){
                return true;
            }
        }
        else{
            QMessageBox::critical(0, QString("updateDoctorProfile异常"), query.lastError().text());
            return false;
        }
    }
    return false;
}

//修该病人信息
bool DBUtils::updatePatientProfile(QString name, QString gender, QString age, QString phone_number){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("update patient "
                            "set gender = :gender, "
                            "age = :age, "
                            "phone_number = :phone_number "
                            "where name = :name");
        query.prepare(sql);
        query.bindValue(":name",name);
        query.bindValue(":gender",gender);
        query.bindValue(":age",age);
        query.bindValue(":phone_number",phone_number);
        if(query.exec()){
            if(query.numRowsAffected()>0){
                return true;
            }
        }
        else{
            QMessageBox::critical(0, QString("updatePatientProfile异常"), query.lastError().text());
            return false;
        }
    }
    return false;
}

//获取病人所挂号的医生信息
QStringList DBUtils::getPatient_Doctor(QString username){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select doctor_name "
                            "from appointment_case "
                            "where patient_name = :patient_name");
        query.prepare(sql);
        query.bindValue(":patient_name",username);
        if(query.exec()){
            QStringList doctors;
            while(query.next()){
                QString doctorName=query.value(0).toString();
                if(!doctors.contains(doctorName)){
                    doctors.append(doctorName);
                }
            }
            return doctors;
        }
        else{
            QMessageBox::critical(0,QString("getPatient_Doctor异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

//获取医生所诊断的病人信息
QStringList DBUtils::getDoctor_Patient(QString username){
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select patient_name "
                            "from appointment_case "
                            "where doctor_name = :doctor_name");
        query.prepare(sql);
        query.bindValue(":doctor_name",username);
        if(query.exec()){
            QStringList patients;
            while(query.next()){
                QString patientName=query.value(0).toString();
                if(!patients.contains(patientName)){
                    patients.append(patientName);
                }
            }
            return patients;
        }
        else{
            QMessageBox::critical(0,QString("getDoctor_Patient异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

bool DBUtils::insertHealth(QString patientName, QString height, QString weight, QString heartRate, QString lungCapacity, QString systolic, QString diastolic) {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("insert into health "
                            "(patient_name, height, weight, heart_rate, lung_capacity, systolic, diastolic) "
                            "values(:patient_name, :height, :weight, :heart_rate, :lung_capacity, :systolic, :diastolic)");
        query.prepare(sql);
        query.bindValue(":patient_name",patientName);
        query.bindValue(":height",height);
        query.bindValue(":weight",weight);
        query.bindValue(":heart_rate",heartRate);
        query.bindValue(":lung_capacity",lungCapacity);
        query.bindValue(":systolic",systolic);
        query.bindValue(":diastolic",diastolic);
        if(query.exec()){
            return true;
        }
        else{
            QMessageBox::critical(0,QString("insertHealth异常"),query.lastError().text());
            return false;
        }
    }
    return false;
}

QString DBUtils::getHealth(QString patientName) {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select height, weight, heart_rate, lung_capacity, systolic, diastolic "
                            "from patient "
                            "where patient_name = :patient_name");
        query.prepare(sql);
        query.bindValue(":patient_name", patientName);
        if(query.exec()){
            while(query.next()){
                QString information=
                        query.value("height").toString()+"_"+
                        query.value("weight").toString()+"_"+
                        query.value("heart_rate").toString()+"_"+
                        query.value("lung_capacity").toString()+"_"+
                        query.value("systolic").toString()+"_"+
                        query.value("diastolic").toString();
                return information;
            }
        }
        else{
            QMessageBox::critical(0,QString("getHealth异常"),query.lastError().text());
            return QString();
        }
    }
    return QString();
}

bool DBUtils::updateHealth(QString patientName, QString height, QString weight, QString heartRate, QString lungCapacity, QString systolic, QString diastolic) {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("update health set "
                            "height=:height, weight=:weight, heart_rate=:heart_rate, lung_capacity=:lung_capacity, systolic=:systolic, diastolic=:diastolic "
                            "where patient_name = :patient_name");
        query.prepare(sql);
        query.bindValue(":height",height);
        query.bindValue(":weight",weight);
        query.bindValue(":heart_rate",heartRate);
        query.bindValue(":lung_capacity",lungCapacity);
        query.bindValue(":systolic",systolic);
        query.bindValue(":diastolic",diastolic);
        query.bindValue(":patient_name",patientName);
        if(query.exec()){
            return true;
        }
        else{
            QMessageBox::critical(0,QString("updateHealth异常"),query.lastError().text());
            return false;
        }
    }
    return false;
}

QStringList DBUtils::getAllUsers() {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select username, password, role from user");
        query.prepare(sql);
        if(query.exec()){
            QStringList res;
            while(query.next()){
                QString information =
                        query.value("username").toString() + "_" +
                        query.value("password").toString() + "_" +
                        query.value("role").toString();
                res.append(information);
            }
            return res;
        }
        else{
            QMessageBox::critical(0,QString("getAllUsers异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllPatientProfile() {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select user_id, name, gender, age, phone_number from patient");
        query.prepare(sql);
        if(query.exec()){
            QStringList res;
            while(query.next()){
                QString information =
                        query.value("user_id").toString() + "_" +
                        query.value("name").toString() + "_" +
                        query.value("gender").toString() + "_" +
                        query.value("age").toString() + "_" +
                        query.value("phone_number").toString();
                res.append(information);
            }
            return res;
        }
        else {
            QMessageBox::critical(0,QString("getAllPatientProfile异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllDoctorProfile() {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select user_id, name, gender, age, phone_number, department from doctor");
        query.prepare(sql);
        if(query.exec()){
            QStringList res;
            while(query.next()){
                QString information =
                        query.value("user_id").toString() + "_" +
                        query.value("name").toString() + "_" +
                        query.value("gender").toString() + "_" +
                        query.value("age").toString() + "_" +
                        query.value("phone_number").toString();
                res.append(information);
            }
            return res;
        }
        else{
            QMessageBox::critical(0,QString("getAllPatientProfile异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}

QStringList DBUtils::getAllCases() {
    if(connect(DBUtils::DBName)){
        QSqlQuery query;
        QString sql=QString("select patient_name, doctor_name, appointment_date, department, symptoms, diagnosis, medicine from appointment_case");
        query.prepare(sql);
        if(query.exec()){
            QStringList res;
            while(query.next()){
                QString information =
                        query.value("patient_name").toString() + "_" +
                        query.value("doctor_name").toString() + "_" +
                        query.value("appointment_date").toString() + "_" +
                        query.value("department").toString() + "_" +
                        query.value("symptoms").toString() + "_" +
                        query.value("diagnosis").toString() + "_" +
                        query.value("medicine").toString();
                res.append(information);
            }
            return res;
        }
        else{
            QMessageBox::critical(0,QString("getAllCases异常"),query.lastError().text());
            return QStringList();
        }
    }
    return QStringList();
}
