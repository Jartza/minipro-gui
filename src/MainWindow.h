#ifndef MINIPRO_GUI_MAINWINDOW_H
#define MINIPRO_GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QRegularExpression>
#include <QDialog>
#include <iostream>
#include <QtDebug>
#include <QFile>


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    bool minipro_found;
    bool programmer_found;

    QString programmer;
    QString device;

    QPushButton *button_programmer;
    QPushButton *button_device;

    QPlainTextEdit *status_view;
    QPlainTextEdit *device_view;
    QPlainTextEdit *hex_view;

    static QString run_process(QPlainTextEdit&, const QStringList&, const QString&);
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void check_for_programmer();
    void get_devices();
    void select_device();
    void check_blank();
    void read_device();
    void write_device();
    void erase_device();
    void update_firmware();

private:
    QWidget *window;

    QGridLayout *layout;

    QFont monospace_font;

    QLabel *targets_label;
    QLabel *actions_label;
    QLabel *status_view_label;
    QLabel *device_view_label;
    QLabel *hex_view_label;

    QPushButton *button_blank;
    QPushButton *button_write;
    QPushButton *button_read;
    QPushButton *button_erase;
    QPushButton *button_update;

    void initializer();
    void check_for_minipro();

};


#endif //MINIPRO_GUI_MAINWINDOW_H
