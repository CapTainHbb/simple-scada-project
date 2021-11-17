#include "server.h"
#include <QCoreApplication>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server srv;
    return a.exec();
}
