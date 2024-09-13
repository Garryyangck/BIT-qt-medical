#ifndef INITPAGE_H
#define INITPAGE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "utils.h"
#include "PatientProfilePage.h"
#include "DoctorProfilePage.h"

class InitPage : public QObject {
    Q_OBJECT
public:
    explicit InitPage(QObject *parent = nullptr);
    bool identity;
    User *user;
    QTcpSocket *socket;
    QString receiver;

public slots:
    void doLogin(const QString &username, const QString &password, const bool &identity);

    void doRegister(const QString &username, const QString &password, const bool &identity);

    bool getIdentity();

    QString getUsername();

    QString getReceiver();

    // ------------------------------------------患者使用的api------------------------------------------

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

    // ------------------------------------------通用的api------------------------------------------

    // 医生和患者通用的，获取可聊天的对象
    void getChats();

    // 医生和患者通用的，输入聊天对象，请求历史信息
    void beginChat(QString receiver);

    // 医生和患者通用的，发送消息
    void sendMessage(QString content, QString receiver);

    // 医生和患者通用的，退出即时聊天，非常关键，离开即时通讯页面的时候，必须要调这个接口
    void quitChatRoom(QString receiver);

    // ------------------------------------------医生使用的api------------------------------------------

    // 查看医生所有的病例，无论是否已经诊断
    void getDoctorCase();

    // 医生修改病例
    void updateDoctorCase(QString patientName, QString doctorName, QString date, QString department, QString symptoms, QString diagnosis, QString medicine);

    // 医生获取自己的个人信息
    void getDoctorProfile();

    // 医生更改自己个人信息的逻辑
    void updateDoctorProfile(QString gender, QString age, QString phoneNumber, QString department);

signals:
    void errorOccurred(const QString &message);
    void registerSuccess();
    void loginSuccess();
    void returnSchedule(const QStringList &schedule);
    void returnDoctorProfile(const QStringList &doctorProfile);
    void appointResult(const bool isSuccess);
    void returnAllCase(const QStringList &cases);
    void returnPatientInfo(const QStringList &info);
    void updatePatientInfoResult(const bool isSuccess);
    void returnChats(const QStringList &chats);
    void returnHistories(const QStringList &histories);
    void receiveNewMessage(const QString &content);
    void sendSuccess(const QString &content);
    void receiverOffline(const QString &content);
    void returnDoctorCase(const QStringList &doctorCase);
    void updateDoctorCaseResult(const bool isSuccess);
    void returnDoctorInfo(const QStringList &doctorProfile);
    void updateDoctorInfoResult(const bool isSuccess);
};

#endif
