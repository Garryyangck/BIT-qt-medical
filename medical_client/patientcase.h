#ifndef PATIENTCASE_H
#define PATIENTCASE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>

#include "user.h"
#include "patientprofile.h"
#include "utils.h"

namespace Ui {
class patientcase;
}

class patientcase : public QWidget
{
    Q_OBJECT

public:
    explicit patientcase(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~patientcase();

private slots:
    void on_patientcase_return_clicked();

    void on_patientcase_sure_clicked();

private:
    Ui::patientcase *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // PATIENTCASE_H
