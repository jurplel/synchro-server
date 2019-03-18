#include <QCoreApplication>
#include "server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server srv;

    return a.exec();
}
