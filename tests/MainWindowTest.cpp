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
  void check_for_programmer_test();
  void get_devices_test();
  void select_device_test();
  void check_blank_test();
  void read_device_test();
  void write_device_test();
  void erase_device_test();
  void update_firmware_test();
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
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "--version", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro --version"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: minipro version"));

  // Check if minipro_found is set correctly
  QVERIFY(mainWindow->minipro_found);
}

void MainWindowTest::check_for_programmer_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "--presence_check", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro --presence_check"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));

  // Check if programmer_found is set correctly
  QVERIFY(mainWindow->programmer_found);
  QCOMPARE(mainWindow->programmer, QString("Mock Programmer"));
}

void MainWindowTest::get_devices_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-l", "stdout");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -l"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));

  // Check if devices_list is populated correctly
  QVERIFY(!mainWindow->devices_list.isEmpty());
}

void MainWindowTest::select_device_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->device_view, SIGNAL(setPlainText(QString)));
  mainWindow->run_process(*mainWindow->device_view, QStringList() << "-d" << "Mock Device", "stderr");
  QCOMPARE(spy.count(), 1);
  QVERIFY(spy.at(0).at(0).toString().contains("Name: Mock Device"));

  // Check if device is set correctly
  QCOMPARE(mainWindow->device, QString("Mock Device"));
}

void MainWindowTest::check_blank_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-p" << "Mock Device" << "-b", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -p Mock Device -b"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));
}

void MainWindowTest::read_device_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-p" << "Mock Device" << "-r" << "Mock File", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -p Mock Device -r Mock File"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));

  // Check if hex_view is populated correctly
  QVERIFY(!mainWindow->hex_view->toPlainText().isEmpty());
}

void MainWindowTest::write_device_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-p" << "Mock Device" << "-w" << "Mock File", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -p Mock Device -w Mock File"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));
}

void MainWindowTest::erase_device_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-p" << "Mock Device" << "-E", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -p Mock Device -E"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));
}

void MainWindowTest::update_firmware_test() {
  // Mock the minipro command output
  QSignalSpy spy(mainWindow->status_view, SIGNAL(appendPlainText(QString)));
  mainWindow->run_process(*mainWindow->status_view, QStringList() << "-p" << "Mock Device" << "-F" << "Mock File", "stderr");
  QCOMPARE(spy.count(), 2);
  QVERIFY(spy.at(0).at(0).toString().contains("[Input]: minipro -p Mock Device -F Mock File"));
  QVERIFY(spy.at(1).at(0).toString().contains("[Output]: "));
}

void MainWindowTest::build_default_hex_output_test() {
  // Check if the default hex output is generated correctly
  QString expectedOutput = "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n"
                           "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n";
  mainWindow->build_default_hex_output();
  QCOMPARE(mainWindow->hex_view->toPlainText(), expectedOutput);
}

void MainWindowTest::build_formatted_hex_output_test() {
  // Mock the temp file content
  QFile tempFile("minipro-gui.app/Contents/MacOS/temp.bin");
  tempFile.open(QFile::WriteOnly);
  tempFile.write("ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789");
  tempFile.close();

  // Check if the formatted hex output is generated correctly
  QString expectedOutput = "41 42 43 44 45 46 30 31 32 33 34 35 36 37 38 39   ABCDEF0123456789\n"
                           "41 42 43 44 45 46 30 31 32 33 34 35 36 37 38 39   ABCDEF0123456789\n"
                           "41 42 43 44 45 46 30 31 32 33 34 35 36 37 38 39   ABCDEF0123456789\n"
                           "41 42 43 44 45 46 30 31 32 33 34 35 36 37 38 39   ABCDEF0123456789\n";
  QCOMPARE(mainWindow->build_formatted_hex_output(), expectedOutput);
}

QTEST_MAIN(MainWindowTest)
#include "MainWindowTest.moc"