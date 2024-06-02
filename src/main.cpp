#include <QApplication>
#include "MainWindow.h"
#include "Read.h"
#include "Write.h"
#include "Firmware.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QGuiApplication::setWindowIcon(QIcon(":/res/AppIcon.png"));
  QCoreApplication::setOrganizationName("Twelve Chairs");
  QCoreApplication::setOrganizationDomain("twelve-chairs");
  QCoreApplication::setApplicationName("minipro-gui");

  MainWindow window;

  return QApplication::exec();
}