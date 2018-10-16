#include "req.h"
#include<QDebug>
#include <vector>
using namespace std;

Req::Req()
{
    next=NULL;
}

int Req::get_current_time_second()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    return timeT;
}

Req* Req::Find_req(Req *&waiting_list, int max_prior)
{
    Req* room_prior=NULL;
    Req* temp_ptr=NULL;
    if(waiting_list==NULL)
    {
        return NULL;
    }
    if(!max_prior)
        max_prior=waiting_list->prior;
    vector<Req*>list_vc;
    for(temp_ptr=waiting_list;temp_ptr;temp_ptr=temp_ptr->next)
    {
        list_vc.push_back(temp_ptr);
    }
    int temp=-1;
    for(int i=0;i<list_vc.size();i++)
    {
        if(list_vc[i]->prior<=max_prior)
        {
            max_prior=list_vc[i]->prior;
            temp=i;
        }
    }
    if(temp==-1)
        return NULL;
    else
    {
        room_prior=list_vc[temp];
        list_vc.erase(list_vc.begin() + temp);
        waiting_list=NULL;
        Req* ptr=waiting_list;
        if(list_vc.empty())
        {
            waiting_list=NULL;
            return room_prior;
        }
        else
        {
            ptr=list_vc[0];
            waiting_list=list_vc[0];
            waiting_list->next=NULL;
            for(int i=1;i<list_vc.size();i++)
            {
                ptr->next=list_vc[i];
                ptr->next->next=NULL;
                ptr=ptr->next;
            }
            return room_prior;
        }
    }

    /*
    max_prior=0;
    for(Req* temp=waiting_list;temp;temp=temp->next)
    {
        if((temp->next==room_prior)&&(temp->next))
        {
            if(room_prior)
            {
                temp->next=room_prior->next;
                max_prior=1;
            }
            else
            {
                max_prior=1;
            }
        }
    }
    if(!max_prior&&(waiting_list)&&(room_prior))
    {
        waiting_list=waiting_list->next;
    }*/
    //return room_prior;
}
