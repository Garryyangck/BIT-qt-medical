#ifndef DOCTORCASE_H
#define DOCTORCASE_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QMessageBox>

#include "doctorprofile.h"
#include "user.h"
#include "utils.h"

namespace Ui {
class doctorcase;
}

class doctorcase : public QWidget
{
    Q_OBJECT

public:
    explicit doctorcase(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~doctorcase();

private slots:
    void on_doctorcase_return_clicked();

    void on_doctorcase_sure_2_clicked();

    void on_doctorcase_sure_clicked();

private:
    Ui::doctorcase *ui;
    QTcpSocket *socket;
    User *user;
};

#endif // DOCTORCASE_H
