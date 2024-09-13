#include "doctorcase.h"
#include "ui_doctorcase.h"

doctorcase::doctorcase(QTcpSocket *socket, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::doctorcase)
{
    ui->setupUi(this);
    this->socket = socket;
    this->user = user;

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    on_doctorcase_sure_2_clicked(); // 构造函数就去请求
}

doctorcase::~doctorcase()
{
    delete ui;
}

void doctorcase::on_doctorcase_return_clicked()
{
    DoctorProfile *dp = new DoctorProfile(socket, user);
    dp->setWindowTitle(QString("医生界面"));
    dp->show();
    this->hide();
}

void doctorcase::on_doctorcase_sure_2_clicked()
{
    Message requestMsg(this->user, new User, QString(), MessageType::DOC_SEE_CASE); // 查看自己没有诊断的病例
    socket->write(Message::messageToByteArray(requestMsg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::DOC_SEE_CASE_SUC)
        {
            // 这里用“|”拆开
            QStringList doctorsee = msg.content.split("|");

            ui->seecase->clear();
            for(const QString &seestr : doctorsee)
            {
                // 接收到的字符串：xxx_xxx_xxx
                // 再使用“_”拆开
                // 显示的美观一点
                QStringList parts = seestr.split("_");
                QString pu = parts[0];
                QString du = parts[1];
                QString date = parts[2];
                QString dep = parts[3];
                QString symp = parts[4];
                QString diag = parts[5];
                QString medic = parts[6];
                ui->seecase->append("患者：" + pu + "  医生：" + du + "  就诊日期："
                                    + date + "  科室：" + dep + "  症状：" + symp +
                                     "  医嘱：" + diag + "  处方及缴费：" + medic + "\n");
            }
        }
    });
}

void doctorcase::on_doctorcase_sure_clicked()
{
    QString pu = Utils::checkBlank(ui->patient_username->text());
    if(pu == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString du = Utils::checkBlank(ui->doctor_username->text());
    if(du == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString da = Utils::checkBlank(ui->date->text());
    if(da == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString dep = Utils::checkBlank(ui->department->text());
    if(dep == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString sym = Utils::checkBlank(ui->symptoms->text());
    if(sym == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString dia = Utils::checkBlank(ui->diagnosis->text());
    if(dia == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }
    QString med = Utils::checkBlank(ui->medicine->text());
    if(med == Utils::blankMsg) {
        QMessageBox::critical(0, "插入失败", "病例不能有为空的字段");
        return;
    }

    QString content = pu + "_" + du + "_" + da + "_" + dep +
                      "_" + sym + "_" + dia + "_" + med ;

    Message msg(this->user, new User, content, MessageType::DOC_UPDATE_CASE);

    // 发送注册请求
    socket->write(Message::messageToByteArray(msg));

    // 监听服务器响应
    connect(socket, &QTcpSocket::readyRead, this, [this](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器响应：";
        msg.print();

        // disconnect，只要是涉及到按钮，都在connect里面disconnect
        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if(msg.messageType == MessageType::DOC_UPDATE_CASE_SUC)
        {
            QMessageBox:: information(0, QString("提示"), QString("诊断成功"));
            DoctorProfile *dp = new DoctorProfile(socket, user);
            dp->setWindowTitle("医生界面");
            dp->show();
            this->hide();
        }
        else if(msg.messageType == MessageType::DOC_UPDATE_CASE_FAI)
        {
            QMessageBox:: information(0, QString("提示"), QString("诊断失败，您输入的病例有误或不存在"));
        }
    });
}
