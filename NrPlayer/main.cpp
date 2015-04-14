#include <QCoreApplication>
#include "playercontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PlayerController controller;
    return a.exec();
}
