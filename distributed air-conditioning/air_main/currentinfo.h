#ifndef CURRENTINFO_H
#define CURRENTINFO_H
#include <QString>
#include <iostream>
using namespace std;
#include <vector>
#include "database.h"
class currentinfo
{
public:
    currentinfo(int roomNumber,int uid,int temperature,
                             double energy, int Speed,double money,QString day);
    int get_roomnumber();
    int get_userid();
    int get_current_temperature();
    double get_current_energy();
    int get_speed();
    double get_fee();
    QString get_date();
    vector<currentinfo> get_roominfo_total();

private:
    int roomnumber;
    int userid;
    int current_temperature;
    double current_energy;
    int speed;
    double fee;
    QString date;
};

#endif // CURRENTINFO_H
