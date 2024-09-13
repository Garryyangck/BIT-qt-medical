#include "widget.h"

#include <QApplication>
#include <QDebug>

#include "dbutils.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();




    return 0;
}
