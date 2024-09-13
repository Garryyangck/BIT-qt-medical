#include "healthtestoutcome.h"
#include "ui_healthtestoutcome.h"

healthtestoutcome::healthtestoutcome(QString assessment, QTcpSocket *socket,
                                     User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::healthtestoutcome)
{
    ui->setupUi(this);
    this->assessment = assessment;
    this->socket = socket;
    this->user = user;
    ui->testoutcome_info->setText(assessment);
}

healthtestoutcome::~healthtestoutcome()
{
    delete ui;
}

void healthtestoutcome::on_testoutcome_clicked()
{
    healthtest* heates = new healthtest(socket, user);
    heates->setWindowTitle(QString("健康评估"));
    heates->show();
    this->hide();
}
