#ifndef CENTRALAC_COM_H
#define CENTRALAC_COM_H


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <unordered_map>
#include <stdio.h>
#include <thread>
#include <string>
#include <time.h>

#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)





class CentralAC;

class CentralAC_Com
{
public:
    CentralAC_Com();
    ~CentralAC_Com();
    CentralAC_Com(int serverport, CentralAC *centralAC);
    void run();																		//开启通信，响应从控机的请求
    void close();																	//关闭通信，不响应从控机的请求
    void sendEnergyAndMoney(int roomNumber, double energy, double money);				//向某个房间发送已消耗的能量和累积金额,成功返回true
    void sendRoomTmpReq(int roomNumber);											//向某个房间发送监控房间温度请求
    void sendIdCheckRes(int roomNumber, bool flag,int mode);
    void sendAirBag(int roomNumber, int speed);

    bool isRunning();

    SOCKET getSocket();

    std::unordered_map<int, sockaddr_in> mp;
private:
    SOCKET m_socket;	/*自己的套接字*/
    sockaddr_in m_sockaddr_in;	/*自己的IP地址和端口信息*/
    int m_serverport;
    CentralAC* m_cac;
    bool running;
    std::thread* m_thread;


    void initSocket();
};

#endif

