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
    connect(button_device, SIGNAL (released()),this, SLOT (select_device()));


    layout->addWidget(actions_label, 3, 0);
    layout->addWidget(button_blank, 4, 0);
    layout->addWidget(button_read, 5, 0);
    layout->addWidget(button_write, 6, 0);
    layout->addWidget(button_erase, 7, 0);
    layout->addWidget(button_update, 8, 0);

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

QString MainWindow::run_process(QPlainTextEdit &status_text, const QString &command_option, const QString &type = "stderr") {
    QProcess minipro;

    status_text.appendPlainText("[Input]: minipro " + command_option);
    minipro.start("minipro", QStringList() << command_option);
    QString error = "";
    QString output = "";
    if (!minipro.waitForStarted()){
        error = "Error";
    }

    if (!minipro.waitForFinished())
        error = "Error";

    if (type != "stderr"){
        output = error + minipro.readAllStandardOutput();
    }
    else {
        output = error + minipro.readAllStandardError();
        status_text.appendPlainText("[Output]: " + output);
    }

    return output;
}

void MainWindow::check_for_minipro(){
    QString initial_check_error = run_process(*status_view, "--version");
    if (initial_check_error.length() > 0) {
        if (initial_check_error.contains("minipro version")) {
            QRegularExpression re("minipro version.*\\n");
            QRegularExpressionMatch match = re.match(initial_check_error);
            if (match.hasMatch()) {
                window->setWindowTitle(match.captured(0));
                minipro_found = true;
            }
        }
        status_view->ensureCursorVisible();
    }
}

void MainWindow::check_for_programmer(){
    QRegularExpression re("(?<=: ).*$");
    QRegularExpressionMatch match = re.match(run_process(*status_view, "--presence_check").trimmed());
    if (match.hasMatch()) {
        button_programmer->setText(match.captured(0));
        programmer_found = true;
    }
}

void MainWindow::select_device(){

}

void MainWindow::update_firmware(){

}