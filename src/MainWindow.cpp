#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initializer();

  auto *groupBox = new QGroupBox(tr("Targets"));
  auto *vbox = new QVBoxLayout;
  vbox->addWidget(button_programmer);
  vbox->addWidget(button_device);
  connect(button_device, SIGNAL (currentTextChanged(QString)), this, SLOT (select_device(QString)));
  vbox->addStretch(1);
  groupBox->setLayout(vbox);
  layout->addWidget(groupBox, 0, 0);

  auto *groupBox3 = new QGroupBox(tr("Actions"));
  auto *vbox3 = new QVBoxLayout;
  vbox3->addWidget(button_blank);
  connect(button_blank, SIGNAL (released()), this, SLOT (check_blank()));
  vbox3->addWidget(button_read);
  connect(button_read, SIGNAL (released()), this, SLOT (read_device()));
  vbox3->addWidget(button_write);
  connect(button_write, SIGNAL (released()), this, SLOT (write_device()));
  vbox3->addWidget(button_erase);
  connect(button_erase, SIGNAL (released()), this, SLOT (erase_device()));
  vbox3->addWidget(button_update);
  connect(button_update, SIGNAL (released()), this, SLOT (update_firmware()));
  vbox3->addStretch(1);
  groupBox3->setLayout(vbox3);
  layout->addWidget(groupBox3, 2, 0);

  auto *groupBox4 = new QGroupBox(tr("Device Info"));
  auto *vbox4 = new QGridLayout;
  vbox4->addWidget(device_name_label, 0, 0);
  vbox4->addWidget(device_name, 0, 1);
  vbox4->addWidget(device_memory_label, 1, 0);
  vbox4->addWidget(device_memory, 1, 1);
  vbox4->addWidget(device_package_label, 2, 0);
  vbox4->addWidget(device_package, 2, 1);

  vbox4->addWidget(device_protocol_label, 0, 2);
  vbox4->addWidget(device_protocol, 0, 3);
  vbox4->addWidget(device_readbuffer_label, 1, 2);
  vbox4->addWidget(device_readbuffer, 1, 3);
  vbox4->addWidget(device_writebuffer_label, 2, 2);
  vbox4->addWidget(device_writebuffer, 2, 3);
//  vbox4->addWidget(device_other);
//  vbox4->addWidget(device_view);
//  vbox4->addStretch(1);
  groupBox4->setLayout(vbox4);
  layout->addWidget(groupBox4, 0, 1);

  auto *groupBox5 = new QGroupBox(tr("Hex Viewer"));
  auto *vbox5 = new QVBoxLayout;

  hexTableView->setModel(&hexViewModel);
  hexTableView->show();
  vbox5->addWidget(hexTableView);
  vbox5->addStretch(1);
  groupBox5->setLayout(vbox5);
  layout->addWidget(groupBox5, 2, 1);

  auto *groupBox6 = new QGroupBox(tr("Output"));
  auto *vbox6 = new QVBoxLayout;
  vbox6->addWidget(status_view);
  vbox6->addStretch(1);
  groupBox6->setLayout(vbox6);
  layout->addWidget(groupBox6, 3, 0, 1, 0);

//  device_view->setReadOnly(true);
  status_view->setReadOnly(true);

  disable_buttons();
  check_for_minipro();
  build_default_hex_output();

  window->setWindowIcon(QIcon(":/res/AppIcon.png"));
  window->show();
}

MainWindow::~MainWindow() = default;

void MainWindow::initializer() {
  window = new QWidget;
  layout = new QGridLayout(window);

  window->setWindowTitle("minipro CLI not found!");
  window->setMinimumSize(1000, 500);

  minipro_found = false;
  programmer_found = false;

  button_programmer = new QComboBox;

  button_blank = new QPushButton("Blank Check");
  button_write = new QPushButton("Write to Device");
  button_read = new QPushButton("Read from Device");
  button_erase = new QPushButton("Erase Device");
  button_update = new QPushButton("Update Firmware");

  button_device = new QComboBox();

//  device_view = new QPlainTextEdit(window);

  device_name_label = new QLabel("Name");
  device_name = new QLineEdit();
  device_name->setReadOnly(true);
//  device_name->setAlignment(Qt::AlignCenter);
  device_memory_label = new QLabel("Memory");
  device_memory = new QLineEdit();
  device_memory->setReadOnly(true);
  device_package_label = new QLabel("Package");
  device_package = new QLineEdit();
  device_package->setReadOnly(true);
  device_protocol_label = new QLabel("Protocol");
  device_protocol = new QLineEdit();
  device_protocol->setReadOnly(true);
  device_readbuffer_label = new QLabel("Read Buffer");
  device_readbuffer = new QLineEdit();
  device_readbuffer->setReadOnly(true);
  device_writebuffer_label = new QLabel("Write Buffer");
  device_writebuffer = new QLineEdit();
  device_writebuffer->setReadOnly(true);
//  device_other = new QLineEdit();


  hexTableView = new QTableView(window);
  status_view = new QPlainTextEdit(window);

  monospace_font.setFamily("Courier New");
  monospace_font.setStyleHint(QFont::Monospace);

//  device_view->setFont(monospace_font);
  status_view->setFont(monospace_font);
}

void MainWindow::run_async_process(const QStringList &process_arguments,
                                   const QString &type = "stderr") {
  async_process = new QProcess();
  QString process_arguments_string = "";
  for (auto const &each : process_arguments) {
    process_arguments_string += each + " ";
  }
  status_view->appendPlainText("[Input]: minipro " + process_arguments_string);
  if (type == "stderr") {
    connect(async_process, SIGNAL(readyReadStandardError()), this, SLOT(async_process_err_output()));
  } else if (type == "stdout") {
    connect(async_process, SIGNAL(readyReadStandardOutput()), this, SLOT(async_process_std_output()));
  }
  connect(async_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(read_device_output(int, QProcess::ExitStatus)));

  status_view->appendPlainText("[Output]: ");
  async_process->start("minipro", process_arguments);
}

void MainWindow::async_process_err_output() {
  status_view->appendPlainText(async_process->readAllStandardError().trimmed());
}

void MainWindow::async_process_std_output() {
  status_view->appendPlainText(async_process->readAllStandardOutput().trimmed());
}

QString MainWindow::run_process(QPlainTextEdit &target_plain_text_edit,
                                const QStringList &process_arguments,
                                const QString &type = "stderr") {
  auto *process = new QProcess();
  QString process_arguments_string = "";
  for (auto const &each : process_arguments) {
    process_arguments_string += each + " ";
  }
  target_plain_text_edit.appendPlainText("[Input]: minipro " + process_arguments_string);
  process->start("minipro", process_arguments);
  QString output = "";
  if (!process->waitForStarted()) {
    output += "Start Error";
  }
  if (!process->waitForFinished()) {
    output += "Finished Error";
  }
  if (type == "stderr") {
    output += process->readAllStandardError();
    QRegularExpression re(R"(Serial code:.*\n([\s\S]*))");
    if (QRegularExpressionMatch match = re.match(output); match.hasMatch()) {
      output = match.captured(1).trimmed();
    }
    target_plain_text_edit.appendPlainText("[Output]: " + output);
  } else if (type == "stdout") {
    output += process->readAllStandardOutput();
    QRegularExpression re(R"(Serial code:.*\n([\s\S]*))");
    if (QRegularExpressionMatch match = re.match(output); match.hasMatch()) {
      output = match.captured(1).trimmed();
    }
  }
  target_plain_text_edit.ensureCursorVisible();
  return output;
}

void MainWindow::check_for_minipro() {
  QStringList arguments;
  arguments << "--version";
  auto initial_check_error = run_process(*status_view, arguments);
  if (initial_check_error.length() > 0 && initial_check_error.contains("minipro version")) {
    QRegularExpression re("minipro version.*\\n");
    QRegularExpressionMatch match = re.match(initial_check_error);
    if (match.hasMatch()) {
      window->setWindowTitle(match.captured(0).trimmed());
      minipro_found = true;
      check_for_programmer();
      get_devices();
    } else {
      button_programmer->setDisabled(true);
    }
  }
}

void MainWindow::check_for_programmer() {
  QStringList arguments;
  arguments << "--presence_check";

  QRegularExpression re("(?<=: ).*$");
  QRegularExpressionMatch match = re.match(run_process(*status_view, arguments).trimmed());
  if (match.hasMatch()) {
    programmer = match.captured(0);
    programmer_found = true;
    button_programmer->addItem(programmer);
    enable_buttons();
  } else {
    button_programmer->setPlaceholderText("No programmer found");
    programmer_found = false;
    disable_buttons();
  }
}

void MainWindow::disable_buttons() {
  button_device->setDisabled(true);
  button_blank->setDisabled(true);
  button_read->setDisabled(true);
  button_write->setDisabled(true);
  button_erase->setDisabled(true);
  button_update->setDisabled(true);
}

void MainWindow::enable_buttons() {
  button_device->setDisabled(false);
  button_blank->setDisabled(false);
  button_read->setDisabled(false);
  button_write->setDisabled(false);
  button_erase->setDisabled(false);
  button_update->setDisabled(false);
}

void MainWindow::get_devices() {
  if (programmer_found) {
    QStringList arguments;
    arguments << "-l";

    devices_list.clear();
    devices_list << run_process(*status_view, arguments, "stdout").split("\n", Qt::SkipEmptyParts);
    devices_list.sort();
    devices_list.removeDuplicates();

    button_device->addItems(devices_list);
  }
}

void MainWindow::select_device(const QString &selected_device) {
  if (selected_device != "") {
    device = selected_device;
    QStringList arguments;
    arguments << "-d" << device;

    QRegularExpression re;
    re.setPatternOptions(QRegularExpression::MultilineOption);
    re.setPattern("Name:([\\s\\S]*)($)");
    QRegularExpressionMatch filter = re.match(run_process(*status_view, arguments));

    QRegularExpressionMatch match;
    if (filter.hasMatch()) {
      re.setPattern("(?<=Name: ).*$");
      match = re.match(filter.captured(0));
      device_name->setText(match.captured(0));
      re.setPattern("(?<=Memory: ).*$");
      match = re.match(filter.captured(0));
      device_memory->setText(match.captured(0));
      re.setPattern("(?<=Package: ).*$");
      match = re.match(filter.captured(0));
      device_package->setText(match.captured(0));
      re.setPattern("(?<=Protocol: ).*$");
      match = re.match(filter.captured(0));
      device_protocol->setText(match.captured(0));
      re.setPattern("(?<=Read buffer size: ).*$");
      match = re.match(filter.captured(0));
      device_readbuffer->setText(match.captured(0));
      re.setPattern("(?<=Write buffer size: ).*$");
      match = re.match(filter.captured(0));
      device_writebuffer->setText(match.captured(0));
      re.setPattern(R"((?<=\*\*\*\*).*$)");
      match = re.match(filter.captured(0));
      std::cout << match.captured(0).toStdString();

      build_default_hex_output();
    }
  }
}

void MainWindow::check_blank() const {
  QStringList arguments;
  arguments << "-p" << device << "-b";
  run_process(*status_view, arguments);
}

void MainWindow::read_device() {
  QStringList arguments;
  arguments << "-p" << device << "-r" << temp_file_name;
  run_async_process(arguments);
}

void MainWindow::read_device_output(int code, QProcess::ExitStatus) {
  hexViewModel.clearHexTable();
  if (code != 1) {
    build_formatted_hex_output();
  } else {
    build_default_hex_output();
  }
}

void MainWindow::write_device() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (fileName != "") {
    QStringList arguments;
    arguments << "-p" << device << "-w" << fileName;
    run_async_process(arguments);
  }
}

void MainWindow::erase_device() {
  QStringList arguments;
  arguments << "-p" << device << "-E";
  run_async_process(arguments);
}

void MainWindow::update_firmware() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (fileName != "") {
    QStringList arguments;
    arguments << "-p" << device << "-F" << fileName;
    run_async_process(arguments);
  }
}

void MainWindow::build_default_hex_output() {
  hexViewModel.clearHexTable();
  format_hex_table_columns();
}

void MainWindow::build_formatted_hex_output() {
  try {
    QFile temp_file(temp_file_name);
    temp_file.open(QFile::ReadOnly);
    QString temp_file_content = QString::fromUtf8(temp_file.readAll().toHex());
    temp_file.close();

    hexViewModel.buildHexTable(temp_file_content);
    format_hex_table_columns();
  }
  catch (const std::exception &e) {
    status_view->appendPlainText("\n[Error]: " + static_cast<QString>(e.what()));
  }
}

void MainWindow::format_hex_table_columns() const {
  hexTableView->horizontalHeader()->setStretchLastSection(true);
  for (int n = 0; n <= hexViewModel.columnCount(); n++) {
    hexTableView->resizeColumnToContents(n);
  }
  for (int n = 0; n <= hexViewModel.rowCount(); n++) {
    hexTableView->resizeRowToContents(n);
  }
}