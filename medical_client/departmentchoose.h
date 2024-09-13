#ifndef DEPARTMENTCHOOSE_H
#define DEPARTMENTCHOOSE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <message.h>

#include "appointmentchoose.h"
#include "user.h"
#include "utils.h"

namespace Ui {
class departmentchoose;
}

class departmentchoose : public QWidget
{
    Q_OBJECT

public:
    explicit departmentchoose(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~departmentchoose();

private slots:
    void on_depcho_sure_clicked();

    void on_depcho_return_clicked();

private:
    Ui::departmentchoose *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // DEPARTMENTCHOOSE_H
