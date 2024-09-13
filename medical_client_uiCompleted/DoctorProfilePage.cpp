#include "DoctorProfilePage.h"

DoctorProfilePage::DoctorProfilePage(QTcpSocket *socket, User *user, QObject *parent) : QObject(parent) {
    this->socket = socket;
    this->user = user;
}
