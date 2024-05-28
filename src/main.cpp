#include <QApplication>
#include "MainWindow.h"
#include "Programmers.h"
#include "Devices.h"
#include "Read.h"
#include "Write.h"
#include "Firmware.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Twelve Chairs");
    QCoreApplication::setOrganizationDomain("twelve-chairs");
    QCoreApplication::setApplicationName("minipro-gui");

    MainWindow window;

    window.show();
    return QApplication::exec();
}