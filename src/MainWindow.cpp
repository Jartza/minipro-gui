#include "MainWindow.h"

void MainWindow::initializer(){
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

    device_view = new QPlainTextEdit(window);
    hex_view = new QPlainTextEdit(window);
    status_view = new QPlainTextEdit(window);

    monospace_font.setFamily("Courier New");
    monospace_font.setStyleHint(QFont::Monospace);

    device_view->setFont(monospace_font);
    hex_view->setFont(monospace_font);
    status_view->setFont(monospace_font);

//    get_programmers();
//    get_devices();
//    check_for_programmer();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    initializer();

    QGroupBox *groupBox = new QGroupBox(tr("Targets"));
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(button_programmer);
    connect(button_programmer, SIGNAL (currentTextChanged(QString)),this, SLOT (check_for_programmer(QString)));
    vbox->addWidget(button_device);
    connect(button_device, SIGNAL (currentTextChanged(QString)),this, SLOT (select_device(QString)));
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    layout->addWidget(groupBox, 0, 0);

    QGroupBox *groupBox3 = new QGroupBox(tr("Actions"));
    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(button_blank);
    connect(button_blank, SIGNAL (released()),this, SLOT (check_blank()));
    vbox3->addWidget(button_read);
    connect(button_read, SIGNAL (released()),this, SLOT (read_device()));
    vbox3->addWidget(button_write);
    connect(button_write, SIGNAL (released()),this, SLOT (write_device()));
    vbox3->addWidget(button_erase);
    connect(button_erase, SIGNAL (released()),this, SLOT (erase_device()));
    vbox3->addWidget(button_update);
    connect(button_update, SIGNAL (released()),this, SLOT (update_firmware()));
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
    layout->addWidget(groupBox6, 3, 0, 1 , 0);

    device_view->setReadOnly(true);
    hex_view->setReadOnly(true);
    status_view->setReadOnly(true);

    window->setWindowIcon(QIcon(":/res/AppIcon.png"));

    window->show();

    check_for_minipro();
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

    QRegularExpression re(R"(Serial code:.*\n([\s\S]*))");
    QRegularExpressionMatch match = re.match(output);
    if (match.hasMatch()) {
        output = match.captured(1);
    }
    status_text.appendPlainText("[Output]: " + output);
    status_text.ensureCursorVisible();
    return output;
}

void MainWindow::check_for_minipro(){
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
            get_programmers();
            check_for_programmer();
            get_devices();
        }
    }
}

void MainWindow::get_programmers(){
    QStringList arguments;
    arguments << "--query_supported";

    programmers_list.clear();
    programmers_list << "No programmer";
    programmers_list << run_process(*status_view, arguments, "stderr").split("\n", Qt::SkipEmptyParts);;

    button_programmer->addItems(programmers_list);
}

void MainWindow::check_for_programmer(const QString& selected_programmer){
    QStringList arguments;
    arguments << "--presence_check";

    QRegularExpression re("(?<=: ).*$");
    QRegularExpressionMatch match = re.match(run_process(*status_view, arguments).trimmed());
    if (match.hasMatch()) {
        programmer = match.captured(0);
        programmer_found = true;
        button_programmer->setCurrentText(selected_programmer);
        button_device->setDisabled(false);
        button_blank->setDisabled(false);
        button_read->setDisabled(false);
        button_write->setDisabled(false);
        button_erase->setDisabled(false);
        button_update->setDisabled(false);
    }
    else {
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

void MainWindow::get_devices(){
    if (programmer_found) {
        QStringList arguments;
        arguments << "-l";

        devices_list.clear();
        devices_list << run_process(*status_view, arguments, "stdout").split("\n", Qt::SkipEmptyParts);;
        devices_list.sort();

        button_device->addItems(devices_list);
    }
}

void MainWindow::select_device(const QString& selected_device){
    if (selected_device != ""){
        device = selected_device;
        QStringList arguments;
        arguments << "-d" << device;

        QRegularExpression re("Name:([\\s\\S]*)($)");
        QRegularExpressionMatch match = re.match(run_process(*device_view, arguments));
        if (match.hasMatch()) {
            device_view->setPlainText(match.captured(0));
            hex_view->clear();
        }
    }
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
    if (!run_process(*status_view, arguments).contains("Unsupported device")
        && !run_process(*status_view, arguments).contains("Invalid Chip ID")) {
        QFile f("temp.bin");
        f.open(QFile::ReadOnly);
        hex_view->setPlainText(QString::fromUtf8(f.readAll().toHex()));
    }
    else {
        hex_view->clear();
    }
}

void MainWindow::write_device(){

}

void MainWindow::erase_device(){
    QStringList arguments;
    arguments << "-p" << device << "-E";
    run_process(*status_view, arguments);
}

void MainWindow::update_firmware(){
    QString fileName = QFileDialog::getOpenFileName(this);
    QStringList arguments;
    arguments << "-p" << device << "-F" << fileName;
    run_process(*status_view, arguments);
}