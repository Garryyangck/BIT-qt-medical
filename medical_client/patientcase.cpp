#include "patientcase.h"
#include "ui_patientcase.h"

patientcase::patientcase(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::patientcase)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    on_patientcase_sure_clicked();
}

patientcase::~patientcase()
{
    delete ui;
}

void patientcase::on_patientcase_return_clicked()
{
    PatientProfile *pp = new PatientProfile(socket, user);
    pp->setWindowTitle(QString("患者界面"));
    pp->show();
    this->hide();
}

void patientcase::on_patientcase_sure_clicked()
{
    Message requestMsg(this->user, new User, QString(), MessageType::PAT_SEE_CASE);
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::PAT_SEE_CASE_SUC)
        {
            QStringList patientsee = msg.content.split("|");

            ui->seecase->clear();
            for(const QString &seestr : patientsee)
            {
                QStringList parts = seestr.split("_");
                QString pu = parts[0];
                QString du = parts[1];
                QString date = parts[2];
                QString dep = parts[3];
                QString symp = parts[4];
                QString diag = parts[5];
                QString medic = parts[6];
                ui->seecase->append("患者：" + pu + "  医生：" + du + "  就诊日期"
                                    + date + "  科室：" + dep + "  症状：" + symp +
                                     "  医嘱：" + diag + "  处方及缴费：" + medic + "\n");
            }
        }
    });
}
