#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <centralac.h>
#include <QMessageBox>
#include "currentinfo.h"
#include <QGridLayout>
#include <QGraphicsOpacityEffect>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CentralAC *CAC;

private slots:

    void on_login_clicked();

    void on_exit_h_clicked();

    void on_room_info_toggled(bool checked);

    void ac_timeout();

    void temper_timeout();

    void on_current_info_clicked();

    void on_room_info_clicked();

    void on_exit_clicked();

    void on_current_info_toggled(bool checked);
    void on_test_btn_clicked();

    void on_print_bill_clicked();

    void on_register_btn_clicked();

    void on_pushButton_toggled(bool checked);

    void on_account_week_toggled(bool checked);

    void on_account_month_toggled(bool checked);

    void on_account_year_toggled(bool checked);

    void on_calendar1_clicked(const QDate &date);

    void on_calendar2_clicked(const QDate &date);

    void on_calendar3_clicked(const QDate &date);

    void on_calendar4_clicked(const QDate &date);

    void on_AC_on_clicked();

    void set_page(int i);

    void on_pushButton_2_clicked();

    void on_add_rate_button_clicked();

    void on_pushButton_3_clicked();

    void on_warm_button_clicked();

    void on_cool_button_clicked();

    void on_AC_off_clicked();



    void on_pushButton_clicked();

    void fade_on_fade_out();

    void gif_move();

    void gif_fade();

    void on_room_line1_textEdited(const QString &arg1);

    void clear_buffer2(int roomnumber);


signals:
    rotateWindow();

private:
    Ui::MainWindow *ui;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    Database page_db;
    Account *page_account;
    bool m_isPressed;
    QPoint m_startMovePos;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    int start_ac_flag = 0;
    QGraphicsOpacityEffect *effect;
    QGraphicsOpacityEffect *effect2;
    float opacity;
    QTimer *warning_timer;
    QTimer *gif_timer;
    QTimer *fade_timer;
    float gif_opa;
    QGraphicsOpacityEffect *gif_effect;
    int gif_count;
    int timer_flag;
    QGridLayout *baseLayout;
    QChartView *chartView;
    QSplineSeries *series;
};

#endif // MAINWINDOW_H
