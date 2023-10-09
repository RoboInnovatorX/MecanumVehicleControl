#include <QCoreApplication>
#include "Qt_Server.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Qt_Server myserver;

    return a.exec();
}
