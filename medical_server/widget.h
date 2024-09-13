#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QThread>
#include <QHash>
#include <QDebug>
#include <QSql>

#include "mythread.h"
#include "dbutils.h"
#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define PORT 8888 // 服务器：127.0.0.1:8888

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newClientHandler();

    void threadMessageHandler(Message msg);

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QHash<QString, QTcpSocket*> table; // <username, socket>，保持和客户端的连接
};
#endif // WIDGET_H
