QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appointmentchoose.cpp \
    chat.cpp \
    department_choose.cpp \
    departmentchoose.cpp \
    doctor_choose.cpp \
    doctor_individualinfo.cpp \
    doctorcase.cpp \
    doctorprofile.cpp \
    healthtest.cpp \
    healthtestoutcome.cpp \
    main.cpp \
    message.cpp \
    messagetype.cpp \
    patient_individualinfo.cpp \
    patientcase.cpp \
    patientprofile.cpp \
    schedule_choose.cpp \
    schedulechoose.cpp \
    seedoctorinfo.cpp \
    seepatientinfo.cpp \
    selectreceiver.cpp \
    signup.cpp \
    user.cpp \
    utils.cpp \
    widget.cpp

HEADERS += \
    appointmentchoose.h \
    chat.h \
    department_choose.h \
    departmentchoose.h \
    doctor_choose.h \
    doctor_individualinfo.h \
    doctorcase.h \
    doctorprofile.h \
    healthtest.h \
    healthtestoutcome.h \
    message.h \
    messagetype.h \
    patient_individualinfo.h \
    patientcase.h \
    patientprofile.h \
    schedule_choose.h \
    schedulechoose.h \
    seedoctorinfo.h \
    seepatientinfo.h \
    selectreceiver.h \
    signup.h \
    user.h \
    utils.h \
    widget.h

FORMS += \
    appointmentchoose.ui \
    chat.ui \
    department_choose.ui \
    departmentchoose.ui \
    doctor_choose.ui \
    doctor_individualinfo.ui \
    doctorcase.ui \
    doctorprofile.ui \
    healthtest.ui \
    healthtestoutcome.ui \
    patient_individualinfo.ui \
    patientcase.ui \
    patientprofile.ui \
    schedule_choose.ui \
    schedulechoose.ui \
    seedoctorinfo.ui \
    seepatientinfo.ui \
    selectreceiver.ui \
    signUp.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
