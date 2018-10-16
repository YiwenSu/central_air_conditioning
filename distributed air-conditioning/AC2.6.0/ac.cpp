#include "ac.h"

AC::AC()
{

}
AC::AC(QString serverip, int serverport)
{

    std::string a = serverip.toStdString();
    this->myaccom = new AC_Com(a,serverport,this);
    this->speedtimer = new QTimer(this);
    connect(speedtimer, SIGNAL(timeout()), this, SLOT(setRealspeedto0()));//过了三秒没有风包，就将真实风速设为0
    connect(this,SIGNAL(tospeedtimer()),this,SLOT(tochangespeedtimer()));
    this->speedtimer->start(5000);//每一次来了风包就重新开始计时
    this->setRoomTmp(29);
}
AC::~AC()
{
    delete myaccom;
}

int AC::getMode()
{
    return this->mode;
}
int AC::getSpeed()
{
    return this->speed;
}

void AC::setSpeed(int speed)
{
    this->speed = speed;
    //this->real_speed = 0;
    if(this->isAutoset == true)//如果自动设为0标记true,这样我手动将风速设为0时，就不会触发温度升高后风速设为原来风速的函数
        this->isAutoset = false;
    //============
    if(this->isClose == 0)
        this->myaccom->sendTmpReq(this->getMode(),speed,this->getTmp());
}

double AC::getEnergy()
{
    return this->energy;
}
double AC::getMoney()
{
    return this->money;
}
int AC::getRoomTmp()
{
    return this->temperature_room;
}
int AC::getTmp()
{
    return this->temperature;
}
void AC::setTmp(int temperature)
{
    //this->temp_temperature = temperature;
    //if(!this->timer->isActive())
    //{
    //    this->timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(real_setTmp()));
    //}
    //timer->start(1000);
    this->temperature = temperature;
    if(this->isClose == 0)
        this->myaccom->sendTmpReq(this->getMode(),this->getSpeed(),temperature);
    //this->real_speed = 0;
    //emit realspd_changed();
}
void AC::setTmp_whenChange(int temperature)
{
    this->temperature = temperature;
    if(this->timerisNew == 0)
    {
        this->timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(real_setTmp()));
        this->timerisNew = 1;
    }
    timer->start(1000);
    //this->temperature = temperature;
    //this->myaccom->sendTmpReq(this->getMode(),this->getSpeed(),temperature);

}
void AC::setTmp_noSend(int temperature)
{
    this->temperature = temperature;
}
void AC::setRoomTmp(int temperature_room)
{
    this->temperature_room = temperature_room;
    emit sgl_setroomtmp();
}
void AC::setTmp_uplimit(int uplimit)
{
    this->tem_uplimit = uplimit;
}
void AC::setTmp_downlimit(int downlimit)
{
    this->tem_downlimit = downlimit;
}
int AC::getTmp_uplimit()
{
    return this->tem_uplimit;
}
int AC::getTmp_downlimit()
{
    return this->tem_downlimit;
}
void AC::setTmp_warm_uplimit(int uplimit)
{
    this->tem_warm_uplimit = uplimit;
}
void AC::setTmp_warm_downlimit(int downlimit)
{
    this->tem_warm_downlimit = downlimit;
}
int AC::getTmp_warm_uplimit()
{
    return this->tem_warm_uplimit;
}
int AC::getTmp_warm_downlimit()
{
    return this->tem_warm_downlimit;
}
void AC::setEnergy(double energy)
{
    this->energy = energy;
    qDebug()<<"setenergy"<<endl;
}
void AC::setMoney(double money)
{
    this->money = money;
    qDebug()<<"setmoney"<<endl;
}
void AC::setMode(int mode)
{
    this->mode = mode;
    if(this->isClose == 0)
        this->myaccom->sendTmpReq(mode,this->getSpeed(),this->getTmp());
    //this->real_speed = 0;
    //emit realspd_changed();
}

void AC::setMode_noSend(int mode)
{
    this->mode = mode;
}

void AC::login(int roomNumber, int userid)
{
    this->roomnumber = roomNumber;
    this->userid = userid;
}

void AC::run()
{
    this->isClose = false;
    this->myaccom->run();
    this->myaccom->sendIdInfo(this->roomnumber,this->userid);
}
void AC::run_noSend()
{
    this->isClose = false;
    this->myaccom->run();
}

void AC::setIdCheckRes(bool flag)
{
    qDebug()<<"标志位 "<<this->isClose<<endl;
    this->flag = flag;

    emit logon();

    emit second_turnon();
    qDebug()<<"第二次打开";

}

bool AC::getflag()
{
    return this->flag;
}

void AC::close()
{
    this->isClose = true;
    this->myaccom->sendCloseReq();
    this->myaccom->close();
}

void AC::setAirBag(int speed)
{
    qDebug()<<"setAirbag"<<endl;
    this->setRealspeed(speed);//来了风包，真实风速才更改
    emit realspd_changed();
    //this->speedtimer->start(3000);//每一次来了风包就重新开始计时
    this->airCnt += speed;
    if(this->airCnt >= 10)
    {
        this->airCnt %= 10;
        //if(mode == 0)//制冷模式
        //{
        //    int i = this->getRoomTmp() - 1;
        //    this->setRoomTmp(i);
        //}
        //else//制热模式
        //{
        //    int i = this->getRoomTmp() + 1;
        //    this->setRoomTmp(i);
        //}
        if(this->getRoomTmp()<this->getTmp())
        {
            int i = this->getRoomTmp() + 1;
            this->setRoomTmp(i);
        }
        else
        {
            int i = this->getRoomTmp() - 1;
            this->setRoomTmp(i);
        }
        emit tem_change();
    }
}

void AC::setAirCnt(int a)
{
    this->airCnt = a;
}

int AC::getAirCnt()
{
    return this->airCnt;
}

void AC::schedule(int mode)
{
    int count = 0;
    while(true)
    {
        int oldSpeed = this->speed;
        if(this->mode == 0)//如果制冷
        {
            if(this->temperature_room == this->temperature)//如果室温等于调控的温度
            {
                this->setSpeed(0);//停风
                this->setRealspeed(0);
                this->isAutoset = true;
                emit speedholdon();//告诉界面风速为0
            }
        }
        else if(this->mode == 1)//如果制热
        {
            if(this->temperature_room == this->temperature)//如果室温等于调控的温度
            {
                this->setSpeed(0);//停风
                this->setRealspeed(0);
                this->isAutoset = true;
                emit speedholdon();//告诉界面风速为0
            }
        }
        while(this->real_speed == 0)//如果停风
        {
            Sleep(1000);
            count++;
            if(count * qAbs(this->temperature_world - this->temperature_room) > 100)
            {
                //if(mode == 0)//如果制冷
                //{
                //    int i = this->getRoomTmp() + 1;
                //    this->setRoomTmp(i);
                //}
                //else//如果制热
                //{
                //    int i = this->getRoomTmp() - 1;
                //    this->setRoomTmp(i);
                //}
                if(this->getRoomTmp() < this->getTmpWorld())//如果现在室温比环境温度低
                {
                    int i = this->getRoomTmp() + 1;
                    this->setRoomTmp(i);
                }
                else if(this->getRoomTmp() > this->getTmpWorld())
                {
                    int i = this->getRoomTmp() - 1;
                    this->setRoomTmp(i);
                }
                count = 0;
                if(this->isAutoset == true)//如果风速是被自动设为0的
                {
                    if(this->isClose == 0)
                    {
                        this->setSpeed(oldSpeed);//设定为之前的风速
                        emit speedback(oldSpeed);
                    }
                    else
                    {
                        while(this->isClose == 1)
                        {

                        }
                        this->setSpeed(oldSpeed);//设定为之前的风速
                        emit speedback(oldSpeed);
                    }
                }
            }
        }

    }

}
int AC::getRoomNumber()
{
    return this->roomnumber;
}

AC_Com* AC::getMyaccom()
{
    return this->myaccom;
}

void AC::real_setTmp()
{

    //this->temperature = temp_temperature;
    if(this->isClose == 0)
        this->myaccom->sendTmpReq(this->getMode(),this->getSpeed(),temperature);
    this->timer->stop();                                                        //这一次发送后就暂停计时器
}
void AC::setRealspeedto0()
{
    this->real_speed = 0;
    if(this->isClose == 0)
        emit realspd_changed();
}

void AC::setRealspeed(int speed)
{
    this->real_speed = speed;
    if(speed != 0)
        emit tospeedtimer();
    emit realspd_changed();
}

int AC::getRealspeed()
{
    return this->real_speed;
}
void AC::tochangespeedtimer()
{
    this->speedtimer->start(5000);
}

int AC::getTmpWorld()
{
    return this->temperature_world;
}
