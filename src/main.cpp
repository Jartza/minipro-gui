#include <QApplication>
#include "Devices.h"
#include "MainWindow.h"
#include "Programmers.h"
#include "Write.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Twelve Chairs");
    QCoreApplication::setOrganizationDomain("twelve-chairs");
    QCoreApplication::setApplicationName("minipro-gui");

    MainWindow window;

    window.show();
    return QApplication::exec();
}