#ifndef SEEDOCTORINFO_H
#define SEEDOCTORINFO_H

#include <QWidget>
#include <QTcpSocket>

#include "patientprofile.h"
#include "user.h"

namespace Ui {
class seedoctorinfo;
}

class seedoctorinfo : public QWidget
{
    Q_OBJECT

public:
    explicit seedoctorinfo(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~seedoctorinfo();

private slots:
    void on_doctorinfo_return_clicked();

    void on_doctorinfo_search_clicked();

private:
    Ui::seedoctorinfo *ui;
    QTcpSocket *socket;
    User* user;
};

#endif // SEEDOCTORINFO_H
