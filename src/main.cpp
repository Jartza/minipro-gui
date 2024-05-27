#include <QApplication>
#include <QTextEdit>
#include <QGridLayout>
#include <QProcess>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string process_exec(const char* cmd) {
    std::array<char, 512> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), &pclose);
    if (!pipe) {
        return "Failed to get process";
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char *argv[]) {
    QProcess minipro;
    minipro.start("minipro", QStringList() << "--version");
    if (!minipro.waitForStarted())
        return false;

    if (!minipro.waitForFinished())
        return false;

    QString initial_check_output = minipro.readAllStandardOutput();
    QString initial_check_error = minipro.readAllStandardError();

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    QApplication a(argc, argv);

    auto window = new QWidget;
    window->setMinimumSize(800, 600);

    auto *layout = new QGridLayout(window);

    auto *textEdit = new QTextEdit(initial_check_output + "\n\n" + initial_check_error, window);
//    textEdit->setReadOnly(true);

    layout->addWidget(textEdit, 0, 2);

    window->show();
    return QApplication::exec();
}