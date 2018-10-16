#include "centralac_com.h"
#include <qDebug>

CentralAC_Com::CentralAC_Com(int serverport,CentralAC *centralAC)
{

}

void CentralAC_Com::run()
{

}

void CentralAC_Com::close()
{

}

void CentralAC_Com::sendAirBag(int roomNumber, int speed)
{
    qDebug() << roomNumber<<"!!";
}

void CentralAC_Com::sendIdCheckRes(int roomNumber, bool flag, int mode)
{

}
