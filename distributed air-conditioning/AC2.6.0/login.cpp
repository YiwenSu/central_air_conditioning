#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->mainwindow = new MainWindow();
    connect(mainwindow->getMyac(), SIGNAL(logon()), this, SLOT(logon1()));
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->ui->waitlabel1->setVisible(false);
    QMovie *movie = new QMovie(":/wait.gif");
    this->ui->waitlabel1->setMovie(movie);
    movie->start();

    QIntValidator *validater1 = new QIntValidator;
    QIntValidator *validater2 = new QIntValidator;
    validater1->setRange(1,10000);
    validater2->setRange(1,10000);
    this->ui->inputID->setValidator(validater1);
    this->ui->inputPWD->setValidator(validater2);
}

Login::~Login()
{
    delete ui;
    delete mainwindow;
}

void Login::accept()
{

    this->x = ui->inputID->text().toInt();
    this->y = ui->inputPWD->text().toInt();

    //this->mainwindow = new MainWindow();
    this->mainwindow->myaclogin(this->y,this->x);
    this->mainwindow->myacrun();
    //this->wait1->show();
    this->ui->waitlabel1->setVisible(true);
}

void Login::on_button_clicked()
{
    this->accept();
}

void Login::logon1()
{
    if(this->mainwindow->getmyacflag() == true)//如果验证通过，就打开主窗口，关闭登录界面
    {
        this->mainwindow->setRoomID(this->y);
        this->mainwindow->setUseriD(this->x);
        mainwindow->show();
        this->close();
    }
    else if(this->x == 0 || this->y == 0)
    {
        QMessageBox::warning(this,"警告","roomID和userID均不能为空",QMessageBox::Yes);
        this->ui->inputPWD->setFocus();
    }
    else//否则就删除
    {
        QMessageBox::warning(this,"警告","用户名或密码错误",QMessageBox::Yes);
        this->ui->inputPWD->setFocus();
        //delete this->mainwindow;
    }
}

