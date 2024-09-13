#ifndef SCHEDULE_CHOOSE_H
#define SCHEDULE_CHOOSE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>

#include "user.h"
#include "department_choose.h"
#include "patientprofile.h"
#include "department_choose.h"
#include "ui_department_choose.h"

namespace Ui {
class schedule_choose;
}

class schedule_choose : public QWidget
{
    Q_OBJECT

public:
    explicit schedule_choose(QTcpSocket *socket, User *user, QString dep, QWidget *parent = nullptr);
    ~schedule_choose();

private slots:
    void on_returnbutton_clicked();

    void on_choose_schedule_activated(const QString &arg1);

    void on_surebutton_clicked();

private:
    Ui::schedule_choose *ui;
    QTcpSocket *socket;
    User *user;
    QString dep;
};

#endif // SCHEDULE_CHOOSE_H
