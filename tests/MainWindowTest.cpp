#include "../src/MainWindow.h"
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

class MainWindowTest : public QObject {
 Q_OBJECT

 public:
  MainWindowTest();

 private slots:
  void initTestCase();
  void check_for_minipro_test();
  void build_default_hex_output_test();
  void build_formatted_hex_output_test();

 private:
  MainWindow *mainWindow;
};

MainWindowTest::MainWindowTest() {
  mainWindow = new MainWindow();
}

void MainWindowTest::initTestCase() {
  // Initialize any necessary resources here
}

void MainWindowTest::check_for_minipro_test() {
  // Mock the minipro command output
  mainWindow->check_for_minipro();
  // Check if minipro_found is set correctly
  QVERIFY(mainWindow->minipro_found);
}

void MainWindowTest::build_default_hex_output_test() {
  // Check if the default hex output is generated correctly
//  QString expectedOutput = "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n"
//                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   ................\n";
//  mainWindow->build_default_hex_output();
//  QCOMPARE(mainWindow->hex_view->toPlainText(), expectedOutput);
}

void MainWindowTest::build_formatted_hex_output_test() {
  // Mock the temp file content
  QFile tempFile("temp.bin");
  tempFile.open(QFile::ReadWrite);
  tempFile.write("ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789");
  tempFile.close();
}

QTEST_MAIN(MainWindowTest)
#include "MainWindowTest.moc"