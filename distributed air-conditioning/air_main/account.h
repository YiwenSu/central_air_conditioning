#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "database.h"
#include "QPrinter"
#include "QPdfWriter"
#include "QString"
#include "QStringList"
#include <QTextCodec>
#include <QTextDocument>
#include <QTextBlock>
#include "QDesktopServices"
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <stdlib.h>
#include <stdio.h>
#include <QObject>
class Account: public QObject
{
    Q_OBJECT
public:
    Account();
    void get_account_day(int year, int month, int day);
    void get_account_month(int year, int month);
    void get_account_year(int year);
    bool get_bill_room_user(int roomnumber,int userid);
    void get_account_week(int year, int month, int day);
signals:
    void clear_buff(int roomnumber);
private:
    Database acdb;

};

#endif // ACCOUNT_H
