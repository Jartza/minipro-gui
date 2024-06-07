#ifndef MINIPRO_GUI_MAINWINDOW_H
#define MINIPRO_GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QProcess>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QRegularExpression>
#include <QDialog>
#include <QFileDialog>
#include <QtDebug>
#include <QFile>
#include <QListWidget>
#include <QGroupBox>
#include <QComboBox>
#include <iostream>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  bool minipro_found;
  bool programmer_found;

  QString programmer;
  QString device;

  QDialog device_selector;
  QComboBox *button_programmer;
  QComboBox *button_device;

  QPlainTextEdit *status_view;
  QPlainTextEdit *device_view;
  QPlainTextEdit *hex_view;

  QStringList programmers_list;
  QStringList devices_list;

  static QString run_process(QPlainTextEdit &, const QStringList &, const QString &);
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

 private slots:
  void check_for_programmer(const QString & = "");
  void get_devices();
  void select_device(const QString & = "");

  void check_blank() const;
  void read_device() const;
  void write_device();
  void erase_device() const;
  void update_firmware();

 private:
  QWidget *window;

  QGridLayout *layout;
  QGridLayout *devices_layout;

  QFont monospace_font;

  const QString temp_file_name = "temp.bin";
  const QString default_hex_output = "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................";

  QPushButton *button_blank;
  QPushButton *button_write;
  QPushButton *button_read;
  QPushButton *button_erase;
  QPushButton *button_update;

  void initializer();
  void check_for_minipro();
  QString build_formatted_hex_output() const;
  void build_default_hex_output() const;
};

#endif //MINIPRO_GUI_MAINWINDOW_H
