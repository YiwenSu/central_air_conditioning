#ifndef ACCOM_H
#define ACCOM_H
#include <QString>
class ACCom
{
public:
    ACCom();
    //ACCom(QString serverip,int serverport,AC* ac);
    void run();
    void close();
    void sendCloseReq();
    bool sendIdInfo(int roomNumber,int userid);						//发送认证信息，包括房间号和用户id,并等待认证结果返回，设置从控机工作模式和缺省温度
    bool sendTmpReq(int speed,int temperature);							//发送温控请求,speed有无风(0)，低速(1)，中速(2)，高速(3)
};

#endif // ACCOM_H
