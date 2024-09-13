#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    this->socket = socket;
    ui->setupUi(this);

    this->setWindowTitle(QString("如医医患平台欢迎您！"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);

    // 回车直接发送
    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &SignUp::on_signupButton_clicked);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &SignUp::on_signupButton_clicked);
    connect(ui->genderLineEdit, &QLineEdit::returnPressed, this, &SignUp::on_signupButton_clicked);
    connect(ui->ageLineEdit, &QLineEdit::returnPressed, this, &SignUp::on_signupButton_clicked);
    connect(ui->phoneNumberLineEdit, &QLineEdit::returnPressed, this, &SignUp::on_signupButton_clicked);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_quitButton_clicked()
{
    this->close();
}

void SignUp::on_signupButton_clicked()
{
    // 必填
    QString username = ui->usernameLineEdit->text();
    if (username.trimmed() == QString("")) {
        QMessageBox::warning(0, QString("注册失败"), QString("用户名不能为空"));
        return;
    }
    QString password = ui->passwordLineEdit->text();
    if (password.trimmed() == QString("")) {
        QMessageBox::warning(0, QString("注册失败"), QString("密码不能为空"));
        return;
    }
    int role = 0;
    if (ui->patientRadioButton->isChecked())
        role++;
    // 选填
    QString gender = Utils::checkBlank(ui->genderLineEdit->text());
    if (gender != QString("男") && gender != QString("女") && gender != Utils::blankMsg) {
        QMessageBox::warning(0, QString("注册失败"), QString("性别选填男或女"));
        return;
    }
    QString age = Utils::checkBlank(ui->ageLineEdit->text());
    QString phoneNumber = Utils::checkBlank(ui->phoneNumberLineEdit->text());

    // 构建user对象
    User *user = new User(username, password, role);
    // 使用“_”分割
    QString content = gender + QString("_") + age + QString("_") + phoneNumber;
    // 构建Message对象
    Message msg(user, new User, content, MessageType::REGISTER);

    // 发送注册请求
    socket->write(Message::messageToByteArray(msg));

    connect(socket, &QTcpSocket::readyRead, this, [this, user](){
        QByteArray ba = socket->readAll();
        Message msg = Message::byteArrayToMessage(ba);

        qDebug() << "服务器对注册的响应";
        msg.print();

        // 关闭connect，防止抢夺其它地方的消息，这里其实不用，因为创建了新的socket，即注册和后续的登录用的不是同一个socket
        disconnect(socket, &QTcpSocket::readyRead, this, nullptr);

        if (msg.messageType == MessageType::REGISTER_SUCCESS) {
            QMessageBox::information(0, QString("注册成功"), QString("注册成功，快去登录吧！"));
            Widget *w = new Widget(user);
            w->show();
            this->close();
        } else if (msg.messageType == MessageType::REGISTER_FAILED) {
            QMessageBox::critical(0, QString("注册失败"), QString("用户名重复或服务器异常"));
        }
    });
}
