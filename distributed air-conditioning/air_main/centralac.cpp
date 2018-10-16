#include "centralac.h"
#include "database.h"
extern std::mutex db_mutex;
extern std::mutex list_mutex;
CentralAC::CentralAC()
{
    list_mutex.lock();
    this->waiting_list = NULL;
    for(int i=0;i<3;i++)
    {
        CPU_preempt[i]=NULL;
    }
    list_mutex.unlock();
    get_system_time();
    qDebug()<<"CentralAC()";
    this->Ca_com = new CentralAC_Com(10000,this);//端口号要进行修改
}

CentralAC::~CentralAC()
{

     //delete Ca_com;

}

void CentralAC::run()
{
    this->Ca_com->run();
    this->Switch = 1;
    this->Rate = 1;//刷新时间为1s
    this->Mode = 0; //制冷
}

void CentralAC::close()
{
    if(this->Switch)
    {
        this->Ca_com->close();
        //delete this->Ca_com;
        this->Switch = 0;
    }
    else
    {
        qDebug()<<"空调已关闭，请勿重复关闭";
    }
}

int CentralAC::getswitch()
{
    return this->Switch;
}
void CentralAC::setRoomTmp(int roomNumber,int temperature)
{
    db_mutex.lock();
    acdb.update_currentinfo_energy_temperature_fee(roomNumber,0,temperature);
    db_mutex.unlock();
}

void CentralAC::setRoomTmpReq(int roomNumber, int mode, int speed, int temperature)
{
    //查看所有消息
    /*db_mutex.lock();
    qDebug()<<"When REQUEST,Room temperature:"<<acdb.find_current_temperature_from_currentinfo(roomNumber);
    acdb.queryAll("bufferinfo",7);
    db_mutex.unlock();*/
    qDebug()<<"REQUEST Message: NUMBER:"<<roomNumber<<";\tMODE:"<<mode<<";\tSpeed:"<<speed<<";\tTemper:"<<temperature;
    bool flag;
    db_mutex.lock();
    flag = acdb.find_room_from_current(roomNumber);
    db_mutex.unlock();
   // int current_temperature;

    if(flag == true)
    {

        if(mode!=Mode)
            speed=0;                        //  等价为0风请求
        if(speed==0)
        {
            db_mutex.lock();
            acdb.update_currentinfo_speed(roomNumber,0);
            db_mutex.unlock();
            list_mutex.lock();
            Req* temp_req=waiting_list;
            if(temp_req)
            {
                while(temp_req->next!=NULL)
                {
                    if((temp_req->next->Req_roomnumber==roomNumber))
                    {
                        temp_req->next=temp_req->next->next;
                        temp_req=temp_req->next;
                    }
                    else
                    {
                        temp_req=temp_req->next;
                    }
                    if(temp_req==NULL)
                        break;
                }
            }
                if(waiting_list)
                    if(waiting_list->Req_roomnumber==roomNumber)
                        waiting_list=waiting_list->next;

            for(int i=0;i<3;i++)
            {
                if(CPU_preempt[i]!=NULL)
                {
                    if(CPU_preempt[i]->Req_roomnumber==roomNumber)
                    {
                        Req* new_req = NULL;
                        new_req=new_req->Find_req(waiting_list,0);
                        db_mutex.lock();
                        acdb.delete_bufferinfo_room(CPU_preempt[i]->Req_roomnumber,gettime(),getdate());
                        db_mutex.unlock();
                        CPU_preempt[i]=NULL;
                        if(new_req)
                        {
                            CPU_preempt[i]=new_req;
                            db_mutex.lock();
                            acdb.insert_bufferinfo(new_req->Req_roomnumber,getdate(),gettime(),acdb.find_current_temperature_from_currentinfo(new_req->Req_roomnumber),
                                                   new_req->Req_temperature,new_req->Req_speed);
                            db_mutex.unlock();
                        }
                    }
                }
            }
            list_mutex.unlock();
        }
        else
        {
            if(Mode)                                //当设置为供暖时，供暖温度控制在25°C～30°C之间.
            {
                if(!((25<=temperature)&&(temperature<=30)))
                    return;
            }
            else                                     //当设置为制冷时，制冷温度控制在18°C～25°C之间。
            {
                if(!((18<=temperature)&&(temperature<=25)))
                    return;
            }
            if(!((speed==1)||(speed==2)||(speed==3)))
                return;
            list_mutex.lock();
            Req* new_req=new Req;
            QString time_now = gettime();
            new_req->Req_roomnumber=roomNumber;
            new_req->Req_speed=speed;
            new_req->Req_temperature=temperature;
            new_req->start_time=new_req->get_current_time_second();
            new_req->prior=(new_req->start_time-system_time)*2;
            new_req->next=NULL;
            int date_now = getdate();
            int flag=0;
            if(waiting_list!=NULL)
            {
                Req* temp_ptr=NULL;
                for(temp_ptr=waiting_list;temp_ptr;temp_ptr=temp_ptr->next)
                {
                    if(temp_ptr->Req_roomnumber==new_req->Req_roomnumber)
                    {
                        temp_ptr->Req_roomnumber=new_req->Req_roomnumber;
                        temp_ptr->Req_speed=new_req->Req_speed;
                        temp_ptr->Req_temperature=new_req->Req_temperature;
                        temp_ptr->start_time=new_req->start_time;
                        temp_ptr->prior=new_req->prior;
                        flag=1;
                        delete new_req;
                        new_req=NULL;
                        break;
                    }
                }
            }
            if(!flag)
            {
                int i,j,z;
                for(i=0,j=0,z=-1;i<3;i++)                // 寻找当前执行的指令中是否有相同指令
                {
                    if(CPU_preempt[i])
                    {
                        if(CPU_preempt[i]->Req_roomnumber==roomNumber)
                        {
                            j=i;
                            i=4;
                        }
                    }
                    else
                    {
                        z=i;
                    }
                }
                if(i==5)                                 //如果有的话更新第三张表，把第三张表中原有信息传给第一张表
                {
                    db_mutex.lock();
                    acdb.delete_bufferinfo_room(CPU_preempt[j]->Req_roomnumber,gettime(),getdate());
                    acdb.insert_bufferinfo(CPU_preempt[j]->Req_roomnumber,getdate(),gettime(),
                                           acdb.find_current_temperature_from_currentinfo(CPU_preempt[j]->Req_roomnumber),new_req->Req_temperature,new_req->Req_speed);
                    db_mutex.unlock();
                     CPU_preempt[j]=new_req;
                }
                else                                   //如果没有的话，加入指令序列尾部
                {
                    if(!waiting_list)
                    {
                        waiting_list=new_req;
                    }
                    else
                    {
                        new_req->next=waiting_list;
                        waiting_list=new_req;
                    }
                }
                if(z!=-1)
                {
                    new_req=new_req->Find_req(waiting_list,0);
                    if(new_req)
                    {
                        db_mutex.lock();
                        acdb.insert_bufferinfo(new_req->Req_roomnumber,date_now,time_now,
                                          acdb.find_current_temperature_from_currentinfo(new_req->Req_roomnumber),new_req->Req_temperature,new_req->Req_speed);
                        db_mutex.unlock();
                        CPU_preempt[z]=new_req;
                        qDebug() <<"成功加入执行序列" <<CPU_preempt[z]->Req_roomnumber;
                    }
                }
            }
            list_mutex.unlock();
        }

    }
    else
        qDebug()<<"**CLOSE NO WIND: REQUEST Message: NUMBER:"<<roomNumber<<";\tMODE:"<<mode<<";\tSpeed:"<<speed<<";\tTemper:"<<temperature;

}

int CentralAC::getMode()
{
    return this->Mode;
}
void CentralAC::setMode(int mode)
{
    this->Mode = mode;
}

bool CentralAC::checkIdInfo(int roomNumber,int userid)
{
    //输出登陆信息
    qDebug()<<"LOGIN_MESSAGE NUMber:"<<roomNumber<<"\tUserid:"<<userid;
    db_mutex.lock();
    if(acdb.check_currentinfo_id_room(roomNumber,userid))
    {
        Ca_com->sendRoomTmpReq(roomNumber);
        Ca_com->sendIdCheckRes(roomNumber,true,Mode);

            acdb.update_currentinfo(roomNumber);
        db_mutex.unlock();
        return true;
    }
    else
    {
        Ca_com->sendIdCheckRes(roomNumber,false,Mode);
        db_mutex.unlock();
        return false;
    }
}
void CentralAC::setInterval(int rate)
{
    this->Rate =  rate;
}

int CentralAC::getInterval()
{
    return this->Rate;
}

void CentralAC::get_system_time()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    system_time=timeT-5000;
}

QString CentralAC::gettime()
{
    //获得当前时间
    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("hh:mm:ss");
     //qDebug()<<strBuffer;
    return strBuffer;
}

int CentralAC::getdate()
{
   //获得当前日期
    QString strBuffer;
    QDateTime time;

    time = QDateTime::currentDateTime();

    strBuffer = time.toString("yyyyMMdd");
    //qDebug()<<strBuffer.toInt();
    return strBuffer.toInt();
}

void CentralAC::ac_timeout()
{
    list_mutex.lock();
    Req *new_req=NULL;
    int date_now = getdate();
    int time_now=new_req->get_current_time_second()-system_time;
   // qDebug()<<"优先级当前时间:"<<time_now;
    for(new_req=waiting_list;new_req;new_req=new_req->next)              // 更新优先级
    {
        new_req->prior=(new_req->start_time-system_time)*2-time_now;
    }
    int min_priority,j=3;
    if(CPU_preempt[0])
        min_priority = CPU_preempt[0]->prior;
    else
        min_priority = 0;
    for(int i=0;i<3;i++)
    {
        if(CPU_preempt[i])
        {
            if(min_priority<=CPU_preempt[i]->prior)
            {
                min_priority=CPU_preempt[i]->prior;
                j=i;
            }
        }
    }
    new_req=new_req->Find_req(waiting_list,min_priority);
    if(new_req)                                                         // 抢占命令
    {
        db_mutex.lock();
        acdb.insert_bufferinfo(new_req->Req_roomnumber,date_now,gettime(),acdb.find_current_temperature_from_currentinfo(new_req->Req_roomnumber),
                               new_req->Req_temperature,new_req->Req_speed);
        db_mutex.unlock();
        if(j!=3)
        {
            db_mutex.lock();
            acdb.delete_bufferinfo_room(CPU_preempt[j]->Req_roomnumber,gettime(),getdate());
            db_mutex.unlock();
            CPU_preempt[j]->next=waiting_list;
            waiting_list=CPU_preempt[j];
            waiting_list->prior=time_now;
            waiting_list->start_time=new_req->get_current_time_second();
            CPU_preempt[j]=new_req;
        }
    }
    for(int i=0;i<3;i++)
    {
        if(CPU_preempt[i])
        {
            Ca_com->sendAirBag(CPU_preempt[i]->Req_roomnumber,CPU_preempt[i]->Req_speed);
            //qDebug()<<"send:"<<CPU_preempt[i]->Req_roomnumber;
            db_mutex.lock();
            acdb.change_element_current_buffer(CPU_preempt[i]->Req_roomnumber,CPU_preempt[i]->Req_temperature);
            db_mutex.unlock();
        }
    }
    list_mutex.unlock();
}

void CentralAC::temper_timeout()
{
    //qDebug()<<"二号定时器已成功执行！";
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.get_room_energy_fee_from_current();
    double energy,money;

    while(query.next())
    {
        int room = query.value(0).toInt();
        Ca_com->sendRoomTmpReq(room);
        energy = query.value(1).toDouble();
        money = query.value(2).toDouble();
        Ca_com->sendEnergyAndMoney(room,energy,money);
    }
    db_mutex.unlock();
}

void CentralAC::roomLogOut(int roomNumber)
{
    list_mutex.lock();
    vector<Req*>remove_vc;
    for(Req *temp=waiting_list;temp;temp=temp->next)        //删除掉等待队列中的请求
    {
        remove_vc.push_back(temp);
    }
    for(vector<Req*>::iterator it = remove_vc.begin();it != remove_vc.end();){
        if((*it)->Req_roomnumber == roomNumber ){
            it = remove_vc.erase(it);
        }
        else{
            it++;
        }
    }
    waiting_list=NULL;
    Req* ptr=waiting_list;
    if(remove_vc.empty())
    {
        waiting_list=NULL;
    }
    else
    {
        ptr=remove_vc[0];
        waiting_list=remove_vc[0];
        waiting_list->next=NULL;
        for(int i=1;i<remove_vc.size();i++)
        {
            ptr->next=remove_vc[i];
            ptr->next->next=NULL;
            ptr=ptr->next;
        }
    }
    int j=3;
    for(int i=0;i<3;i++)                                    //删除掉正在执行的请求
    {
        if(CPU_preempt[i])
        {
            if(CPU_preempt[i]->Req_roomnumber==roomNumber)
            {
                db_mutex.lock();
                acdb.delete_bufferinfo_room(CPU_preempt[i]->Req_roomnumber,gettime(),getdate());
                acdb.insert_bufferinfo(CPU_preempt[i]->Req_roomnumber,getdate(),gettime(),-1,-1,-1);       //发送关机记录
                acdb.delete_bufferinfo_room(CPU_preempt[i]->Req_roomnumber,gettime(),getdate());
                acdb.update_currentinfo_speed(CPU_preempt[i]->Req_roomnumber,-1);
                db_mutex.unlock();
                delete CPU_preempt[i];
                CPU_preempt[i]=NULL;
                j=i;
                i=3;
            }
        }
    }
    if(j!=3)
    {
        Req *new_req=new_req->Find_req(waiting_list,0);
        if(new_req)
        {
            db_mutex.lock();
            acdb.insert_bufferinfo(new_req->Req_roomnumber,getdate(),gettime(),
                              acdb.find_target_temperature_from_bufferinfo(new_req->Req_roomnumber),new_req->Req_temperature,new_req->Req_speed);
            db_mutex.unlock();
            CPU_preempt[j]=new_req;
        }
    }
    else
    {
        db_mutex.lock();
        acdb.insert_bufferinfo(roomNumber,getdate(),gettime(),-1,-1,-1);       //发送关机记录
        acdb.delete_bufferinfo_room(roomNumber,gettime(),getdate());
        acdb.update_currentinfo_speed(roomNumber,-1);
        db_mutex.unlock();
    }
    list_mutex.unlock();
}

QString CentralAC:: getdate_string()
{
    QString strBuffer;
    QDateTime time;

    time = QDateTime::currentDateTime();

    strBuffer = time.toString("yyyy-MM-dd");
    return strBuffer;
}

int CentralAC:: add_rate()
{
    this->Rate =this->Rate + 1;
    this->temper_timer->stop();
    this->temper_timer->start(this->getInterval()*1000);
    return this->Rate;
}

int CentralAC:: sub_rate()
{
    if(this->Rate < 1)
    {
        qDebug()<<"has been limited rate"<<endl;
        return this->Rate;
    }
    else
    {
         this->Rate =this->Rate - 1;
         this->temper_timer->stop();
         this->temper_timer->start(this->getInterval()*1000);
         return this->Rate;
    }

}

int CentralAC::change_mode_to_cold()
{
    this->Mode = 0;
    clear_buffer();
    return this->Mode;
}


int CentralAC::change_mode_to_warm()
{
    this->Mode = 1;
    clear_buffer();
    return this->Mode;
}

void CentralAC::clear_buffer()
{
    list_mutex.lock();
    for(Req* temp=waiting_list;temp;)
    {
        Req* flag=temp;
        temp=temp->next;
        delete flag;
        flag=NULL;
    }
    waiting_list=NULL;
    for(int i=0;i<3;i++)
    {
        if(CPU_preempt[i])
        {
            db_mutex.lock();
            acdb.delete_bufferinfo_room(CPU_preempt[i]->Req_roomnumber,gettime(),getdate());
            db_mutex.unlock();
            delete CPU_preempt[i];
            CPU_preempt[i]=NULL;
        }
    }
    list_mutex.unlock();
}

void CentralAC::set_row(QTableWidget* table)
{

    /*qDebug()<<"j:"<<table->rowCount();
    for(int i=0;i<3;i++)
    {
        if(CPU_preempt[i])
        {
            for(int j=0;j<9;j++)
            {
                if(CPU_preempt[i]->Req_roomnumber==table->item(j,0)->text().toInt())
                {
                      table->item(j,0)->setTextColor(RGB(200,111,100));
                }
               qDebug()<<"j:"<<j;
            }
        }
    }*/

}


vector<int> CentralAC::get_CPU()
{
    list_mutex.lock();
    vector<int> temp;
    for(int i=0;i<3;i++)
    {
        if(CPU_preempt[i])
        {
            temp.push_back(CPU_preempt[i]->Req_roomnumber);
        }
    }
    list_mutex.unlock();
    return temp;
}
/*bool CentralAC:: judge_temperature(int temp, int target)
{
    if(Mode == 0)//制冷
        if(target >= temp)
            return true;
    else
         if(target <= temp)
             return true;
    return false;
}*/


