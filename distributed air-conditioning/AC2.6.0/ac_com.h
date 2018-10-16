#ifndef AC_COM_H
#define AC_COM_H
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <iostream>
#include <time.h>
#include <QDebug>

class AC;

class AC_Com
{
public:
    AC_Com();
    ~AC_Com();
    AC_Com(std::string serverip, int serverport, AC* ac);
    void run();
    void close();
    void sendIdInfo(int roomNumber, int userid);						//发送认证信息，包括房间号和用户id
    void sendTmpReq(int mode,int speed, int temperature);							//发送温控请求,speed有无风(0)，低速(1)，中速(2)，高速(3)
    void sendRoomTmp(int temperature);
    void sendCloseReq();

    bool isRunning();
    SOCKET getSocket();
    sockaddr_in getServerAddr();
private:
    AC *m_ac;
    SOCKET m_socket;	/*自己的套接字*/
    sockaddr_in m_sockaddr_in;	/*自己的IP地址和端口信息*/
    sockaddr_in m_serveraddr_in;	/*服务器的IP地址和端口信息*/
    std::string m_serverip;
    int m_serverport;
    std::thread* m_thread;
    bool m_running;


    void initSocket();					/*初始化套接字*/
};

#endif
