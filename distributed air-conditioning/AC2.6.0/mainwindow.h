#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ac.h"
#include "accom.h"
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include "workthread.h"
#include <QMovie>
#include <QGraphicsOpacityEffect>
#define IP "127.0.0.1"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void word_close();
    void myaclogin(int roomnumber,int userid);
    void myacrun();
    bool getmyacflag();
    AC* getMyac();
    void setRoomID(int roomid);
    void setUseriD(int userid);

private slots:
    void on_turnonBtn_clicked();

    void on_speedBtn_clicked();

    void on_modeBtn_clicked();

    void on_IDBtn_clicked();

    void on_turnonBtn_2_clicked();

    void on_blue_clicked();

    void on_temup_clicked();

    void on_temdown_clicked();

    void on_orange_clicked();

    void updatetime();

    void tem_change1();

    void speed0();

    void speedback0(int oldspeed);

    void slot_setroomtmp();

    void on_temup_toggled(bool checked);

    void slot_setrealspeed();

    void slot_secondturnon();

    void on_pink_clicked();

private:
    Ui::MainWindow *ui;
    AC *myac;
    QThread *thread1;
    QDateTime *datatime;
    int roomID;
    int userID;
    int opencount = 1;
    QGraphicsOpacityEffect *effect;
    float opacity;
    bool helpcount = false;
};

#endif // MAINWINDOW_H
