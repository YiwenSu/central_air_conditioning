#include "currentinfo.h"
#include <mutex>
extern std::mutex db_mutex;
currentinfo::currentinfo(int roomNumber,int uid,int temperature,
                         double energy, int Speed,double money,QString day)
{
    this->roomnumber = roomNumber;
    this->userid = uid;
    this->current_temperature = temperature;
    this->current_energy = energy;
    this->speed = Speed;
    this->fee = money;
    this->date = day;
}
int currentinfo::get_roomnumber()
{
    return this->roomnumber;
}
int currentinfo::get_userid()
{
    return this->userid;
}
int currentinfo::get_current_temperature()
{
    return this->current_temperature;
}
double currentinfo::get_current_energy()
{
    return this->current_energy;
}
int currentinfo::get_speed()
{
    return this->speed;
}
double currentinfo::get_fee()
{
    return this->fee;

}
QString currentinfo::get_date()
{
    return this->date;
}
vector<currentinfo> currentinfo::get_roominfo_total()
{
    db_mutex.lock();
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     * "create table currentinfo\
                              (\
                              roomnumber integer,\
                              userid integer,\
                              current_temperature integer,\
                              current_energy numeric(5,1),\
                              speed integer,\
                              fee numeric(6,2),\
                              date varchar(20)\
                              )"*/
    query.prepare(QString("select roomnumber,userid,current_temperature,\
                           current_energy,speed,fee,date\
                           from currentinfo\
                           order by roomnumber"));
    int roomNumber;
    int uid;
    int temperature;
    double energy;
    int speed;
    double money;
    QString day;
    bool success = query.exec();
    vector <currentinfo> temp;
    if(success)
    {
         while(query.next())
         {
             roomNumber = query.value(0).toInt();
             uid = query.value(1).toInt();
             temperature = query.value(2).toDouble();
             energy = query.value(3).toDouble();
             speed = query.value(4).toInt();
             money = query.value(5).toDouble();
             day = query.value(6).toString();
             currentinfo a = currentinfo(roomNumber,uid,temperature,energy,speed,money,day);
             temp.push_back(a);
         }
         db_mutex.unlock();
         return temp;
    }
    else
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo::get_roominfo_total()提取失败"));
        db_mutex.unlock();
        return temp;
    }
}

