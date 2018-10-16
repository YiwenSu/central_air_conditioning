#include "database.h"

Database::Database()
{

}

bool Database::createConnection()
{
    //以后就可以用"sqlite1"与数据库进行连接了
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
    db.setDatabaseName(".//qtDb.db");
    if( !db.open())
    {
        qDebug() << "无法建立数据库连接"<<endl;
        return false;
    }
    return true;
}

bool Database::createRoominfo()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table roominfo\
                              (\
                              roomnumber integer,\
                              userid integer,\
                              start_date integer,\
                              end_date integer,\
                              start_time varchar(20),\
                              end_time varchar(20),\
                              target_temperatue integer,\
                              start_temperature integer,\
                              end_temperature integer,\
                              speed integer,\
                              fee numeric(10,2)\
                              )");

    if(success)
    {
        qDebug() << QObject::tr("数据库表roominfo创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表roominfo创建失败！\n");
        return false;
    }
}

bool Database::createCurrentinfo()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table currentinfo\
                              (\
                              roomnumber integer primary key,\
                              userid integer,\
                              current_temperature integer,\
                              current_energy numeric(5,1),\
                              speed integer,\
                              fee numeric(6,2),\
                              date varchar(20)\
                              )");

    if(success)
    {
        qDebug() << QObject::tr("数据库表Currentinfo创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表Currentinfo创建失败！\n");
        return false;
    }
}

bool Database:: createBufferinfo()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table bufferinfo\
                              (\
                              roomnumber integer,\
                              start_date integer,\
                              start_time varchar(20),\
                              start_temperature integer,\
                              target_temperature integer,\
                              energy numeric(5,1),\
                              speed integer\
                              )");

    if(success)
    {
        qDebug() << QObject::tr("数据库表Bufferinfo创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表Bufferinfo创建失败！\n");
        return false;
    }
}
bool Database::insert_roominfo(int roomnumber, int userid, int start_date, int end_date,
                               QString start_time, QString end_time,int start_temperature,
                               int end_temperature,int target_temperature,int speed,double energy)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare("insert into roominfo values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");//插入信息

        double fee = energy * 5;
        query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用

        bool success=query.exec();
        if(!success)
        {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("roominfo插入失败"));
            return false;
        }

    return true;
}


bool Database::insert_currentinfo(int roomnumber, int userid, int current_temperature,
                        int speed,  double fee, double energy, QString date)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare("insert into currentinfo values(?, ?, ?, ?, ?, ?, ?)");//插入信息

    query.bindValue(0,roomnumber);
    query.bindValue(1,userid);
    query.bindValue(2,current_temperature);
    query.bindValue(3,energy);
    query.bindValue(4,speed);
    query.bindValue(5,fee);
    query.bindValue(6,date);
    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo插入失败"));
        return false;
    }

return true;
}

bool Database::insert_bufferinfo(int roomnumber, int start_date, QString start_time,
                       int start_temperature,int target_temperature, int speed)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from bufferinfo where roomnumber = %1").arg(roomnumber));
    if(!query.exec())
    {
            qDebug() << "删除buffer记录失败！";
    }
    query.prepare("insert into bufferinfo values(?, ?, ?, ?, ?, ?, ?)");//插入信息

    query.bindValue(0,roomnumber);
    query.bindValue(1,start_date);
    query.bindValue(2,start_time);
    query.bindValue(3,start_temperature);
    query.bindValue(4,target_temperature);
    query.bindValue(5,0);//插入能量
    query.bindValue(6,speed);

    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("buffertinfo插入失败"));
        return false;
    }
    update_currentinfo_speed(roomnumber,speed);
    return true;
}
bool Database::queryAll(QString str,int num)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.exec(QString("select * from %1").arg(str));
   // QSqlRecord rec = query.record();
    //qDebug() << QObject::tr("automobil表字段数：" ) << rec.count();

    while(query.next())
    {
        for(int index = 0; index < num; index++)
            qDebug() << query.value(index) << " ";
        qDebug() << "\n";
    }
   return true;
}

bool Database::update_bufferinfo_energy(int roomnumber,int rate)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    query.prepare(QString("select energy, speed\
                          from bufferinfo\
                          where roomnumber = %1 ").arg(roomnumber));//修改
    query.exec();
    double energy;
    int speed;
    if(query.next())
    {
          energy = query.value(0).toDouble();
          speed = query.value(1).toInt();
    }

    double temp_energy;

    //qDebug() << speed;
    //qDebug() << energy;
    if(speed == 1)
        temp_energy = energy + (double)0.8 * rate/60;
    else if(speed == 2)
        temp_energy = energy + (double)1 * rate/60;
    else if (speed ==3)
        temp_energy = energy + (double)1.3 * rate/60;

    //qDebug() << temp_energy;
    query.prepare(QString("update bufferinfo set energy = :energy\
                                  where roomnumber= %1").arg(roomnumber));
    query.bindValue(":energy",temp_energy); //修改



     bool success=query.exec();
     if(!success)
     {
          QSqlError lastError = query.lastError();
          qDebug() << lastError.driverText() << QString(QObject::tr("buffer energy更新失败"));
          return false;
     }
    return true;
}

bool Database::update_bufferinfo_speed(int roomnumber, int speed)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update bufferinfo set speed = :speed\
                                  where roomnumber= %1").arg(roomnumber));

    query.bindValue(":speed",speed); //修改速度
    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("buffer speed更新失败"));
           return false;
     }
   return true;
}

bool Database::update_bufferinfo_target_temperature(int roomnumber, int target_temp)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update bufferinfo set target_temperature = :target\
                                  where roomnumber= %1").arg(roomnumber));

    query.bindValue(":target",target_temp); //修改速度
    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("buffer target_temp更新失败"));
           return false;
     }
   return true;
}

bool Database::update_currentinfo(int roomnumber)

{
    /*
     * *create table currentinfo\
                              (\
                              roomnumber integer primary key,\
                              userid integer,\
                              current_temperature integer,\
                              current_energy numeric(5,1),\
                              speed integer,\
                              fee numeric(6,2),\
                              date varchar(20)\
                              )/*/
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update currentinfo set speed = :sp\
                   where roomnumber = %1").arg(roomnumber));


    //query.bindValue(":usrid",userid);
    //query.bindValue(":current",current_temperature);
    //query.bindValue(":energy",0);
    query.bindValue(":sp",0);
    //query.bindValue(5,fee);
    //query.bindValue(6,date);
    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo update all失败"));
        return false;
    }

return true;
}


bool Database::update_currentinfo_energy_temperature_fee(int roomnumber, int rate, int current_temperature)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    if(rate == 1){
    query.prepare(QString("select current_energy, speed, fee\
                          from currentinfo\
                          where roomnumber = %1 ").arg(roomnumber));
    query.exec();

    double energy , fee;
    int speed;

    if(query.next()){
    energy = query.value(0).toDouble();
    speed = query.value(1).toInt();
    fee = query.value(2).toDouble();

    }


    if(speed == 1)
    {
        energy = energy + (double)0.8 * rate/60;
        fee = fee + (double)0.8 * 5 *rate/60;
    }
    else if(speed == 2)
    {
        energy = energy + (double)1 * rate/60;
        fee = fee + (double)1 * 5 *rate/60;
    }
    else if (speed ==3)
    {
        energy = energy + (double)1.3 * rate/60;
        fee = fee + (double)1.3 * 5 *rate/60;
    }
    query.prepare(QString("update currentinfo set current_energy = :energy,\
                          fee = :fee where roomnumber= %1").arg(roomnumber));
    query.bindValue(":energy",energy);
    //query.bindValue(":tem",current_temperature);
    query.bindValue(":fee",fee);
    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("current energy fee更新失败"));
           return false;
     }
    //qDebug()<<"energy:"<<energy<<endl;
    //qDebug()<<"speed:"<<speed<<endl;
    //qDebug()<<"fee:"<<fee<<endl;

   return true;

   }
    else
    {
    query.prepare(QString("update currentinfo set current_temperature = :tem\
                        where roomnumber= %1").arg(roomnumber));

    query.bindValue(":tem",current_temperature);

    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("current tem 更新失败"));
           return false;
     }

   return true;
    }
}

bool Database::update_currentinfo_speed(int roomnumber, int speed)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("update currentinfo set speed = :speed\
                                  where roomnumber= %1").arg(roomnumber));

    query.bindValue(":speed",speed); //修改速度
    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("current speed更新失败"));
           return false;
     }
   return true;
}

bool Database::update_roominfo(int roomnumber, QString end_time, int end_date, int end_temperature)
{  //从三表获取目标温度，风速，本次能量，开始时间，开始日期，开始温度
   //费用使用本次计算
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    query.prepare(QString("select start_date,start_time,start_temperature,\
                          target_temperature,energy,speed from bufferinfo\
                          where roomnumber = %1").arg(roomnumber));
    bool success_buffer = query.exec();
    if(!success_buffer)
    {
         QSqlError lastError = query.lastError();
         qDebug() << lastError.driverText() << QString(QObject::tr("bufferinfo信息提取失败"));
         return false;
    }
    int date;
    QString time;
    int start_temperature;
    int target_temperature;
    double energy;
    int speed;

    if(query.next())
    {
        date = query.value(0).toInt();
        time = query.value(1).toString();
        start_temperature = query.value(2).toInt();
        target_temperature = query.value(3).toInt();
        energy = query.value(4).toDouble();
        speed = query.value(5).toInt();
    }

    //qDebug()<<date<<endl<<time<<endl<<start_temperature<<endl<<target_temperature<<endl<<energy<<endl<<speed<<endl;

    query.prepare(QString("select userid from currentinfo\
                          where roomnumber = %1").arg(roomnumber));
    bool success_current = query.exec();
    if(!success_current)
    {
         QSqlError lastError = query.lastError();
         qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo信息提取失败"));
         return false;
    }
    int userid;
    if(query.next())
    {
       userid = query.value(0).toInt();
       qDebug()<<"id:"<<userid<<endl;
    }
    return insert_roominfo(roomnumber,userid,date,end_date,time,end_time,start_temperature,
                           end_temperature,target_temperature,speed,energy);
}

bool Database::check_currentinfo_id_room(int roomnumber, int userid)
{
    //在数据库中查找是否有
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);


    query.prepare(QString("select * from currentinfo where roomnumber = %1 and userid =%2").arg(roomnumber).arg(userid));
    bool success = query.exec();

    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("check room id 程序出错"));
        return false;
    }
    if(query.next())
    {
        qDebug() <<"got"<<endl;

        return true;
    }
    else
    {
        qDebug() <<"got nan"<<endl;
        return false;
    }
}

int Database:: find_target_temperature_from_bufferinfo(int roomnumber)
{
   //搜索目标温度
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);


    query.prepare(QString("select target_temperature from bufferinfo where roomnumber = %1").arg(roomnumber));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("target_temp程序出错"));
        return false;
    }
    if(query.next())
       return query.value(0).toInt();
    else
       return -1;
}

int Database::find_current_temperature_from_currentinfo(int roomnumber)
{
    //搜索current温度
     QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
     QSqlQuery query(db);


     query.prepare(QString("select current_temperature from currentinfo where roomnumber = %1").arg(roomnumber));
     bool success = query.exec();
     if(!success)
     {
         QSqlError lastError = query.lastError();
         qDebug() << lastError.driverText() << QString(QObject::tr("current_temp程序出错"));
         return false;
     }
     if(query.next())
        return query.value(0).toInt();
     else
        return -1;
}

bool Database:: delete_bufferinfo_room(int roomnumber, QString time, int date)
{
    //删除房间信息
    /*
     * "create table bufferinfo\
                              (\
                              roomnumber integer,\
                              start_date integer,\
                              start_time varchar(20),\
                              start_temperature integer,\
                              target_temperature integer,\
                              energy numeric(5,1),\
                              speed integer\
                              )"*/
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select target_temperature,start_temperature from bufferinfo where roomnumber = %1").arg(roomnumber));
    bool success_target = query.exec();
    if(!success_target){
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("查询-1 target失败"));
        return false;
    }

    int target;
    if(query.next())
          target = query.value(0).toInt();
    //--------------------------------------------------------------------------------------------------------
    query.prepare(QString("select userid,current_temperature from currentinfo\
                         where roomnumber = %1").arg(roomnumber));
   bool success_current = query.exec();
   if(!success_current)
   {
       QSqlError lastError = query.lastError();
       qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo信息delete484提取失败"));
       return false;
   }
   int userid;
   int current_temperature;
   if(query.next())
    {
        //insert_roominfo(roomnumber,userid,date,date,time,time,-1,-1,-1,-1,0)
        userid = query.value(0).toInt();
        current_temperature = query.value(1).toInt();
       // qDebug()<<"成功删除 id:"<<userid<<endl;
    }

    //===========================================================================================================
    if(target != -1 && current_temperature != -1 )//modify
    {
       query.prepare(QString("select start_date,start_time,start_temperature,target_temperature,energy,speed from bufferinfo\
                              where roomnumber = %1").arg(roomnumber));
      bool success_buffer = query.exec();
      if(!success_buffer)
       {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("currentinfo信息delete484提取失败"));
            return false;
       }

      if(query.next()){
      int start_date = query.value(0).toInt();
      QString start_time = query.value(1).toString();
      int start_temperature = query.value(2).toInt();
      int target_temperature = query.value(3).toInt();
      double energy = query.value(4).toDouble();
      int speed = query.value(5).toInt();
      insert_roominfo(roomnumber,userid,start_date,date,start_time,time,start_temperature,current_temperature,target_temperature,speed,energy);
      }
    }
    else
        insert_roominfo(roomnumber,userid,date,date,time,time,-1,-1,-1,-1,0);//设置风速为-1
    //================================================================================================================================
    query.prepare(QString("delete from bufferinfo where roomnumber = %1").arg(roomnumber));

     bool success = query.exec();

     if(!success)
     {
         QSqlError lastError = query.lastError();
         qDebug()<<lastError.driverText()<<QString(QObject::tr("删除失败"));
         return false;
     }
     return true;
}

QSqlQuery Database::search_table_year(int year)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int year1 = year* 10000;
    int year2 = year * 10000 +10000;
    query.prepare(QString("select * from roominfo \
                           where start_date > %1 and start_date < %2 \
                            and speed > 0 ").arg(year1).arg(year2));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("构造表year出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::search_table_month(int year, int month)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int time = year* 10000 + month * 100;
    int time1 = year* 10000 + month * 100 + 100;
    query.prepare(QString("select * from roominfo where start_date > %1 and start_date < %2 \
                          and speed > 0 ").arg(time).arg(time1));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("构造表year month出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::search_table_date(int year, int month, int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int time = year* 10000 + month * 100 + day;

    query.prepare(QString("select * from roominfo where start_date = %1\
                          and speed > 0 ").arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("构造表year month day出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::get_opentime_per_room_from_roominfo_year(int year)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    int year1 = year* 10000;
    int year2 = year* 10000 + 10000;
     query.prepare(QString("select roomnumber,count(*) from roominfo\
                           where start_date > %1 and start_date < %2 and speed = -1\
                           group by roomnumber").arg(year1).arg(year2));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << query.lastError().text() << QString(QObject::tr("year open出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::get_opentime_per_room_from_roominfo_month(int year,int month)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    int time = year* 10000 + month * 100;
    int time1 = year* 10000 + month * 100 + 100;
    query.prepare(QString("select roomnumber,count(*) \
                          from roominfo\
                          where start_date > %1 and start_date < %2 \
                          and speed = -1\
                          group by roomnumber").arg(time).arg(time1));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("month open出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::get_opentime_per_room_from_roominfo_date(int year,int month,int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);


    int time = year* 10000 + month * 100 + day;

    query.prepare(QString("select roomnumber,count(*)\
                          from roominfo where start_date = %1 \
                          and speed =-1\
                          group by roomnumber ").arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("date open出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database:: get_user_info_from_current(int roomnumber,int userid)
{
    /*
     *  query.bindValue(0,roomnumber);
    query.bindValue(1,userid);
    query.bindValue(2,current_temperature);
    query.bindValue(3,energy);
    query.bindValue(4,speed);
    query.bindValue(5,fee);
    query.bindValue(6,date);*/
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    query.prepare(QString("select * from currentinfo where roomnumber = %1 and userid = %2").arg(roomnumber).arg(userid));

    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("get userinfo出错"));
        return query;
    }
    else
        return query;


}

bool  Database:: delete_currentinfo_user(int roomnumber,int userid)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from currentinfo where roomnumber= %1 and userid = %2").arg(roomnumber).arg(userid));


    bool success=query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("current 删除失败"));
           return false;
     }
   return true;
}

QSqlQuery Database ::search_table_week(int year, int month, int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int time = year* 10000 + month * 100 + day;
    int time1 = year *10000 + month * 100 + day - 6;
    query.prepare(QString("select * from roominfo where start_date >= %1 and start_date <= %2\
                          and speed > 0 ").arg(time1).arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("构造表week出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database ::get_opentime_per_room_from_roominfo_week(int year,int month,int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);


    int time = year* 10000 + month * 100 + day;
    int time1 = year* 10000 + month * 100 + day - 6;

    query.prepare(QString("select roomnumber,count(*)\
                          from roominfo where start_date >= %1 and start_date <=%2\
                          and speed =-1\
                          group by roomnumber ").arg(time1).arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("month open出错"));
        return query;
    }
    else
        return query;
}

QSqlQuery Database::get_roomnumber_from_current()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    query.prepare(QString("select roomnumber from currentinfo"));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("get roomnumber from currentinfo出错"));
        return query;
    }
    else
        return query;

}

QSqlQuery Database ::get_room_energy_fee_from_current()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select roomnumber,current_energy,fee from currentinfo"));
    bool success = query.exec();
    if(!success)
    {
           QSqlError lastError = query.lastError();
           qDebug() << lastError.driverText() << QString(QObject::tr("get_room_energy_fee_from_current出错"));
           return query;
    }
    else
           return query;
}

bool Database ::change_element_current_buffer(int roomnumber,int current_temperature)
{
    update_currentinfo_energy_temperature_fee(roomnumber,1,current_temperature);
    update_bufferinfo_energy(roomnumber,1);
}

double Database::get_fee_year(int year)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    int year1 = year* 10000;
    int year2 = year * 10000 +10000;
    query.prepare(QString("select sum(fee) from roominfo \
                           where start_date > %1 and start_date < %2 \
                            and speed > 0 ").arg(year1).arg(year2));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("表year算fee account出错"));
        return -100;
    }
    else
    {
         if(query.next())
               return query.value(0).toDouble();
    }
}

double Database ::get_fee_month(int year, int month)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int time = year* 10000 + month * 100;
    int time1 = year* 10000 + month * 100 + 100;
    query.prepare(QString("select sum(fee) from roominfo where start_date > %1 and start_date < %2 \
                          and speed > 0 ").arg(time).arg(time1));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("表month算fee account出错"));
        return -100;
    }
    else
    {
         if(query.next())
               return query.value(0).toDouble();
    }
}

double Database ::get_fee_date(int year, int month, int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    int time = year* 10000 + month * 100 + day;

    query.prepare(QString("select sum(fee) from roominfo where start_date = %1\
                          and speed > 0 ").arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("表date算fee account出错"));
        return -100;
    }
    else
    {
         if(query.next())
            return query.value(0).toDouble();
    }
}

double Database ::get_fee_week(int year, int month, int day)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    int time = year* 10000 + month * 100 + day;
    int time1 = year *10000 + month * 100 + day - 6;
    query.prepare(QString("select sum(fee) from roominfo where start_date >= %1 and start_date <= %2\
                          and speed > 0 ").arg(time1).arg(time));
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("构造表week出错"));
         return -100;
    }
    else
     {
        if(query.next())
            return query.value(0).toDouble();
     }
}

bool Database :: find_room_from_current(int roomnumber)
{

    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("select *\
                           from currentinfo where roomnumber = %1 and speed <> -1").arg(roomnumber));
    bool success =  query.exec();
    if(success)
        if(query.next())
             return true;
    else
        return false;
}
