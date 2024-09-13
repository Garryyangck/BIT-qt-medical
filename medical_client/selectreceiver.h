#ifndef SELECTRECEIVER_H
#define SELECTRECEIVER_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QHash>

#include "user.h"
#include "message.h"
#include "messagetype.h"

namespace Ui {
class SelectReceiver;
}

class SelectReceiver : public QWidget
{
    Q_OBJECT

public:
    explicit SelectReceiver(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~SelectReceiver();

signals:
    void returnToProfile();

    void completeSelect(QString receiver);

private slots:
    void on_quitButton_clicked();

    void on_confirmButton_clicked();

    void on_receiverComboBox_activated(const QString &receiver);

private:
    Ui::SelectReceiver *ui;
    QTcpSocket *socket;
    User *user;
    QHash<QString, int> table; // 保持可以联系的对象，检查用户的选择是否合法
};

#endif // SELECTRECEIVER_H
