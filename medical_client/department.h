#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <QWidget>

namespace Ui {
class department;
}

class department : public QWidget
{
    Q_OBJECT

public:
    explicit department(QWidget *parent = nullptr);
    ~department();

private:
    Ui::department *ui;
};

#endif // DEPARTMENT_H
