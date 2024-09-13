#include "appointmentchoose.h"
#include "ui_appointmentchoose.h"

appointmentchoose::appointmentchoose(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::appointmentchoose)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;
}

appointmentchoose::~appointmentchoose()
{
    delete ui;
}


void appointmentchoose::on_department_choose_clicked()
{
    departmentchoose* depcho = new departmentchoose(socket, user);
    depcho->setWindowTitle("按照科室检索医生");
    depcho->show();
    this->hide();
}

void appointmentchoose::on_time_choose_clicked()
{
    schedulechoose* schcho = new schedulechoose(socket, user);
    schcho->setWindowTitle("按照时间检索医生");
    schcho->show();
    this->hide();
}

void appointmentchoose::on_appointreturn_clicked()
{
    PatientProfile *patfill = new PatientProfile(socket, user);
    patfill->setWindowTitle(QString("患者界面"));
    patfill->show();
    this->hide();
}
