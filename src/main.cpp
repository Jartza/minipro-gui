#include <QApplication>
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
const QString temp_device_name = "M2764A@DIP28";


QString run_process(QPlainTextEdit &status_text, const QString& command_option){
    QProcess minipro;
    minipro.start("minipro", QStringList() << command_option);
    QString error = "";
    QString output = "";
    if (!minipro.waitForStarted())
        error = "Error";

    if (!minipro.waitForFinished())
        error = "Error";

    output = minipro.readAllStandardError();

    status_text.appendPlainText("Input: minipro " + command_option);
    status_text.appendPlainText("\nOutput:");
    status_text.appendPlainText(output);
    if (!error.isEmpty()){
        status_text.appendPlainText("Error: " + error);
    }

    return output;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    auto window = new QWidget;
    window->setWindowTitle("minipro CLI not found!");
    window->setMinimumSize(800, 500);

    auto *layout = new QGridLayout(window);
    auto *status_text = new QPlainTextEdit(window);
    auto *device_View = new QPlainTextEdit("Device Info", window);
    auto *hex_View = new QPlainTextEdit("Hex Viewer", window);
    auto *button_programmer = new QPushButton("Select Programmer");
    auto *button_device = new QPushButton("Select Device");
    auto *button_blank = new QPushButton("Blank Check");
    auto *button_write = new QPushButton("Write to Device");
    auto *button_read = new QPushButton("Read from Device");
    auto *button_erase = new QPushButton("Erase Device");
    auto *button_update = new QPushButton("Update Firmware");

    status_text->setReadOnly(true);
    device_View->setReadOnly(true);
    hex_View->setReadOnly(true);

    QString initial_check_output = "";
    QString initial_check_error = run_process(*status_text, "--version");

    if (initial_check_error.length() > 0) {
        if (initial_check_error.contains("minipro version")){
            QRegularExpression re("minipro version.*\\n");
            QRegularExpressionMatch match = re.match(initial_check_error);
            if (match.hasMatch()) {
                window->setWindowTitle(match.captured(0));
            }
        }
        status_text->appendPlainText(initial_check_error);
        status_text->ensureCursorVisible();
    }

    button_programmer->setText(run_process(*status_text, "--presence_check").trimmed());

    button_device->setText(temp_device_name);

    device_View->setPlainText(run_process(*status_text, "-d " + temp_device_name));

    layout->addWidget(button_programmer, 0, 0);
    layout->addWidget(button_device, 1, 0);

    layout->addWidget(button_blank, 3, 0);
    layout->addWidget(button_read, 4, 0);
    layout->addWidget(button_write, 5, 0);
    layout->addWidget(button_erase, 6, 0);
    layout->addWidget(button_update, 7, 0);
    layout->addWidget(device_View, 0, 1, 2, 1);
    layout->addWidget(hex_View, 3, 1, 5, 1);
    layout->addWidget(status_text, 9, 0, 1, 0);

    window->show();
    return QApplication::exec();
}