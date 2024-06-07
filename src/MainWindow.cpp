#include "MainWindow.h"

void MainWindow::initializer() {
  window = new QWidget;
  layout = new QGridLayout(window);

  window->setWindowTitle("minipro CLI not found!");
  window->setMinimumSize(900, 380);
  statusBar()->showMessage(tr("minipro-gui v0.1"));

  minipro_found = false;
  programmer_found = false;

  button_programmer = new QComboBox;

  button_blank = new QPushButton("Blank Check");
  button_write = new QPushButton("Write to Device");
  button_read = new QPushButton("Read from Device");
  button_erase = new QPushButton("Erase Device");
  button_update = new QPushButton("Update Firmware");

  button_device = new QComboBox();

  device_view = new QPlainTextEdit(window);
  hex_view = new QPlainTextEdit(window);
  status_view = new QPlainTextEdit(window);

  monospace_font.setFamily("Courier New");
  monospace_font.setStyleHint(QFont::Monospace);

  device_view->setFont(monospace_font);
  hex_view->setFont(monospace_font);
  status_view->setFont(monospace_font);

  build_default_hex_output();
}

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
  auto *vbox4 = new QVBoxLayout;
  vbox4->addWidget(device_view);
  vbox4->addStretch(1);
  groupBox4->setLayout(vbox4);
  layout->addWidget(groupBox4, 0, 1);

  auto *groupBox5 = new QGroupBox(tr("Hex Viewer"));
  auto *vbox5 = new QVBoxLayout;
  vbox5->addWidget(hex_view);
  vbox5->addStretch(1);
  groupBox5->setLayout(vbox5);
  layout->addWidget(groupBox5, 2, 1);

  auto *groupBox6 = new QGroupBox(tr("Output"));
  auto *vbox6 = new QVBoxLayout;
  vbox6->addWidget(status_view);
  vbox6->addStretch(1);
  groupBox6->setLayout(vbox6);
  layout->addWidget(groupBox6, 3, 0, 1, 0);

  device_view->setReadOnly(true);
  hex_view->setReadOnly(true);
  status_view->setReadOnly(true);

  window->setWindowIcon(QIcon(":/res/AppIcon.png"));

  window->show();

  check_for_minipro();
}

MainWindow::~MainWindow() = default;

QString MainWindow::run_process(QPlainTextEdit &target_plain_text_edit,
                                const QStringList &process_arguments,
                                const QString &type = "stderr") {
  QProcess process;
  QString process_arguments_string = "";
  for (auto const &each : process_arguments) {
    process_arguments_string += each + " ";
  }
  target_plain_text_edit.appendPlainText("[Input]: minipro " + process_arguments_string);
  process.start("minipro", process_arguments);
  QString output = "";
  if (!process.waitForStarted()) {
    output += "Start Error";
  }
  if (!process.waitForFinished()) {
    output += "Finished Error";
  }
  if (type == "stderr") {
    output += process.readAllStandardError();
    QRegularExpression re(R"(Serial code:.*\n([\s\S]*))");
    if (QRegularExpressionMatch match = re.match(output); match.hasMatch()) {
      output = match.captured(1);
    }
    target_plain_text_edit.appendPlainText("[Output]: " + output);
  } else if (type == "stdout") {
    output += process.readAllStandardOutput();
    QRegularExpression re(R"(Serial code:.*\n([\s\S]*))");
    if (QRegularExpressionMatch match = re.match(output); match.hasMatch()) {
      output = match.captured(1);
    }
  }
  target_plain_text_edit.ensureCursorVisible();
  return output;
}

void MainWindow::check_for_minipro() {
  button_programmer->setDisabled(true);
  QStringList arguments;
  arguments << "--version";
  QString initial_check_error = run_process(*status_view, arguments);
  if (initial_check_error.length() > 0 && initial_check_error.contains("minipro version")) {
    QRegularExpression re("minipro version.*\\n");
    QRegularExpressionMatch match = re.match(initial_check_error);
    if (match.hasMatch()) {
      window->setWindowTitle(match.captured(0).trimmed());
      minipro_found = true;
      button_programmer->setDisabled(false);
      check_for_programmer();
      get_devices();
    }
  }
}

void MainWindow::check_for_programmer(const QString &selected_programmer) {
  QStringList arguments;
  arguments << "--presence_check";

  QRegularExpression re("(?<=: ).*$");
  QRegularExpressionMatch match = re.match(run_process(*status_view, arguments).trimmed());
  if (match.hasMatch()) {
    programmer = match.captured(0);
    programmer_found = true;
    button_programmer->addItem(programmer);
    button_programmer->setDisabled(true);
    button_device->setDisabled(false);
    button_blank->setDisabled(false);
    button_read->setDisabled(false);
    button_write->setDisabled(false);
    button_erase->setDisabled(false);
    button_update->setDisabled(false);
  } else {
    button_programmer->setPlaceholderText("No programmer found");
    button_programmer->setDisabled(true);
    programmer_found = false;
    button_device->setDisabled(true);
    button_blank->setDisabled(true);
    button_read->setDisabled(true);
    button_write->setDisabled(true);
    button_erase->setDisabled(true);
    button_update->setDisabled(true);
  }
}

void MainWindow::get_devices() {
  if (programmer_found) {
    QStringList arguments;
    arguments << "-l";

    devices_list.clear();
    devices_list << run_process(*status_view, arguments, "stdout").split("\n", Qt::SkipEmptyParts);;
    devices_list.sort();

    button_device->addItems(devices_list);
  }
}

void MainWindow::select_device(const QString &selected_device) {
  if (selected_device != "") {
    device = selected_device;
    QStringList arguments;
    arguments << "-d" << device;

    QRegularExpression re("Name:([\\s\\S]*)($)");
    QRegularExpressionMatch match = re.match(run_process(*device_view, arguments));
    if (match.hasMatch()) {
      device_view->setPlainText(match.captured(0));
      build_default_hex_output();
    }
  }
}

void MainWindow::check_blank() const {
  QStringList arguments;
  arguments << "-p" << device << "-b";
  run_process(*status_view, arguments);
}

void MainWindow::read_device() const {
  hex_view->clear();
  QStringList arguments;
  arguments << "-p" << device << "-r" << temp_file_name;

  if (!run_process(*status_view, arguments).contains("Unsupported device")
      && !run_process(*status_view, arguments).contains("Invalid Chip ID")) {
    hex_view->setPlainText(build_formatted_hex_output());
  } else {
    build_default_hex_output();
  }
}

void MainWindow::write_device() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (fileName != "") {
    QStringList arguments;
    arguments << "-p" << device << "-w" << fileName;
    run_process(*status_view, arguments);
  }
}

void MainWindow::erase_device() const {
  QStringList arguments;
  arguments << "-p" << device << "-E";
  run_process(*status_view, arguments);
}

void MainWindow::update_firmware() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (fileName != "") {
    QStringList arguments;
    arguments << "-p" << device << "-F" << fileName;
    run_process(*status_view, arguments);
  }
}

void MainWindow::build_default_hex_output() const {
  hex_view->clear();
  QString blank_hex;
  for (int n = 0; n < 16; n++) {
    blank_hex += "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ................\n";
  }
  hex_view->setPlainText(blank_hex);
}

QString MainWindow::build_formatted_hex_output() const {
  try {
    QFile temp_file(temp_file_name);
    temp_file.open(QFile::ReadOnly);
    QString temp_file_content = QString::fromUtf8(temp_file.readAll().toHex());

    QString formatted_hex_output;
    QString ascii_string_line;

    const int chars_per_line = 32; // (16 bytes * 2)
    int line_counter = 0;

    for (int i = 0; i <= temp_file_content.length(); i++) {
      // Format hex values, and convert to ascii
      if (i % 2 != 0) {
        QString byte_string = QChar(temp_file_content[i - 1]);
        byte_string += QChar(temp_file_content[i]);

        formatted_hex_output += (byte_string.toUpper() += " ");

        auto ascii_int = byte_string.toUInt(nullptr, 16);
        // Non-extended ASCII only
        if (ascii_int > 126) ascii_int = 46;
        auto ascii_char = QChar(ascii_int);
        // Printable characters only
        if (!ascii_char.isPrint() || ascii_char.isNull()) {
          ascii_char = '.';
        }
        ascii_string_line += ascii_char;
      }
      // Next 16-byte line
      if (i != 0 && i % chars_per_line == 0) {
        formatted_hex_output += "  " + ascii_string_line + "\n";
        ascii_string_line.clear();
        line_counter++;
      }
    }
    return formatted_hex_output;
  }
  catch (const std::exception &e) {
    status_view->appendPlainText("\n[Error]: " + static_cast<QString>(e.what()));
    return default_hex_output;
  }
}