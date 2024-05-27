#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QSizePolicy>
#include <QStatusBar>
#include <QProcess>
#include <QRegularExpression>

#include <memory>
#include <iostream>

// TODO: move to Class
const QString temp_device_name = "ST2764A@DIP28";
bool programmer_found = false;


QString run_process(QPlainTextEdit &status_text, const QString &command_option, const QString type = "stderr") {
    QProcess minipro;
    //    minipro.setProcessChannelMode(QProcess::ForwardedChannels);

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

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);

    auto window = new QWidget;
    window->setWindowTitle("minipro CLI not found!");
    window->setMinimumSize(800, 500);

    auto *layout = new QGridLayout(window);

    auto *targets_label = new QLabel("Targets");
    auto *actions_label = new QLabel("Actions");
    auto *status_text_label = new QLabel("Output");
    auto *device_View_label = new QLabel("Device Info");
    auto *hex_View_label = new QLabel("HEX Viewer");

    auto *button_programmer = new QPushButton("Select Programmer");
    auto *button_device = new QPushButton("Select Device");
    auto *button_blank = new QPushButton("Blank Check");
    auto *button_write = new QPushButton("Write to Device");
    auto *button_read = new QPushButton("Read from Device");
    auto *button_erase = new QPushButton("Erase Device");
    auto *button_update = new QPushButton("Update Firmware");

    auto *status_text = new QPlainTextEdit(window);
    auto *device_View = new QPlainTextEdit(window);
    auto *hex_View = new QPlainTextEdit(window);

    status_text->setFont(font);
    device_View->setFont(font);
    hex_View->setFont(font);

    status_text->setReadOnly(true);
    device_View->setReadOnly(true);
    hex_View->setReadOnly(true);

    QString initial_check_output = "";
    QString initial_check_error = run_process(*status_text, "--version");

    if (initial_check_error.length() > 0) {
        if (initial_check_error.contains("minipro version")) {
            QRegularExpression re("minipro version.*\\n");
            QRegularExpressionMatch match = re.match(initial_check_error);
            if (match.hasMatch()) {
                window->setWindowTitle(match.captured(0));
            }
        }
        status_text->ensureCursorVisible();
    }

    QRegularExpression re("(?<=: ).*$");
    QRegularExpressionMatch match = re.match(run_process(*status_text, "--presence_check").trimmed());
    if (match.hasMatch()) {
        button_programmer->setText(match.captured(0));
        programmer_found = true;
    }


    if (programmer_found) {
        device_View->setPlainText(run_process(*status_text, "-d " + temp_device_name));
        device_View->setPlainText(run_process(*status_text, "--list", "stdout"));
    }

    layout->addWidget(targets_label, 0, 0);
    layout->addWidget(button_programmer, 1, 0);
    layout->addWidget(button_device, 2, 0);

    layout->addWidget(actions_label, 3, 0);
    layout->addWidget(button_blank, 4, 0);
    layout->addWidget(button_read, 5, 0);
    layout->addWidget(button_write, 6, 0);
    layout->addWidget(button_erase, 7, 0);
    layout->addWidget(button_update, 8, 0);

    layout->addWidget(device_View_label, 0, 1);
    layout->addWidget(device_View, 1, 1, 2, 1);
    layout->addWidget(hex_View_label, 3, 1);
    layout->addWidget(hex_View, 4, 1, 5, 1);
    layout->addWidget(status_text_label, 9, 0);
    layout->addWidget(status_text, 10, 0, 1, 0);

    window->show();
    return QApplication::exec();
}