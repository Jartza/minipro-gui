#include <QApplication>
#include <QTextEdit>

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
//    QString temp1 = QString::fromStdString(process_exec("minipro -k"));
    QString temp2 = QString::fromStdString(process_exec("man minipro"));

    QApplication a(argc, argv);

    QTextEdit textEdit;
    textEdit.setReadOnly(true);
    textEdit.setText(temp2);
    textEdit.show();

    return QApplication::exec();
}