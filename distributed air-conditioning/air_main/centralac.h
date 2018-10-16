#ifndef CENTRALAC_H
#define CENTRALAC_H
#include <QString>
#include "database.h"
#include <queue>
#include <centralac_com.h>
#include <req.h>
#include <QDateTime>
#include <QTimer>
#include "account.h"
#include <vector>
#include <iostream>
#include <QTableWidget>
#include <QColor>
#include <QBrush>
#include <vector>
#include <mutex>
using namespace std;
class CentralAC
{
public:
    CentralAC();
    ~CentralAC();
    void run();														//中央空调开启，开启中央空调通信
    void close();													//关闭中央空调通信
    void setRoomTmp(int roomNumber,int temperature);				//监测温度请求发出后，由CentralAC_Com填写房间温度
    void setRoomTmpReq(int roomNumber,int mode,int speed,int temperature);	//收到从控机温控请求后，由CentralAC_Com调用，中央空调要做好记录信息，用来生成报表
    int getMode();													//获取中央空调工作模式
    bool checkIdInfo(int roomNumber, int userid);					//验证房间号和用户Id是否合法，合法返回true
    int getInterval();
    int getswitch();
    void get_system_time();
    QString gettime();
    QString getdate_string();
    int getdate();
    QTimer *ac_timer;                                               //定时器
    QTimer *temper_timer;                                           //定时器
    void ac_timeout();                                              //发送风包定时事件
    void temper_timeout();                                          //检测温度定时事件
    void roomLogOut(int roomNumber);                                //某个从控机关机
    int add_rate();                                                 //加刷新频率
    int sub_rate();                                                 //减刷新频率
    int change_mode_to_warm();                                      //制热 1
    int change_mode_to_cold();                                      //制冷 0
    void set_row(QTableWidget *table);                               //标红表格信息
    vector<int> get_CPU();
    //bool judge_temperature(int temp,int target);
private:
    void setMode(int mode);
    void setInterval(int rate);	                                    //管理员可以设置监控房间温度的频率
    void clear_buffer();                                            //删除所有命令
    int system_time;                                                //基准时间
private:
    Req* CPU_preempt[3];                                            //执行队列缓存
    int Switch = 0;                                                     //0关机，1开机
    CentralAC_Com *Ca_com;                                          //通信实体
    int Mode;                                                       //制冷为0
    int Rate;
    Req* waiting_list;                                              //命令等待队列
    Database acdb;                                                  //数据库实体
};




#endif // CENTRALAC_H
