#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myac = new AC(IP,10000);
    connect(myac, SIGNAL(tem_change()), this, SLOT(tem_change1()));//将ac的室温改变信号与主界面的室温改变关联
    connect(myac,SIGNAL(speedholdon()), this,SLOT(speed0()));//在屏幕上设定风速为0
    connect(myac,SIGNAL(speedback(int)), this,SLOT(speedback0(int)));
    connect(myac,SIGNAL(sgl_setroomtmp()), this,SLOT(slot_setroomtmp()));
    connect(myac,SIGNAL(realspd_changed()), this,SLOT(slot_setrealspeed()));
    connect(myac,SIGNAL(second_turnon()), this,SLOT(slot_secondturnon()));
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->ui->unuse->setVisible(false);
    this->ui->label->setVisible(false);
    this->ui->roomtem->setVisible(false);
    this->ui->IDBtn->setVisible(false);
    this->ui->modeBtn->setVisible(false);
    this->ui->speedBtn->setVisible(false);
    this->ui->turnonBtn_2->setVisible(false);
    this->ui->toolButton->setVisible(false);
    this->ui->temup->setVisible(false);
    this->ui->temdown->setVisible(false);
    this->ui->blue->setVisible(false);
    this->ui->orange->setVisible(false);
    this->ui->pink->setVisible(false);
    this->ui->time->setVisible(false);
    this->ui->mode_cold->setVisible(false);
    this->ui->mode_warm->setVisible(false);
    this->ui->speed_0->setVisible(false);
    this->ui->speed_1->setVisible(false);
    this->ui->speed_2->setVisible(false);
    this->ui->speed_3->setVisible(false);
    word_close();
    this->ui->tem_label->setVisible(false);
    this->ui->tem_room_label->setVisible(false);
    this->ui->ID_label->setVisible(false);
    this->ui->realspeed->setVisible(false);
    this->ui->logo->setVisible(true);
    myac->setTmp_uplimit(30);
    myac->setTmp_downlimit(18);
    //myac->setEnergy(100.5);
    //myac->setMoney(123.6);
    this->ui->graylabel->setVisible(false);
    effect = new QGraphicsOpacityEffect(this);
    this->ui->waitlabel1->setVisible(false);
    QMovie *movie = new QMovie(":/wait.gif");
    this->ui->waitlabel1->setMovie(movie);
    movie->start();

    this->ui->help->setVisible(false);
    this->ui->help_2->setVisible(false);
    this->ui->help_3->setVisible(false);
    this->ui->help_4->setVisible(false);
    this->ui->help_5->setVisible(false);
    this->ui->help_6->setVisible(false);
    this->ui->help_7->setVisible(false);
    this->ui->help_8->setVisible(false);
    this->ui->help_9->setVisible(false);
    this->ui->help_10->setVisible(false);
    this->ui->help_11->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myac;
    qDebug()<<"delete myac";
}



void MainWindow::on_turnonBtn_clicked()//按下开机按钮后
{
    int mode = this->myac->getMode();
    if(this->opencount != 1)
    {
        //myac = new AC(IP,10000);
        myac->run();//再次开机
        this->ui->waitlabel1->setVisible(true);
    }
    if(opencount == 1)
    {
        this->ui->realspeed->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->IDBtn->setVisible(true);
        this->ui->modeBtn->setVisible(true);
        this->ui->speedBtn->setVisible(true);
        this->ui->turnonBtn_2->setVisible(true);
        this->ui->turnonBtn->setVisible(false);
        this->ui->roomtem->setVisible(true);
        this->ui->toolButton->setVisible(true);
        this->ui->temup->setVisible(true);
        this->ui->temdown->setVisible(true);
        this->ui->blue->setVisible(true);
        this->ui->orange->setVisible(true);
        this->ui->pink->setVisible(true);
        this->ui->time->setVisible(true);
        this->ui->tem_label->setVisible(true);
        this->ui->tem_room_label->setVisible(true);
        this->ui->unuse->setVisible(true);
        this->ui->ID_label->setVisible(true);
        this->ui->logo->setVisible(false);
        int mode = this->myac->getMode();//初始化模式
        if(mode == 0)//制冷模式
        {
            this->ui->mode_cold->setVisible(true);
        }
        else
        {
            this->ui->mode_warm->setVisible(true);
        }
        //初始化风速
        //this->myac->setSpeed(1);
        int speed = this->myac->getSpeed();
        switch(speed)
        {
            case 0:this->ui->speed_0->setVisible(true);break;
            case 1:this->ui->speed_1->setVisible(true);break;
            case 2:this->ui->speed_2->setVisible(true);break;
            case 3:this->ui->speed_3->setVisible(true);break;
        }

        word_close();
        QString info = "Successfully turn on!";
        this->ui->success_label->setText(info);
        this->ui->success_label->setVisible(true);
    }


    /******************初始化*******************/
    //this->myac->setMode(1);


    //初始化温度
    if(this->opencount == 1)
    {
        //this->myac->setTmp(25);
        if(mode == 0)
            this->myac->setTmp(22);
        else
            this->myac->setTmp(28);
        int tem = this->myac->getTmp();
        QString tem1 = QString::number(tem,10);
        this->ui->tem_label->setText(tem1);
        //this->myac->setRoomTmp(29);
        int roomtem = this->myac->getRoomTmp();
        QString roomtem1 = QString::number(roomtem,10);
        this->ui->tem_room_label->setText(roomtem1);
    }
    //else
    //{
    //    int tem = this->myac->getTmp();
    //    this->myac->setTmp(tem);
    //}

    //初始化状态栏
    //word_close();
    //QString info = "Successfully turn on!";
    //this->ui->success_label->setText(info);
    //this->ui->success_label->setVisible(true);

    //初始化时间
    if(this->opencount == 1)
    {
        this->datatime = new QDateTime(QDateTime::currentDateTime());
        this->ui->time->setText(datatime->currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updatetime()));
        timer->start(1000);
    }

    //初始化ID
    if(this->opencount == 1)
    {
        QString ID1 = QString::number(this->userID);
        QString roomID1 = QString::number(this->roomID);
        QString temp = "ID:"+ID1+"\nRoom:"+roomID1;
        this->ui->ID_label->setText(temp);

    }

    //多线程
    if(this->opencount == 1)
    {
        WorkThread *threadnew = new WorkThread();
        threadnew->set(this->myac);
        threadnew->start();
    }


    //第一次打开后，将opencount赋值
    if(this->opencount == 1)
        this->opencount = 2;
}

void MainWindow::on_speedBtn_clicked()//按下speed按钮后
{
    this->ui->speed_0->setVisible(false);
    this->ui->speed_1->setVisible(false);
    this->ui->speed_2->setVisible(false);
    this->ui->speed_3->setVisible(false);
    int speed = this->myac->getSpeed();//获取风速
    speed = (speed+1) % 4;
    this->myac->setSpeed(speed);//设定风速,并发送请求给主机
    switch(speed)
    {
        case 0:this->ui->speed_0->setVisible(true);break;
        case 1:this->ui->speed_1->setVisible(true);break;
        case 2:this->ui->speed_2->setVisible(true);break;
        case 3:this->ui->speed_3->setVisible(true);break;
    }
}

void MainWindow::on_modeBtn_clicked()
{
    int mode = this->myac->getMode();
    if(mode == 0)//制冷模式
    {
        this->ui->mode_cold->setVisible(false);
    }
    else
    {
        this->ui->mode_warm->setVisible(false);
    }
    mode++;
    mode %= 2;
    this->myac->setMode(mode);
    if(mode == 0)//制冷模式
    {
        this->ui->mode_cold->setVisible(true);
    }
    else
    {
        this->ui->mode_warm->setVisible(true);
    }
}

void MainWindow::on_IDBtn_clicked()
{

}

void MainWindow::on_turnonBtn_2_clicked()//按下关机按钮后
{
    this->ui->realspeed->setVisible(false);
    this->ui->label->setVisible(false);
    this->ui->roomtem->setVisible(false);
    this->ui->IDBtn->setVisible(false);
    this->ui->modeBtn->setVisible(false);
    this->ui->speedBtn->setVisible(false);
    this->ui->turnonBtn_2->setVisible(false);
    this->ui->turnonBtn->setVisible(true);
    this->ui->toolButton->setVisible(false);
    this->ui->temup->setVisible(false);
    this->ui->temdown->setVisible(false);
    this->ui->blue->setVisible(false);
    this->ui->orange->setVisible(false);
    this->ui->pink->setVisible(false);
    this->ui->time->setVisible(false);
    this->ui->mode_cold->setVisible(false);
    this->ui->mode_warm->setVisible(false);
    this->ui->speed_0->setVisible(false);
    this->ui->speed_1->setVisible(false);
    this->ui->speed_2->setVisible(false);
    this->ui->speed_3->setVisible(false);
    word_close();
    this->ui->tem_label->setVisible(false);
    this->ui->tem_room_label->setVisible(false);
    this->ui->ID_label->setVisible(false);
    this->ui->unuse->setVisible(false);
    this->ui->logo->setVisible(true);
    this->myac->close();
}

void MainWindow::on_blue_clicked()//按下电量按钮后
{
    word_close();
    double energy = this->myac->getEnergy();
    QString energy1 = QString::number(energy);
    QString info = "Your have consumed "+energy1+" degree.";
    this->ui->info_label->setText(info);
    this->ui->info_label->setVisible(true);
}

void MainWindow::on_temup_clicked()//温度增加按钮
{
    word_close();
    int tem = this->myac->getTmp();
    int pretem = tem;
    tem++;
    if(tem>=this->myac->getTmp_downlimit() && tem<=this->myac->getTmp_uplimit())//如果温度在允许范围内
    {
        if(pretem == 30 && tem == 31)
        {
            word_close();
            this->ui->warning_label->setText("Warning: The temperature is too high");
            this->ui->warning_label->setVisible(true);
        }
        this->myac->setTmp_whenChange(tem);
        QString tem1 = QString::number(tem,10);
        this->ui->tem_label->setText(tem1);
    }
    else
    {
        this->ui->error_label->setVisible(true);
        QString up = QString::number(this->myac->getTmp_uplimit());
        QString down = QString::number(this->myac->getTmp_downlimit());
        QString error = "ERROR:The temperature must be set "+down+" ~ "+up;

        this->ui->error_label->setText(error);
    }

}

void MainWindow::on_temdown_clicked()//温度降低按钮
{
    word_close();
    int tem = this->myac->getTmp();
    int pretem = tem;
    tem--;
    if(tem>=this->myac->getTmp_downlimit() && tem<=this->myac->getTmp_uplimit())
    {
        if(pretem == 20 && tem == 19)
        {
            word_close();
            this->ui->warning_label->setText("Warning: The temperature is too low");
            this->ui->warning_label->setVisible(true);
        }
        this->myac->setTmp_whenChange(tem);
        QString tem1 = QString::number(tem,10);
        this->ui->tem_label->setText(tem1);
    }
    else
    {
        this->ui->error_label->setVisible(true);
        QString up = QString::number(this->myac->getTmp_uplimit());
        QString down = QString::number(this->myac->getTmp_downlimit());
        QString error = "ERROR:The temperature must be set "+down+" ~ "+up;

        this->ui->error_label->setText(error);
    }
}
void MainWindow::word_close()//关闭所有信息条
{
    this->ui->error_label->setVisible(false);
    this->ui->warning_label->setVisible(false);
    this->ui->info_label->setVisible(false);
    this->ui->success_label->setVisible(false);
}

void MainWindow::on_orange_clicked()//按下费用按钮,显示现在费用
{
    word_close();
    double money = this->myac->getMoney();
    QString money1 = QString::number(money);
    QString info = "You have spend "+money1+" yuan.";
    this->ui->info_label->setText(info);
    this->ui->info_label->setVisible(true);
}
void MainWindow::updatetime()
{
    this->datatime = new QDateTime(QDateTime::currentDateTime());
    this->ui->time->setText(datatime->currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::myaclogin(int roomnumber, int userid)
{
    this->myac->login(roomnumber,userid);
}
void MainWindow::myacrun()
{
    this->myac->run();
}

bool MainWindow::getmyacflag()
{
    return this->myac->getflag();
}

void MainWindow::tem_change1()
{
    QString a = QString::number(this->myac->getRoomTmp());//将int转为qstring
    this->ui->tem_room_label->setText(a);
}

AC* MainWindow::getMyac()
{
    return this->myac;
}

void MainWindow::setRoomID(int roomid)
{
    this->roomID = roomid;
}

void MainWindow::setUseriD(int userid)
{
    this->userID = userid;
}

void MainWindow::speed0()//将界面设为风速0
{
    this->ui->speed_0->setVisible(false);
    this->ui->speed_1->setVisible(false);
    this->ui->speed_2->setVisible(false);
    this->ui->speed_3->setVisible(false);
    int speed = 0;
    switch(speed)
    {
        case 0:this->ui->speed_0->setVisible(true);break;
        case 1:this->ui->speed_1->setVisible(true);break;
        case 2:this->ui->speed_2->setVisible(true);break;
        case 3:this->ui->speed_3->setVisible(true);break;
    }
}

void MainWindow::speedback0(int oldspeed)//将界面设为原来的风速
{
    this->ui->speed_0->setVisible(false);
    this->ui->speed_1->setVisible(false);
    this->ui->speed_2->setVisible(false);
    this->ui->speed_3->setVisible(false);
    switch(oldspeed)
    {
        case 0:this->ui->speed_0->setVisible(true);break;
        case 1:this->ui->speed_1->setVisible(true);break;
        case 2:this->ui->speed_2->setVisible(true);break;
        case 3:this->ui->speed_3->setVisible(true);break;
    }
}

void MainWindow::slot_setroomtmp()
{
    int tmp = this->myac->getRoomTmp();
    QString a = QString::number(tmp);
    this->ui->tem_room_label->setText(a);
}

void MainWindow::on_temup_toggled(bool checked)
{

}

void MainWindow::slot_setrealspeed()
{
    int realspeed = this->myac->getRealspeed();
    switch(realspeed)
    {
    case 0:this->ui->realspeed->setStyleSheet("border-image: url(:/status/realspeed0.png);");break;
    case 1:this->ui->realspeed->setStyleSheet("border-image: url(:/status/realspeed1.png);");break;
    case 2:this->ui->realspeed->setStyleSheet("border-image: url(:/status/realspeed2.png);");break;
    case 3:this->ui->realspeed->setStyleSheet("border-image: url(:/status/realspeed3.png);");break;
    }
    //if(this->opencount != 1)
    //{
    //    word_close();
    //    QString tempspeed = QString::number(realspeed);
    //    QString info = "realspeed is set to Level " + tempspeed;
    //    this->ui->info_label->setText(info);
    //    this->ui->info_label->setVisible(true);
    //}

}

void MainWindow::slot_secondturnon()
{
    if(opencount != 1)
    {
        bool flag = this->myac->getflag();
        if(flag == true)
        {
            this->ui->waitlabel1->setVisible(false);
            this->ui->realspeed->setVisible(true);
            this->ui->label->setVisible(true);
            this->ui->IDBtn->setVisible(true);
            this->ui->modeBtn->setVisible(true);
            this->ui->speedBtn->setVisible(true);
            this->ui->turnonBtn_2->setVisible(true);
            this->ui->turnonBtn->setVisible(false);
            this->ui->roomtem->setVisible(true);
            this->ui->toolButton->setVisible(true);
            this->ui->temup->setVisible(true);
            this->ui->temdown->setVisible(true);
            this->ui->blue->setVisible(true);
            this->ui->orange->setVisible(true);
            this->ui->pink->setVisible(true);
            this->ui->time->setVisible(true);
            this->ui->tem_label->setVisible(true);
            this->ui->tem_room_label->setVisible(true);
            this->ui->unuse->setVisible(true);
            this->ui->ID_label->setVisible(true);
            this->ui->logo->setVisible(false);

            /******************初始化*******************/
            //this->myac->setMode(1);
            int mode = this->myac->getMode();//初始化模式
            if(mode == 0)//制冷模式
            {
                this->ui->mode_cold->setVisible(true);
            }
            else
            {
                this->ui->mode_warm->setVisible(true);
            }
            //初始化风速
            //this->myac->setSpeed(1);
            int speed = this->myac->getSpeed();
            switch(speed)
            {
                case 0:this->ui->speed_0->setVisible(true);break;
                case 1:this->ui->speed_1->setVisible(true);break;
                case 2:this->ui->speed_2->setVisible(true);break;
                case 3:this->ui->speed_3->setVisible(true);break;
            }

            //初始化温度
            if(this->opencount == 1)
            {
                //this->myac->setTmp(25);
                if(mode == 0)
                    this->myac->setTmp(22);
                else
                    this->myac->setTmp(28);
                int tem = this->myac->getTmp();
                QString tem1 = QString::number(tem,10);
                this->ui->tem_label->setText(tem1);
                this->myac->setRoomTmp(29);
                int roomtem = this->myac->getRoomTmp();
                QString roomtem1 = QString::number(roomtem,10);
                this->ui->tem_room_label->setText(roomtem1);
            }
            else
            {
                int tem = this->myac->getTmp();
                this->myac->setTmp(tem);
            }

            //初始化状态栏
            word_close();
            QString info = "Successfully turn on!";
            this->ui->success_label->setText(info);
            this->ui->success_label->setVisible(true);

            //初始化时间
            if(this->opencount == 1)
            {
                this->datatime = new QDateTime(QDateTime::currentDateTime());
                this->ui->time->setText(datatime->currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                QTimer *timer = new QTimer(this);
                connect(timer, SIGNAL(timeout()), this, SLOT(updatetime()));
                timer->start(1000);
            }

            //初始化ID
            if(this->opencount == 1)
            {
                QString ID1 = QString::number(this->userID);
                QString roomID1 = QString::number(this->roomID);
                QString temp = "ID:"+ID1+"\nRoom:"+roomID1;
                this->ui->ID_label->setText(temp);

            }

            //多线程
            if(this->opencount == 1)
            {
                WorkThread *threadnew = new WorkThread();
                threadnew->set(this->myac);
                threadnew->start();
            }


            //第一次打开后，将opencount赋值
            if(this->opencount == 1)
                this->opencount = 2;
        }
    }

}

void MainWindow::on_pink_clicked()
{
    if(helpcount == true)
    {
        this->ui->help->setVisible(false);
        this->ui->help_2->setVisible(false);
        this->ui->help_3->setVisible(false);
        this->ui->help_4->setVisible(false);
        this->ui->help_5->setVisible(false);
        this->ui->help_6->setVisible(false);
        this->ui->help_7->setVisible(false);
        this->ui->help_8->setVisible(false);
        this->ui->help_9->setVisible(false);
        this->ui->help_10->setVisible(false);
        this->ui->help_11->setVisible(false);
        this->ui->graylabel->setVisible(false);
    }
    else if(helpcount == false)
    {
        this->ui->graylabel->setVisible(true);
        this->ui->graylabel->raise();
        opacity =0.7;
        effect->setOpacity(opacity);
        ui->graylabel->setGraphicsEffect(effect);
        this->ui->help->setVisible(true);
        this->ui->help->raise();
        this->ui->pink->raise();
        this->ui->help_2->setVisible(true);
        this->ui->help_3->setVisible(true);
        this->ui->help_4->setVisible(true);
        this->ui->help_5->setVisible(true);
        this->ui->help_6->setVisible(true);
        this->ui->help_7->setVisible(true);
        this->ui->help_8->setVisible(true);
        this->ui->help_9->setVisible(true);
        this->ui->help_10->setVisible(true);
        this->ui->help_11->setVisible(true);
        this->ui->help_2->raise();
        this->ui->help_3->raise();
        this->ui->help_4->raise();
        this->ui->help_5->raise();
        this->ui->help_6->raise();
        this->ui->help_7->raise();
        this->ui->help_8->raise();
        this->ui->help_9->raise();
        this->ui->help_10->raise();
        this->ui->help_11->raise();
    }


    if(helpcount == true)
        helpcount = false;
    else
        helpcount = true;
}
