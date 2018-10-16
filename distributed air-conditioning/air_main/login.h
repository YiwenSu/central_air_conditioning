#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_loginBT_clicked();

    void on_exitBT_clicked();

private:
    Ui::login *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    bool m_isPressed;
    QPoint m_startMovePos;
signals:
    void rotateWindow();
    void page(int i);
    void exit();
};

#endif // LOGIN_H
