#include "base_widget.h"
#include <QApplication>
#include "account.h"
std::mutex db_mutex;
std::mutex list_mutex;
int main(int argc, char *argv[])
{
   // Database mdb;
    //CentralAC ac;
    //mdb.createConnection();
    //mdb.createRoominfo();
    //mdb.createCurrentinfo();
    //mdb.createBufferinfo();
    /*mdb.insert_currentinfo(111,234,30,0,0,0,ac.getdate_string());
    mdb.insert_currentinfo(222,237,30,0,0,0,ac.getdate_string());
    mdb.insert_currentinfo(333,238,30,0,0,0,ac.getdate_string());
    ac.setRoomTmpReq(222,1,25);
    ac.setRoomTmpReq(222,1,25);
    ac.setRoomTmpReq(222,1,25);
    ac.setRoomTmpReq(222,1,25);
    ac.setRoomTmpReq(111,1,25);
    ac.setRoomTmpReq(333,1,25);
    ac.setRoomTmpReq(333,1,25);
    mdb.queryAll("roominfo",11);
    return 0;
    //mdb.insert_currentinfo(333,238,30,23,24,99,ac.getdate_string());
    mdb.insert_roominfo(11,11,20180606,20180606,"19:00","20:00",23,23,23,3,3);
    mdb.insert_roominfo(11,12,20180607,20180609,"19:00","20:00",23,23,23,3,3);
    mdb.insert_roominfo(11,11,20180606,20180606,"19:00","20:00",-1,-1,-1,-1,0);
    mdb.insert_roominfo(11,11,20180601,20180606,"19:00","20:00",23,23,23,3,3);
    mdb.insert_currentinfo(333,238,30,666,66,99,ac.getdate_string());*/
    QApplication a(argc, argv);
   base_Widget w;
     //Account testa;
     //mdb.queryAll("roominfo",11);
    //mdb.queryAll("roominfo",11);
  // testa.get_bill_room_user(333,238);
  //  testa.get_account_month(2018,6);
    w.show();
    return a.exec();
}
