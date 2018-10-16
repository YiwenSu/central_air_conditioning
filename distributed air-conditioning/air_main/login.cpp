#include "login.h"
#include "ui_login.h"
#include<QMouseEvent>
#include"QDebug"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //ui->pButtonArrow->raise();
    this->raise();
    ui->pwd->setEchoMode(QLineEdit::Password);
    m_isPressed=0;
}

login::~login()
{
   // qDebug()<<"登陆界面死亡！";
    delete ui;
}

void login::on_loginBT_clicked()
{
    //int i=ui->user->text().toInt();
    QString name = ui->user->text();
    QString password = ui->pwd->text();
    int i = -1;
    if(name.isEmpty()||password.isEmpty())
    {
        QMessageBox::about(NULL, "WARNING", "Please input every blank!");
        ui->user->clear();
        ui->pwd->clear();
    }
    else if(name == "1" && password == "1")
    {
        i=1;
        emit rotateWindow();
        emit page(i);
        if(!ui->remenber->isChecked())
        {
            ui->user->clear();
            ui->pwd->clear();
        }
    }
    else if(name == "2" && password == "2")
    {
        i = 0;
        emit rotateWindow();
        emit page(i);
        ui->user->clear();
        ui->pwd->clear();
    }
    else
    {
        //<font color='red'>NAME AND PASSWORD</font>
        QMessageBox::about(NULL, "WARNING", "Incorrect <font color='red'>NAME</font> or <font color='red'>PASSWORD</font>");
        ui->user->clear();
        ui->pwd->clear();
    }

}

void login::on_exitBT_clicked()
{
    emit exit();
}

void login::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void login::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos() + movePoint;
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x(), widgetPos.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void login::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}
