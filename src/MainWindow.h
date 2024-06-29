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
#include <QSet>
#include <QTableView>
#include <QListWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QHeaderView>
#include <iostream>
#include <array>
#include "HexView.h"

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
  QComboBox *vpp;
  QComboBox *vdd;
  QComboBox *vcc;

  QCheckBox *no_id_error;
  QCheckBox *skip_id;
  QCheckBox *no_size_error;
  QCheckBox *skip_verify;
  QCheckBox *pin_check;
  QCheckBox *blank_check;
  QCheckBox *erase_device;
  QCheckBox *hardware_check;

  QPlainTextEdit *status_view;

  QLabel *device_name_label;
  QLineEdit *device_name;
  QLabel *device_memory_label;
  QLineEdit *device_memory;
  QLabel *device_package_label;
  QLineEdit *device_package;
  QLabel *device_protocol_label;
  QLineEdit *device_protocol;
  QLabel *device_readbuffer_label;
  QLineEdit *device_readbuffer;
  QLabel *device_writebuffer_label;
  QLineEdit *device_writebuffer;
  QLineEdit *device_other;

  QTableView *hexTableView;
  HexView hexViewModel;

  QStringList programmers_list;
  QStringList devices_list;

  QProcess *async_process;

  static QString run_process(QPlainTextEdit &, const QStringList &, const QString &);
  void run_async_process(const QStringList &, const QString &);

  void build_default_hex_output();
  void build_formatted_hex_output();
  void check_for_minipro();

  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

 private slots:
  void check_for_programmer();
  void get_devices();
  void select_device(const QString & = "");
  void async_process_err_output();
  void async_process_std_output();
  void read_device_output(int, QProcess::ExitStatus);

  void run_command();
  void read_device();
  void write_device();
  void update_firmware();

 private:
  QWidget *window;

  QGridLayout *layout;

  QFont monospace_font;

  const QString temp_file_name = "temp.bin";

  QPushButton *button_run_command;
  QPushButton *button_write;
  QPushButton *button_read;
  QPushButton *button_update;

  void initializer();
  void format_hex_table_columns() const;
  void disable_buttons();
  void enable_buttons();

  QStringList parse_checkboxes();

};

#endif //MINIPRO_GUI_MAINWINDOW_H
