#ifndef DATABASE_H
#define DATABASE_H

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QString>
#include <QDateTime>
#include <QObject>
class Database
{
public:
    Database();
    bool createConnection();
    bool createRoominfo();                                                                       //创建总表，风速温度目标开始计费
    bool createCurrentinfo();
    bool createBufferinfo();
    bool insert_roominfo(int roomnumber, int userid, int start_date, int end_date,
                         QString start_time, QString end_time, int start_temperature,
                         int end_temperature, int target_temperature, int speed, double energy); //插入报表数据

    bool insert_currentinfo(int roomnumber,int userid, int current_temperature,
                            int speed, double fee, double energy,QString date);                               //插入房间数据

    bool insert_bufferinfo(int roomnumber, int start_date, QString start_time, int start_temperature,
                           int target_temperature, int speed);                                   //插入缓冲表数据

    bool queryAll(QString str, int num);                                                         //查询所有信息,1表 11， 二表 6，三表7

    bool update_bufferinfo_energy(int roomnumber, int rate);                                     //按频率更新能量
    bool update_bufferinfo_speed(int roomnumber, int speed);                                     //更新buffer表风速
    bool update_bufferinfo_target_temperature(int roomnumber, int target_temp);                  //更新 buffer目标温度

    bool update_currentinfo_energy_temperature_fee(int roomnumber, int rate,
                                                   int current_temperature);                     //更新current表能量，费用，温度

    bool update_currentinfo_speed(int roomnumber, int speed);                                     //更新current表风速
    bool update_roominfo(int roomnumber, QString end_time, int end_date, int end_temperature);    //更新room表
    int find_target_temperature_from_bufferinfo(int roomnumber);                                  //返回目标温度,找不到则返回-1
    bool check_currentinfo_id_room(int roomnumber, int userid);                                   //查看表内是否存在用户
    int find_current_temperature_from_currentinfo(int roomnumber);                                //查看当前温度
    QSqlQuery search_table_year(int year);
    QSqlQuery search_table_month(int year, int month);
    QSqlQuery search_table_date(int year, int month, int day);
    QSqlQuery search_table_all();
    QSqlQuery search_table_week(int year, int month, int day);

    bool delete_bufferinfo_room(int roomnumber,QString time, int date); //删除buffer

    QSqlQuery get_opentime_per_room_from_roominfo_year(int year);//计算空调使用次数
    QSqlQuery get_opentime_per_room_from_roominfo_month(int year,int month);//计算空调使用次数
    QSqlQuery get_opentime_per_room_from_roominfo_date(int year,int month,int day);//计算空调使用次数
    QSqlQuery get_user_info_from_current(int roomnumber,int userid);//获取房间用户信息
    QSqlQuery get_opentime_per_room_from_roominfo_week(int year,int month,int day);

    QSqlQuery get_roomnumber_from_current();
    QSqlQuery get_room_energy_fee_from_current();
    bool delete_currentinfo_user(int roomnumber,int userid);
    bool change_element_current_buffer(int roomnumber,int current_temperature);    //周期改值




    double get_fee_year(int year);
    double get_fee_month(int year, int month);
    double get_fee_date(int year, int month, int day);
    double get_fee_week(int year, int month, int day);

    bool update_currentinfo(int roomnumber);
    bool find_room_from_current(int roomnumber); //查找非关机的房间
};

#endif // DATABASE_H
