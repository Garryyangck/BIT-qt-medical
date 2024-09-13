#ifndef SCHEDULECHOOSE_H
#define SCHEDULECHOOSE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <message.h>

#include "appointmentchoose.h"
#include "user.h"

namespace Ui {
class schedulechoose;
}

class schedulechoose : public QWidget
{
    Q_OBJECT

public:
    explicit schedulechoose(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~schedulechoose();

private slots:
    void on_schedule_return_clicked();

    void on_schedule_sure_clicked();

private:
    Ui::schedulechoose *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // SCHEDULECHOOSE_H
