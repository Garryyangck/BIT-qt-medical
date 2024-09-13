#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

#include "user.h"
#include "message.h"
#include "messagetype.h"
#include "doctorprofile.h"
#include "patientprofile.h"
#include "signup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    Widget(User *user, QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_quitButton_clicked();

    void on_loginButton_clicked();

    void on_signupButton_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;
};
#endif // WIDGET_H
