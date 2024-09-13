#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QMessageBox>
#include <QException>
#include <QTcpServer>

#include "user.h"
#include "doctorprofile.h"
#include "patientprofile.h"
#include "widget.h"
#include "utils.h"

namespace Ui {
class SignUp;
}

class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QTcpSocket *socket, QWidget *parent = nullptr);
    ~SignUp();

signals:


private slots:
    void on_quitButton_clicked();

    void on_signupButton_clicked();

private:
    Ui::SignUp *ui;
    User *user;
    QTcpSocket *socket;
};

#endif // SIGNUP_H
