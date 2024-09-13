#include "department.h"
#include "ui_department.h"

department::department(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::department)
{
    ui->setupUi(this);
}

department::~department()
{
    delete ui;
}
