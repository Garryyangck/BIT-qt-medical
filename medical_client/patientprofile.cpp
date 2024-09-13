#include "patientprofile.h"
#include "ui_patientprofile.h"

PatientProfile::PatientProfile(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientProfile)
{
    this->socket = socket;
    this->user = user;
    ui->setupUi(this);

    this->setWindowTitle(QString("患者页面"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
}

PatientProfile::~PatientProfile()
{
    delete ui;
}

void PatientProfile::on_quitButton_clicked()
{
    // 发送退出的请求
    Message msg(user, new User, QString(), MessageType::QUIT);
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::QUIT_SUCCESS) {
            this->close();
        }
    });
}

void PatientProfile::on_chatButton_clicked()
{
    this->hide();
    SelectReceiver *sr = new SelectReceiver(socket, user);
    sr->show();

    connect(sr, &SelectReceiver::returnToProfile, this, [this](){
        this->show();
    });

    connect(sr, &SelectReceiver::completeSelect, this, [this](QString receiver){
        Chat *c = new Chat(this->socket, this->user, receiver);
        c->show();

        connect(c, &Chat::returnToProfile, this, [this](){
            this->show();
        });
    });
}

void PatientProfile::on_appointmentButton_clicked() // 过时
{
    appointmentchoose* appcho = new appointmentchoose(socket, user);
    appcho->setWindowTitle(QString("挂号检索"));
    appcho->show();
    this->hide();
}

void PatientProfile::on_healthButton_clicked()
{
    healthtest* heates = new healthtest(socket, user);
    heates->setWindowTitle(QString("健康评估"));
    heates->show();
    this->hide();
}

void PatientProfile::on_doctorButton_clicked()
{
    seedoctorinfo* sdin = new seedoctorinfo(socket, user);
    sdin->setWindowTitle(QString("查看医生信息"));
    sdin->show();
    this->hide();
}

void PatientProfile::on_medicalRecordButton_clicked() // 过时
{
    patientcase *pc =new patientcase(socket, user);
    pc->setWindowTitle(QString("查看病历"));
    pc->show();
    this->hide();
}

void PatientProfile::on_profileButton_clicked()
{
    patient_individualinfo *pind = new patient_individualinfo(socket, user);
    pind->setWindowTitle(QString("查看个人信息"));
    pind->show();
    this->hide();

}

void PatientProfile::on_prescriptionButton_clicked() // 过时
{
    patientcase *pc =new patientcase(socket, user);
    pc->setWindowTitle(QString("查看病历"));
    pc->show();
    this->hide();
}

void PatientProfile::on_appointmentCaseButton_clicked()
{
    patientcase *pc =new patientcase(socket, user);
    pc->setWindowTitle(QString("查看病历"));
    pc->show();
    this->hide();
}

void PatientProfile::on_scheduleButton_clicked()
{ // 挂号
    department_choose *dep_cho = new department_choose(socket, user);
    dep_cho->setWindowTitle(QString("挂号"));
    dep_cho->show();
    this->hide();
}
