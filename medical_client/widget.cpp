#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("如医医患平台欢迎您！"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    // 连接服务器
    QString ip = QString("127.0.0.1");
    QString port = QString("8888");
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress(ip), port.toShort());

    // 回车键直接登录
    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &Widget::on_loginButton_clicked);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &Widget::on_loginButton_clicked);
}

Widget::Widget(User *user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->usernameLineEdit->setText(user->username);
    ui->passwordLineEdit->setText(user->password);
    if (user->role == 1) {
        ui->patientRadioButton->click();
    }

    // 连接服务器
    QString ip = QString("127.0.0.1");
    QString port = QString("8888");
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress(ip), port.toShort());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_quitButton_clicked()
{
    this->close();
}

void Widget::on_loginButton_clicked()
{
    // 获取用户名和密码，以及身份
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    int role = 0;
    if (ui->patientRadioButton->isChecked()) // 病人role=1
        role++;
    User *user = new User(username, password, role);

    // 构造Message对象
    Message msg(user, new User, QString(), MessageType::LOGIN);

    // 传输Message
    socket->write(Message::messageToByteArray(msg));

    // 阻塞等待服务器的响应
    connect(socket, &QTcpSocket::readyRead, this, [this, username, password, role, user](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器对登录的响应";
        msg.print();

        // 关闭connect，防止抢夺其它地方的消息
        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        // 判断是否登录成功
        if (msg.messageType == MessageType::LOGIN_SUCCESS) {
            // 登录成功，提示框
            QString msg = QString(username);
            msg.append(QString("，欢迎您！"));
            QMessageBox::information(0, QString("登录成功"), msg);

            // 跳转到对应窗口
            this->hide();
            if (role == 0) {
                DoctorProfile *doctorProfile = new DoctorProfile(socket, user);
                doctorProfile->setWindowTitle(QString("医生界面"));
                doctorProfile->show();
            } else if (role == 1) {
                PatientProfile *patientProfile = new PatientProfile(socket, user);
                patientProfile->setWindowTitle(QString("患者界面"));
                patientProfile->show();
            }
        } else if (msg.messageType == MessageType::LOGIN_FAILED) {
            // 登录失败
            ui->usernameLineEdit->setText(QString(""));
            ui->passwordLineEdit->setText(QString(""));
            QMessageBox::information(0, QString("登录失败"), QString("用户名或密码不正确"));
        }
    });
}

void Widget::on_signupButton_clicked()
{
    SignUp *signUp = new SignUp(socket);
    signUp->show();
    this->close();
}
