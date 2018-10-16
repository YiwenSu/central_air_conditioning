#ifndef REQ_H
#define REQ_H
#include<QString>
#include <QDateTime>
class Req
{
public:
    Req();
    Req* next;
    int Req_temperature;
    int Req_roomnumber;
    int Req_speed;
    int prior;
    int start_time;
    int get_current_time_second();
    Req* Find_req(Req* &waiting_list,int max_prior);
};

#endif // REQ_H
