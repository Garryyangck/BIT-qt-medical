#include "department_choose.h"
#include "ui_department_choose.h"

department_choose::department_choose(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::department_choose)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    //构造函数中显示所有科室
    Message requestMsg(this->user, new User, QString(), MessageType::GET_ALL_DEPART);
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = this->socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(this->socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::GET_ALL_DEPART_SUC)
        {
            QStringList depList = msg.content.split("|");
            depList.removeDuplicates();

            ui->combo_d_choose->clear();

            QStringList filtered;
            for(const QString &item : depList)
            {
                if(!item.trimmed().isEmpty()) filtered.append(item);
            }
            ui->combo_d_choose->addItems(filtered);
        }
    });
}

department_choose::~department_choose()
{
    delete ui;
}

void department_choose::on_department_choose_return_clicked()
{
    PatientProfile *patfil = new PatientProfile(socket, user);
    patfil->setWindowTitle(QString("患者界面"));
    patfil->show();
    delete this;
}

void department_choose::on_department_choose_sure_clicked()
{
    QString readd = ui->combo_d_choose->currentText();
    schedule_choose *schcho = new schedule_choose(socket, user, readd);
    schcho->setWindowTitle(QString("按照排班时间检索医生"));
    schcho->show();
    this->hide();
}

void department_choose::on_department_choose_sure_2_clicked()
{
    QString readd = ui->combo_d_choose->currentText();
    doctor_choose *doccho = new doctor_choose(socket, user, readd);
    doccho->setWindowTitle(QString("按照医生姓名检索"));
    doccho->show();
    delete this;
}
