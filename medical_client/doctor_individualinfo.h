#ifndef DOCTOR_INDIVIDUALINFO_H
#define DOCTOR_INDIVIDUALINFO_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>

#include "user.h"
#include "message.h"
#include "doctorprofile.h"
#include "utils.h"

namespace Ui {
class doctor_individualinfo;
}

class doctor_individualinfo : public QWidget
{
    Q_OBJECT

public:
    explicit doctor_individualinfo(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~doctor_individualinfo();

private slots:
    void on_doctor_info_sure_clicked();

    void on_doctor_info_return_clicked();

private:
    Ui::doctor_individualinfo *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // DOCTOR_INDIVIDUALINFO_H
