#ifndef DBUTILS_H
#define DBUTILS_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

class DBUtils : public QObject
{
    Q_OBJECT
public:
    static QString DBName;

    static QString DATE_FORMAT;

    static QString DATE_FORMAT_ONLY_DATE;

    static QString ALL_DATE;

    static QString ALL_DOCTOR;

    explicit DBUtils(QObject *parent = nullptr);

    // 登录注册相关
    static bool connect(const QString &dbName);

    static bool login(QString username, QString password, int role);

    static bool signUp(QString username, QString password, int role, QString gender, QString age, QString phoneNumber);


    // 查找可聊天用户相关
    static QStringList getReceivers(QString username, int role);

    static QStringList getAllPatients();

    static QStringList getAllDoctors();


    // 即时通讯相关
    static int getUnreadNum(QString receiverName, QString senderName);

    static QStringList getChatMessage(QString username, QString chatter);

    static bool insertChatMessage(QString sender, QString receiver, QDateTime chatDate, QString content, int isRead);

    static bool updateIsRead(QString username, QString chatter);


    // 挂号（排班）相关
    static QHash<QString,QStringList> getScheduleDate();

    static QHash<QString,QStringList> getScheduleDepartment();

    static bool updateScheduleDecreaseNum(QString doctorName, QString department, QString date);

    static QStringList getAllDepartments();

    static QStringList getAllDates(QString dep);

    static QStringList getDoctorFromDate(QString date, QString dep);

    static int getAvailableNum(QString doctorName, QString department, QString date);

    // 下面“四托”是要调用的
    // 两个都不是“全部”
    static QStringList getScheduleByDateDoctor(QString department, QString date, QString doctor);

    static QStringList getScheduleByDate(QString department, QString date);

    static QStringList getScheduleByDoctor(QString department, QString doctor);
    // 两个都是“全部”
    static QStringList getSchedule(QString department);


    // 病例处方相关
    static QStringList getAppointmentCase(QString username,int role);

    static bool updateAppointmentCase(QString patientName, QString doctorName, QString appointmentDate, QString department, QString symptoms, QString diagnosis, QString medicine);

    static bool insertAppointmentCase(QString patientName, QString doctorName, QString appointmentDate, QString department, QString symptoms, QString diagnosis, QString medicine, int isDialogue);


    // 个人信息相关
    static QString getDoctorProfile(QString username);

    static QString getPatientProfile(QString username);

    static bool updateDoctorProfile(QString name,QString gender, QString age, QString phone_number, QString department);

    static bool updatePatientProfile(QString name,QString gender, QString age, QString phone_number);

    static QStringList getPatient_Doctor(QString username);

    static QStringList getDoctor_Patient(QString username);

    // health相关
    static bool insertHealth(QString patientName, QString height, QString weight, QString heartRate, QString lungCapacity, QString systolic, QString diastolic);

    static QString getHealth(QString patientName);

    static bool updateHealth(QString patientName, QString height, QString weight, QString heartRate, QString lungCapacity, QString systolic, QString diastolic);

    // 数据库页面相关
    static QStringList getAllUsers();

    static QStringList getAllPatientProfile();

    static QStringList getAllDoctorProfile();

    static QStringList getAllCases();

signals:
};


#endif // DBUTILS_H
