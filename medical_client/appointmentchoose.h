#ifndef APPOINTMENTCHOOSE_H
#define APPOINTMENTCHOOSE_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>

#include "departmentchoose.h"
#include "schedulechoose.h"
#include "patientprofile.h"
#include "user.h"

namespace Ui {
class appointmentchoose;
}

class appointmentchoose : public QWidget
{
    Q_OBJECT

public:
    explicit appointmentchoose(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~appointmentchoose();

private slots:
    void on_department_choose_clicked();

    void on_time_choose_clicked();

    void on_appointreturn_clicked();

private:
    Ui::appointmentchoose *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // APPOINTMENT_CASE_H
