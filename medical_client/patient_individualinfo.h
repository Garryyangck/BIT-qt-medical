#ifndef PATIENT_INDIVIDUALINFO_H
#define PATIENT_INDIVIDUALINFO_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>

#include "user.h"
#include "patientprofile.h"
#include "utils.h"

namespace Ui {
class patient_individualinfo;
}

class patient_individualinfo : public QWidget
{
    Q_OBJECT

public:
    explicit patient_individualinfo(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~patient_individualinfo();

private slots:
    void on_patient_info_return_clicked();

    void on_patient_info_sure_clicked();

private:
    Ui::patient_individualinfo *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // PATIENT_INDIVIDUALINFO_H
