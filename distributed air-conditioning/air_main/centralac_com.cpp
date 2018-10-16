#include "CentralAC_Com.h"
#include "CentralAC.h"
#include <QDebug>
#include<QString>
#include <iostream>
using namespace std;
void splitDouble(double a, int &b, int &c);
void recvThread(CentralAC *cac, CentralAC_Com *caccom);

void message(std::string s)
{
    //std::cout << s << std::endl;
    QString str =QString::fromStdString(s);
    qDebug()<<"debug:"<<str;
}

CentralAC_Com::CentralAC_Com()
{

}


CentralAC_Com::~CentralAC_Com()
{

}

CentralAC_Com::CentralAC_Com(int serverport, CentralAC *centralAC)
{
    m_serverport = serverport;
    m_cac = centralAC;
    initSocket();
    running = false;
    m_thread = NULL;
}
void CentralAC_Com::run()																		//开启通信，响应从控机的请求
{
    if (running) return;
    running = true;
    /*开启接收线程*/
    m_thread = new std::thread(recvThread, m_cac, this);
}

bool CentralAC_Com::isRunning()
{
    return running;
}

void CentralAC_Com::close()																	//关闭通信，不响应从控机的请求
{
    running = false;
}
void CentralAC_Com::sendEnergyAndMoney(int roomNumber, double energy, double money)				//向某个房间发送已消耗的能量和累积金额,成功返回true
{
    //qDebug()<<"-------room:"<<roomNumber<<"  energy:"<<energy<<"  money:"<<money;
    if (!mp.count(roomNumber)) return;

    int buff[5];
    buff[0] = 6;
    splitDouble(money, buff[1], buff[2]);
    splitDouble(energy, buff[3], buff[4]);

    if (sendto(m_socket, (char *)buff, sizeof(buff), 0, (sockaddr*)&mp[roomNumber], sizeof(mp[roomNumber])) == -1){
        message("sendIdCheckRes");
    }
}
void CentralAC_Com::sendRoomTmpReq(int roomNumber)											//向某个房间发送监控房间温度请求
{
    if (!mp.count(roomNumber)) return;

    int buff[1];
    buff[0] = 4;

    if (sendto(m_socket, (char *)buff, sizeof(buff), 0, (sockaddr*)&mp[roomNumber], sizeof(mp[roomNumber])) == -1){
        message("sendIdCheckRes");
    }
}

void CentralAC_Com::sendIdCheckRes(int roomNumber, bool flag, int mode)
{
    if (!mp.count(roomNumber)) return;

    int buff[3];
    buff[0] = 2;
    buff[1] = flag ? 0 : 1;
    buff[2] = mode;
    qDebug()<<"RETURN Message:"<<roomNumber<<"; flag:"<<flag<<"; mode:"<<mode;
    if (sendto(m_socket, (char *)buff, sizeof(buff), 0, (sockaddr*)&mp[roomNumber], sizeof(mp[roomNumber])) == -1){
        message("sendIdCheckRes");
    }
}

void CentralAC_Com::sendAirBag(int roomNumber, int speed)
{
    if (!mp.count(roomNumber)) {
        message("air bag error");
        return;
    }

    int buff[2];
    buff[0] = 7;
    buff[1] = speed;

    if (sendto(m_socket, (char *)buff, sizeof(buff), 0, (sockaddr*)&mp[roomNumber], sizeof(mp[roomNumber])) == -1){
        message("sendIdCheckRes");
    }
}

SOCKET CentralAC_Com::getSocket()
{
    return m_socket;
}

void CentralAC_Com::initSocket()
{
    /*初始化Socket资源*/
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        message("WSASTARTUP ERROR");
        exit(1);
    }

    /*创建自己的套接字*/
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET)
    {
        message("SOCKET ERROR");
        exit(1);
    }

    /*初始化服务器的ip地址、端口*/
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_port = htons((unsigned short)m_serverport);
    m_sockaddr_in.sin_addr.S_un.S_addr = INADDR_ANY;

    /*将套接字绑定到具体的端口上*/
    if (bind(m_socket, (sockaddr *)&m_sockaddr_in, sizeof(m_sockaddr_in)) == SOCKET_ERROR)
    {
        message("BIND ERROR");
        closesocket(m_socket);
        exit(1);
    }

    /*忽略连接重置错误10054*/
    DWORD dwBytesReturned = 0;
    BOOL bNewBehavior = FALSE;
    DWORD status;
    status = WSAIoctl(m_socket,SIO_UDP_CONNRESET,
        &bNewBehavior, sizeof(bNewBehavior),
        NULL, 0, &dwBytesReturned,
        NULL, NULL);
    if (SOCKET_ERROR == status)
    {
        DWORD dwErr = WSAGetLastError();
        if (WSAEWOULDBLOCK == dwErr)
        {
            message("WSAWOULDBLOCK");
        }
        else
        {
            message("WSAIoctl(SIO_UDP_CONNRESET) Error");
        }
    }
}

void recvThread(CentralAC *cac, CentralAC_Com *caccom)
{
    message("cac start to recv");
    int buff[10];

    /*可读套接字集合，放入自己的SOCKET，每次select检查自己是否可读*/
    fd_set fdread;
    /*超时信息*/
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    while (1){
        if (!caccom->isRunning()){
            /*若CentralAC_Com关闭，则退出线程*/
            message("cac thread quit");
            return;
        }

        /*套接字集合初始化*/
        FD_ZERO(&fdread);
        FD_SET(caccom->getSocket(), &fdread);

        /*非阻塞检查SOCKET是否可读*/
        int sel = select(0, &fdread, NULL, NULL, &timeout);

        if (sel == SOCKET_ERROR){
            message("SELECT ERROR");
            return;
        }
        else{
            if (sel == 0){
                /*超时*/
                continue;
            }
            else{
                /*socket可读*/
                sockaddr_in sin_client;
                int len = sizeof(sin_client);

                int ret = recvfrom(caccom->getSocket(), (char *)buff, 40, 0, (sockaddr*)&sin_client, &len);
                //message("cac receive a packet");
                if (ret > 0){
                    switch (buff[0]){
                    case 1:
                        caccom->mp[buff[1]] = sin_client;
                        cac->checkIdInfo(buff[1], buff[2]);
                        message(std::string("id check") + " " + std::to_string(buff[1]) + " " + std::to_string(buff[2]));
                        break;
                    case 3:
                        cac->setRoomTmpReq(buff[1], buff[2], buff[3], buff[4]);
                        break;
                    case 5:
                        cac->setRoomTmp(buff[1], buff[2]);
                        break;
                    case 8:
                        cac->roomLogOut(buff[1]);
                        //caccom->mp.erase(buff[1]);
                        message(std::string("close request") + " " + std::to_string(buff[1]));
                        break;
                    default:
                        break;
                    }
                }
                else{
                    message(std::string("recv error:") + std::to_string(WSAGetLastError()));
                }
            }
        }
    }
}

void splitDouble(double a, int &b, int &c)
{
    a += 0.000001;
    b = (int)a;
    char buff[20];
    sprintf(buff, "%.2lf", a);
    int len = strlen(buff);
    int i;
    for (i = 0; i < len; i++){
        if (buff[i] == '.'){
            i++;
            break;
        }
    }

    c = 0;
    for (; i < len; i++){
        c = c * 10 + buff[i] - '0';
    }
}
