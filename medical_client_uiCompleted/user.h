#ifndef USER_H
#define USER_H

#include <QObject>

class User
{
public:
    User();
    User(QString username, QString password, int role);

    QString username;
    QString password;
    int role; // 0为医生，1为病人
};

#endif // USER_H
