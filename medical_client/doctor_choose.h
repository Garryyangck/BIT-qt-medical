#ifndef DOCTOR_CHOOSE_H
#define DOCTOR_CHOOSE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>

#include "user.h"
#include "department_choose.h"
#include "patientprofile.h"

namespace Ui {
class doctor_choose;
}

class doctor_choose : public QWidget
{
    Q_OBJECT

public:
    explicit doctor_choose(QTcpSocket *socket, User *user, QString dep, QWidget *parent = nullptr);
    ~doctor_choose();

private slots:
    void on_returnbutton_clicked();

private:
    Ui::doctor_choose *ui;
    QTcpSocket *socket;
    User *user;
    QString dep;
};

#endif // DOCTOR_CHOOSE_H
