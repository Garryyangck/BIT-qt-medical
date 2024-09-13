#ifndef HEALTHTEST_H
#define HEALTHTEST_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QLineEdit>

#include "patientprofile.h"
#include "user.h"
#include "healthtestoutcome.h"

namespace Ui {
class healthtest;
}

class healthtest : public QWidget
{
    Q_OBJECT

public:
    explicit healthtest(QTcpSocket *socket, User *user, QWidget *parent = nullptr);
    ~healthtest();

private slots:

    void on_healthtest_return_clicked();

    void on_healthtest_sure_clicked();

private:
    Ui::healthtest *ui;
    QTcpSocket *socket;
    User *user;


    QString Health_Test(int heightint, int weightint,
                        int heart_rateint, int pressureint_height,
                        int pressureint_low, int vital_capacityint,
                        const QString &sexstr, int temperatureint,
                        bool heightok, bool weightok, bool heartok,
                        bool presshighok, bool presslowok,
                        bool vitalok, bool temperaok);

    QString sexstr;
    int heightint;
    int weightint;
    int heart_rateint;
    int pressureint_height;
    int pressureint_low;
    int temperatureint;
    int vital_capacityint;
};

#endif // HEALTHTEST_H
