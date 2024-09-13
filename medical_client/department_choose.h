#ifndef DEPARTMENT_CHOOSE_H
#define DEPARTMENT_CHOOSE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <message.h>

#include "utils.h"
#include "patientprofile.h"
#include "user.h"
#include "schedule_choose.h"
#include "doctor_choose.h"

namespace Ui {
class department_choose;
}

class department_choose : public QWidget
{
    Q_OBJECT

public:
    explicit department_choose(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~department_choose();

private slots:
    void on_department_choose_return_clicked();

    void on_department_choose_sure_clicked();

    void on_department_choose_sure_2_clicked();

private:
    Ui::department_choose *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // DEPARTMENT_CHOOSE_H
