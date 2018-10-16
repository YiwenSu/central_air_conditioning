#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "mainwindow.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "waitdialog.h"
#include <QIntValidator>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_button_clicked();
    void logon1();

private:
    Ui::Login *ui;
    MainWindow *mainwindow;
    void accept();
    int x;
    int y;
};

#endif // LOGIN_H
