#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QWidget>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>

#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "dbutils.h"
#include "utils.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QTcpSocket *socket, User *user, QThread *parent = nullptr);
    void run() override;

signals:
    // 将接收到的Message转发给主线程
    void sendToMainThread(Message msg);

public slots:
    void clientManageSlot();

private:
    QTcpSocket *socket;
    User *user;
};

#endif // MYTHREAD_H
