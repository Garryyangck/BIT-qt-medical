#include "user.h"

User::User()
{
    this->username = QString();
    this->password = QString();
    this->role = 0;
}

User::User(QString username, QString password, int role)
{
    this->username = QString(username);
    this->password = QString(password);
    this->role = role;
}
