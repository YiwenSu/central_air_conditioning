#ifndef AC_H
#define AC_H
#include "accom.h"
#include "ac_com.h"
#include <QMap>
#include <QObject>
#include <QDebug>
#include <string>
#include <QTimer>
#include <synchapi.h>
class AC : public QObject
{
    Q_OBJECT

public:
    explicit AC();
     ~AC();
    AC(QString serverip,int serverport);
    void login(int roomNumber,int userid);			//用户入住
    void run();											//开机，开启从控机通信类，发送认证消息
    void run_noSend();											//开机
    void close();										//关机，关闭从控机通信类，若认证失败，从控机会被关闭
    void setTmp(int temperature);						//设置从控机的设定温度
    void setTmp_whenChange(int temperature);						//设置从控机的设定温度
    void setTmp_noSend(int temperature);						//设置从控机的设定温度
    int getTmp();										//获取设定温度
    void setRoomTmp(int temperature_room);					//设置房间温度
    int getRoomTmp();									//获取房间温度
    void setSpeed(int speed);							//设置风速
    void setRealspeed(int speed);
    int getSpeed();										//获取风速
    void setEnergy(double energy);						//设置累积消耗能量
    double getEnergy();									//获取累积消耗能量
    void setMoney(double money);						//设置累计消费金额
    double getMoney();									//获取累积消费金额
    void setMode(int mode);								//设置制冷/供暖模式，用户不能自己设定，只能由AC_Com根据认证信息的返回结果设置
    void setMode_noSend(int mode);
    int getMode();
    void setTmp_uplimit(int uplimit);						//设置从控机的上限温度
    int getTmp_uplimit();
    void setTmp_downlimit(int downlimit);						//设置从控机的下限温度
    int getTmp_downlimit();
    void setTmp_warm_uplimit(int uplimit);						//设置从控机的制热上线温度
    int getTmp_warm_uplimit();
    void setTmp_warm_downlimit(int downlimit);						//设置从控机的制热下限温度
    int getTmp_warm_downlimit();
    bool getflag();
    int getRoomNumber();
    int getRealspeed();

    void setIdCheckRes(bool flag);

    void setAirBag(int speed);							//接受风包，来模拟房间温度的变化
    void setAirCnt(int a);
    int getAirCnt();
    void schedule(int mode);//根据风包和模式调节室温
    AC_Com* getMyaccom();
    int getTmpWorld();
public slots:
    void real_setTmp();
    void setRealspeedto0();
    void tochangespeedtimer();
private:
    bool isAutoset = false;//是否自动设为风速0
    int temperature;
    int temperature_room;
    int speed = 1;
    int real_speed = 1;
    double energy;
    double money;
    int mode;
    int tem_uplimit;
    int tem_downlimit;
    int tem_warm_uplimit;
    int tem_warm_downlimit;
    bool flag = false;//是否验证成功
    bool isClose = false;//是否被关闭
    int roomnumber;
    int userid;

    AC_Com *myaccom;

    int airCnt = 0;

    int temperature_world = 34;

    QTimer *timer;
    QTimer *speedtimer;
    int temp_temperature;
    int isSet = 0;//为0，就不能响应realsetTmp，说明当前的设定请求已发送过
    int timerisNew = 0;//timer是不是new过
signals:
     void tem_change();
     void logon();
     void speedholdon();
     void speedback(int oldspeed);
     void sgl_setroomtmp();
     void realspd_changed();
     void tospeedtimer();
     void second_turnon();
};

#endif // AC_H
