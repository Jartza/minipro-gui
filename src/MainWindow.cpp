#include "MainWindow.h"

void MainWindow::initializer(){
    window = new QWidget;
    layout = new QGridLayout(window);

    window->setWindowTitle("minipro CLI not found!");
    window->setMinimumSize(800, 500);

    minipro_found = false;
    programmer_found = false;

    targets_label = new QLabel("Targets");
    actions_label = new QLabel("Actions");
    device_view_label = new QLabel("Device Info");
    hex_view_label = new QLabel("HEX Viewer");
    status_view_label = new QLabel("Output");

    button_programmer = new QPushButton("No programmer found");
    button_device = new QPushButton("Select Device");
    button_blank = new QPushButton("Blank Check");
    button_write = new QPushButton("Write to Device");
    button_read = new QPushButton("Read from Device");
    button_erase = new QPushButton("Erase Device");
    button_update = new QPushButton("Update Firmware");

    device_view = new QPlainTextEdit(window);
    hex_view = new QPlainTextEdit(window);
    status_view = new QPlainTextEdit(window);

    monospace_font.setFamily("Courier New");
    monospace_font.setStyleHint(QFont::Monospace);

    device_view->setFont(monospace_font);
    hex_view->setFont(monospace_font);
    status_view->setFont(monospace_font);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    initializer();

    layout->addWidget(targets_label, 0, 0);
    layout->addWidget(button_programmer, 1, 0);
    connect(button_programmer, SIGNAL (released()),this, SLOT (check_for_programmer()));
    layout->addWidget(button_device, 2, 0);
    connect(button_device, SIGNAL (released()),this, SLOT (get_devices()));

    layout->addWidget(actions_label, 3, 0);
    layout->addWidget(button_blank, 4, 0);
    connect(button_blank, SIGNAL (released()),this, SLOT (check_blank()));
    layout->addWidget(button_read, 5, 0);
    connect(button_read, SIGNAL (released()),this, SLOT (read_device()));
    layout->addWidget(button_write, 6, 0);
    connect(button_write, SIGNAL (released()),this, SLOT (write_device()));
    layout->addWidget(button_erase, 7, 0);
    connect(button_erase, SIGNAL (released()),this, SLOT (erase_device()));
    layout->addWidget(button_update, 8, 0);
    connect(button_update, SIGNAL (released()),this, SLOT (update_firmware()));

    layout->addWidget(device_view_label, 0, 1);
    layout->addWidget(device_view, 1, 1, 2, 1);
    layout->addWidget(hex_view_label, 3, 1);
    layout->addWidget(hex_view, 4, 1, 5, 1);
    layout->addWidget(status_view_label, 9, 0);
    layout->addWidget(status_view, 10, 0, 1, 0);

    device_view->setReadOnly(true);
    hex_view->setReadOnly(true);
    status_view->setReadOnly(true);

    window->setWindowIcon(QIcon(":/res/AppIcon.png"));

    window->show();

    check_for_minipro();
    check_for_programmer();
}

MainWindow::~MainWindow() {

}

QString MainWindow::run_process(QPlainTextEdit &status_text, const QStringList &arguments, const QString &type = "stderr") {
    QProcess minipro;
    QString command_options = "";
    for (auto const& each : arguments){
        command_options += each + " ";
    }
    status_text.appendPlainText("[Input]: minipro " + command_options);
    minipro.start("minipro", arguments);
    QString output = "";
    if (!minipro.waitForStarted()) {
        output += "Start Error";
    }
    if (!minipro.waitForFinished()) {
        output += "Finished Error";
    }
    if (type == "stderr"){
        output += minipro.readAllStandardError();
    }
    else if (type == "stdout"){
        output += minipro.readAllStandardOutput();
    }

    status_text.appendPlainText("[Output]: " + output);
    status_text.ensureCursorVisible();
    return output;
}

void MainWindow::check_for_minipro(){
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
            }
            else {
                button_programmer->setDisabled(true);
            }
    }
}

void MainWindow::check_for_programmer(){
    QStringList arguments;
    arguments << "--presence_check";
    QRegularExpression re("(?<=: ).*$");
    QRegularExpressionMatch match = re.match(run_process(*status_view, arguments).trimmed());
    if (match.hasMatch()) {
        programmer = match.captured(0);
        programmer_found = true;
        button_programmer->setText(programmer);
        button_device->setDisabled(false);
        button_blank->setDisabled(false);
        button_read->setDisabled(false);
        button_write->setDisabled(false);
        button_erase->setDisabled(false);
        button_update->setDisabled(false);

    }
    else {
        button_programmer->setText("No programmer found");
        button_device->setDisabled(true);
        button_blank->setDisabled(true);
        button_read->setDisabled(true);
        button_write->setDisabled(true);
        button_erase->setDisabled(true);
        button_update->setDisabled(true);
    }
}

void MainWindow::get_devices(){
    QListWidget *listWidget = new QListWidget(this);

    QStringList arguments;
    arguments << "-l";

    QStringList devices_list = run_process(*status_view, arguments, "stdout").split("\n", Qt::SkipEmptyParts);;
    listWidget->addItems(devices_list);

    devices_layout = new QGridLayout();
    devices_layout->addWidget(listWidget);

    device_selector.setLayout(devices_layout);
    device_selector.setModal(true);
    device_selector.show();
    select_device();
}

void MainWindow::select_device(){
    device = "M2732@DIP24";
    QStringList arguments;
    arguments << "-d" << device;
    run_process(*device_view, arguments);

    button_device->setText(device);
}

void MainWindow::check_blank(){
    read_device();
    QStringList arguments;
    arguments << "-p" << device << "-b";
    run_process(*status_view, arguments);
}

void MainWindow::read_device(){
    QStringList arguments;
    arguments << "-p" << device << "-r" << "temp.bin";
    run_process(*status_view, arguments);
    QFile f("temp.bin");
    f.open(QFile::ReadOnly);
    hex_view->setPlainText(QString::fromUtf8(f.readAll().toHex()));
}

void MainWindow::write_device(){

}

void MainWindow::erase_device(){
    QStringList arguments;
    arguments << "-p" << device << "-E";
    run_process(*status_view, arguments);
}

void MainWindow::update_firmware(){
    QStringList arguments;
    arguments << "-p" << device << "-F";
    run_process(*status_view, arguments);
}