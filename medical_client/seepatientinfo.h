#ifndef SEEPATIENTINFO_H
#define SEEPATIENTINFO_H

#include <QWidget>
#include <QTcpSocket>

#include "doctorprofile.h"
#include "user.h"

namespace Ui {
class seepatientinfo; // 改名之后，这里可能有bug
}

class seeAllSchedule : public QWidget
{
    Q_OBJECT

public:
    explicit seeAllSchedule(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~seeAllSchedule();

private slots:
    void on_patientinfo_return_clicked();

    void on_patientinfo_search_clicked();

private:
    Ui::seepatientinfo *ui; // 改名之后，这里可能有bug
    QTcpSocket *socket;
    User *user;
};

#endif // SEEPATIENTINFO_H
