#ifndef DOCTORPROFILEPAGE_H
#define DOCTORPROFILEPAGE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "utils.h"

class DoctorProfilePage : public QObject
{
    Q_OBJECT
public:
    explicit DoctorProfilePage(QTcpSocket *socket, User *user, QObject *parent = nullptr);

    User *user;
    QTcpSocket *socket;

signals:

};

#endif // DOCTORPROFILEPAGE_H
