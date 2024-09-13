#include "seedoctorinfo.h"
#include "ui_seedoctorinfo.h"
#include "patientprofile.h"

seedoctorinfo::seedoctorinfo(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seedoctorinfo)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;
}

seedoctorinfo::~seedoctorinfo()
{
    delete ui;
}

void seedoctorinfo::on_doctorinfo_return_clicked()
{
    PatientProfile *patfil = new PatientProfile(socket, user);
    patfil->setWindowTitle(QString("患者界面"));
    patfil->show();
    this->hide();
}

void seedoctorinfo::on_doctorinfo_search_clicked()
{
    Message requestMsg(this->user, new User, QString(), MessageType::SEE_DOCTOR_INFO);
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::SEE_D_SUCCESS)
        {
            QStringList doctorList = msg.content.split("|");

            // 将医生列表显示在 UI 中，例如 QListWidge
            ui->mydoctor_info->clear();
            for(const QString &doctorstr : doctorList)
            {
                ui->mydoctor_info->append(doctorstr+"\n");
            }
        }
    });
}
