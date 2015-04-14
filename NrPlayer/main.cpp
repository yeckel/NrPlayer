#include <QGuiApplication>
#include "playercontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LTSoft");
    QCoreApplication::setOrganizationDomain("tomsik.eu");
    QCoreApplication::setApplicationName("NuvelarRbPiPlayer");

    PlayerController controller;
    return a.exec();
}
