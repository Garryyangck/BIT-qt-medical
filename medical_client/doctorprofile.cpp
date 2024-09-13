#include "doctorprofile.h"
#include "ui_doctorprofile.h"

DoctorProfile::DoctorProfile(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::doctorProfile)
{
    this ->socket = socket;
    this->user = user;
    ui->setupUi(this);

    this->setWindowTitle(QString("医生页面"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
}

DoctorProfile::~DoctorProfile()
{
    delete ui;
}

void DoctorProfile::on_quitButton_clicked()
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

void DoctorProfile::on_chatButton_clicked()
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

void DoctorProfile::on_appointmentButton_clicked() // 过时
{
    seeAllSchedule* sptinf = new seeAllSchedule(socket, user);
    sptinf->setWindowTitle(QString("查看挂号患者信息"));
    sptinf->show();
    this->hide();
}

void DoctorProfile::on_recordButton_clicked() // 过时
{
    doctorcase* doct = new doctorcase(socket, user);
    doct->setWindowTitle(QString("查看并修改病历"));
    doct->show();
    this->hide();
}

void DoctorProfile::on_prescriptionButton_clicked() // 过时
{
    doctorcase* doct = new doctorcase(socket, user);
    doct->setWindowTitle(QString("查看处方与缴费"));
    doct->show();
    this->hide();
}

void DoctorProfile::on_profileButton_clicked()
{
    doctor_individualinfo* doc = new doctor_individualinfo(socket, user);
    doc->setWindowTitle(QString("个人信息编辑"));
    doc->show();
    this->hide();
}

void DoctorProfile::on_appointmentCaseButton_clicked()
{
    doctorcase* doct = new doctorcase(socket, user);
    doct->setWindowTitle(QString("编辑病例和处方"));
    doct->show();
    this->hide();
}

void DoctorProfile::on_scheduleButton_clicked()
{
    seeAllSchedule* sptinf = new seeAllSchedule(socket, user);
    sptinf->setWindowTitle(QString("查看所有挂号和病例"));
    sptinf->show();
    this->hide();
}
