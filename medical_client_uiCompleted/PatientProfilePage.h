#ifndef PATIENTPROFILEPAGE_H
#define PATIENTPROFILEPAGE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "utils.h"

class PatientProfilePage : public QObject
{
    Q_OBJECT
public:
    explicit PatientProfilePage(QTcpSocket *socket, User *user, QObject *parent = nullptr);

    User *user;
    QTcpSocket *socket;

public slots:
    // 根据三个参数，获取对应的schedule，展示到页面上
    void getSchedule(QString department, QString date, QString doctor);

    // 查看医生信息
    void getDoctorProfile(QString doctorName);

    // 患者点击预约
    void doAppointment(QString department, QString date, QString doctor);

    // 患者查询自己所有的病例
    void getAllCase();

    // 患者获取自己的个人信息
    void getPatientProfile();

    // 患者更改自己个人信息的逻辑
    void updatePatientProfile(QString gender, QString age, QString phoneNumber);

    // 健康自测
    void submitHealth();

signals:

};

#endif // PATIENTPROFILEPAGE_H
