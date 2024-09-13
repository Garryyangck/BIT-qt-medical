#include "seepatientinfo.h"
#include "ui_seepatientinfo.h"

seeAllSchedule::seeAllSchedule(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seepatientinfo)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    on_patientinfo_search_clicked(); // 构造函数中就去请求
}

seeAllSchedule::~seeAllSchedule()
{
    delete ui;
}

void seeAllSchedule::on_patientinfo_return_clicked()
{
    DoctorProfile *docfil = new DoctorProfile (socket, user);
    docfil->setWindowTitle(QString("医生界面"));
    docfil->show();
    this->hide();
}

void seeAllSchedule::on_patientinfo_search_clicked()
{
    Message requestMsg(this->user, new User, QString(), MessageType::SEE_ALL_SCHEDULE); // 请求医生的所有挂号信息
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::SEE_ALL_SCHEDULE_SUC)
        {
            QStringList patientList = msg.content.split("|");

            // 将医生列表显示在 UI 中，例如 QListWidge
            ui->mypatient_info->clear();
            for(const QString &patientstr : patientList)
            {
                QStringList parts = patientstr.split("_");
                QString pu = parts[0];
                QString du = parts[1];
                QString date = parts[2];
                QString dep = parts[3];
                QString symp = parts[4];
                QString diag = parts[5];
                QString medic = parts[6];
                ui->mypatient_info->append("患者：" + pu + "  医生：" + du + "  就诊日期："
                                    + date + "  科室：" + dep + "  症状：" + symp +
                                     "  医嘱：" + diag + "  处方及缴费：" + medic + "\n");
            }
        }
    });
}
