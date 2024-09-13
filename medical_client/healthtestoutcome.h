#ifndef HEALTHTESTOUTCOME_H
#define HEALTHTESTOUTCOME_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>

#include "healthtest.h"
#include "user.h"

namespace Ui {
class healthtestoutcome;
}

class healthtestoutcome : public QWidget
{
    Q_OBJECT

public:
    explicit healthtestoutcome(QString assessment, QTcpSocket *socket,
                               User *user, QWidget *parent = nullptr);
    ~healthtestoutcome();

private slots:
    void on_testoutcome_clicked();

private:
    Ui::healthtestoutcome *ui;
    QString assessment;
    QTcpSocket *socket;
    User *user;
};

#endif // HEALTHTESTOUTCOME_H
