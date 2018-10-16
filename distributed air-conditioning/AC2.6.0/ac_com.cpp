#include "AC_Com.h"
#include "AC.h"

#include <QDebug>

void recvThread(AC* ac, AC_Com *accom);
double getDouble(int a, int b);

void message(std::string s)
{
    std::cout << s << std::endl;
}

AC_Com::AC_Com()
{

}


AC_Com::~AC_Com()
{
    sendCloseReq();
}

AC_Com::AC_Com(std::string serverip, int serverport, AC* ac)
{
    m_serverip = serverip;
    m_serverport = serverport;
    m_ac = ac;
    m_running = false;
    initSocket();
}

void AC_Com::run()
{
    /*开启接收线程*/
    //m_thread = new std::thread(recvThread, m_ac, this);
    if(m_running) return;
    m_running = true;
    std::thread th(recvThread,m_ac,this);
    th.detach();

}
void AC_Com::close()
{
    m_running = false;
}
void AC_Com::sendIdInfo(int roomNumber, int userid)						//发送认证信息，包括房间号和用户id
{
    qDebug()<<"发送id"<<endl;
    int sendData[3];
    sendData[0] = 1;
    sendData[1] = roomNumber;
    sendData[2] = userid;

    if (sendto(m_socket, (char *)sendData, sizeof(sendData), 0, (sockaddr*)&m_serveraddr_in, sizeof(m_serveraddr_in)) == -1){
        message("sendIdInfo");
    }

}
void AC_Com::sendTmpReq(int mode,int speed, int temperature)							//发送温控请求,speed有无风(0)，低速(1)，中速(2)，高速(3)
{
    QString a,b,c;
    a = QString::number(temperature);
    b = QString::number(speed);
    c = QString::number(mode);
    a = "send temperature req tem:" + a +" speed:"+b+" mode:"+c;
    //message("send temperature req");
    qDebug()<<a<<endl;
    int sendData[5];
    sendData[0] = 3;
    sendData[1] = m_ac->getRoomNumber();
    sendData[2] = mode;
    sendData[3] = speed;
    sendData[4] = temperature;

    if (sendto(m_socket, (char *)sendData, sizeof(sendData), 0, (sockaddr*)&m_serveraddr_in, sizeof(m_serveraddr_in)) == -1){
        message("sendTmpReq");
    }
}


void AC_Com::sendRoomTmp(int temperatur)
{
    int sendData[3];
    sendData[0] = 5;
    sendData[1] = m_ac->getRoomNumber();
    sendData[2] = m_ac->getRoomTmp();

    if (sendto(m_socket, (char *)sendData, sizeof(sendData), 0, (sockaddr*)&m_serveraddr_in, sizeof(m_serveraddr_in)) == -1){
        message("sendRoomTmp");
    }
}

void AC_Com::sendCloseReq()
{
    int sendData[2];
    sendData[0] = 8;
    sendData[1] = m_ac->getRoomNumber();

    if (sendto(m_socket, (char *)sendData, sizeof(sendData), 0, (sockaddr*)&m_serveraddr_in, sizeof(m_serveraddr_in)) == -1){
        message("sendCloseReq");
    }
}

void AC_Com::initSocket()
{
    /*初始化Socket资源*/
    message("init socket");
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
    m_serveraddr_in.sin_family = AF_INET;
    m_serveraddr_in.sin_port = htons((unsigned short)m_serverport);
//    if (!inet_pton(AF_INET, m_serverip.c_str(), (void *)&(m_serveraddr_in.sin_addr.S_un.S_addr))){
//        message("SERVER IP ERROR");
//        exit(1);
//    }
    u_long t = inet_addr(m_serverip.c_str());
    if(t == INADDR_NONE){
        message("SERVER IP ERROR");
        exit(1);
    }
    m_serveraddr_in.sin_addr.S_un.S_addr = t;
}

bool AC_Com::isRunning()
{
    return m_running;
}

SOCKET AC_Com::getSocket()
{
    return m_socket;
}
sockaddr_in AC_Com::getServerAddr()
{
    return m_serveraddr_in;
}

void recvThread(AC* ac, AC_Com *accom)
{
    message("ac start to receive");
    int buff[10];
    double a,b;

    /*可读套接字集合，放入自己的SOCKET，每次select检查自己是否可读*/
    fd_set fdread;
    /*超时信息*/
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    while (1){
        sockaddr_in sin = accom->getServerAddr();
        int len = sizeof(sin);

        /*若AC_Com关闭，则退出线程*/
        if (!accom->isRunning()){
            message("ac receive thread quit");
            return;
        }

        /*套接字集合初始化*/
        FD_ZERO(&fdread);
        FD_SET(accom->getSocket(), &fdread);

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
                int ret = recvfrom(accom->getSocket(), (char *)buff, 40, 0, (sockaddr*)&sin, &len);

                if (ret > 0){
                    switch (buff[0]){
                    case 2:
                        if (buff[1] == 1){
                            /*认证失败*/

                            ac->setIdCheckRes(false);
                            message("id check failed");
                        }
                        else{
                            ac->setIdCheckRes(true);
                            ac->setMode_noSend(buff[2]);
                            message("id check success");
                        }
                        break;
                    case 6:
                        qDebug() << buff[1] << " " << buff[2] << " " << buff[3] << buff[4];
                        a = getDouble(buff[1], buff[2]);
                        b = getDouble(buff[3], buff[4]);
                        qDebug() << "money "<< a << " energy" << b;
                        ac->setMoney(a);
                        ac->setEnergy(b);

                        break;
                    case 4:
                        accom->sendRoomTmp(ac->getRoomTmp());
                        message("room_temperature");
                        break;
                    case 7:
                        ac->setAirBag(buff[1]);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

double getDouble(int a, int b)
{
    double t = (double)b;
    t /= 100;
    t = t - (int)t;
    return (double)a + t;
}
