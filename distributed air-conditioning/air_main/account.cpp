#include "account.h"
#include <mutex>
extern std::mutex db_mutex;
extern std::mutex list_mutex;
Account::Account()
{
    db_mutex.lock();
    acdb.createConnection();
    acdb.createCurrentinfo();
    acdb.createBufferinfo();
    acdb.createRoominfo();
    db_mutex.unlock();
}
void Account::get_account_day(int year, int month, int day)
{
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    QString strBuffer;
    QDateTime time;
    QString y_m = QString::number(year)+"年"+QString::number(month)+"月"+QString::number(day)+"日空调使用报表";
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yy-MM-dd hh:mm:ss");
    QStringList title;
    title.push_back(QStringLiteral("房间号"));
    title.push_back(QStringLiteral("用户身份证号"));
    title.push_back(QStringLiteral("使用开始日期"));
    title.push_back(QStringLiteral("使用结束日期"));
    title.push_back(QStringLiteral("使用开始时间"));
    title.push_back(QStringLiteral("使用结束时间"));
    title.push_back(QStringLiteral("使用目标温度"));
    title.push_back(QStringLiteral("使用开始温度"));
    title.push_back(QStringLiteral("使用结束温度"));
    title.push_back(QStringLiteral("使用风速"));
    title.push_back(QStringLiteral("使用费用"));

    QString html;
    //html +="<h1 align=\"center\">" + QString("报表试用版")+"</h1>";
    html += QString("<h1 align=\"center\">%1</h1>").arg(y_m);
    html +="<h2 align=\"center\">" + strBuffer+"</h2>";
    //html +="<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title.count(); i++)
        html += QString("<th>%1</th>").arg(title.at(i));
    html +="</th>";
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.search_table_date(year,month,day);

    int roomnumber;
    int userid;
    int start_date;
    int end_date;
    QString start_time;
    QString end_time;
    int target_temperature;
    int start_temperature;
    int end_temperature;
    int speed;
    double fee;

    while(query.next())
    {
        html +="<tr>";

            roomnumber = query.value(0).toInt();
            userid = query.value(1).toInt();
            start_date = query.value(2).toInt();
            end_date =query.value(3).toInt();
            start_time =  query.value(4).toString();
            end_time =  query.value(5).toString();
            target_temperature = query.value(6).toInt();
            start_temperature = query.value(7).toInt();
            end_temperature = query.value(8).toInt();
            speed = query.value(9).toInt();
            fee = query.value(10).toDouble();
            QString fee1 = QString::number(fee,'f',2);
            fee = fee1.toDouble();


        html += QString("<td><h5>%1</h5></td>").arg(roomnumber);
        html += QString("<td><h5>%1</h5></td>").arg(userid);
        html += QString("<td><h5>%1</h5></td>").arg(start_date);
        html += QString("<td><h5>%1</h5></td>").arg(end_date);
        html += QString("<td><h5>%1</h5></td>").arg(start_time);
        html += QString("<td><h5>%1</h5></td>").arg(end_time);
        html += QString("<td><h5>%1</h5></td>").arg(target_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(start_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(end_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(speed);
        html += QString("<td><h5>%1</h5></td>").arg(fee);

        html +="</tr>";
     }

    html +="</table> </td> <br /><br /><br /><br />";
    double total_fee = acdb.get_fee_date(year,month,day);
    QString str = "总费用:"+QString::number(total_fee,'f',2);
    html +="<h2 align=\"right\">" +str+"</h2>";
    QStringList title_open;
    title_open.push_back(QStringLiteral("房间号"));
    title_open.push_back(QStringLiteral("空调开启次数"));

    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title_open.count(); i++)
        html += QString("<th>%1</th>").arg(title_open.at(i));
    html +="</th>";
    QSqlQuery query1;
    query1 = acdb.get_opentime_per_room_from_roominfo_date(year,month,day);


    int count;

    while(query1.next())
    {
        html +="<tr>";

            roomnumber = query1.value(0).toInt();
            count = query1.value(1).toInt();


        html += QString("<td><h3>%1</h3></td>").arg(roomnumber);
        html += QString("<td><h3>%1</h3></td>").arg(count);
        html +="</tr>";
     }
    db_mutex.unlock();
    qDebug()<<"day lock！！！";
    QPrinter printer_html;
    QString pdfname = y_m + ".pdf";
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(pdfname);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();
   // QString strDoc = "test_html.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfname));
    qDebug()<<"day lock 002！！！";

}

void Account::get_account_month(int year, int month)
{
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    QString y_m = QString::number(year)+"年"+QString::number(month)+"月空调使用报表";
    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yy-MM-dd hh:mm:ss");
    QStringList title;
    title.push_back(QStringLiteral("房间号"));
    title.push_back(QStringLiteral("用户身份证号"));
    title.push_back(QStringLiteral("使用开始日期"));
    title.push_back(QStringLiteral("使用结束日期"));
    title.push_back(QStringLiteral("使用开始时间"));
    title.push_back(QStringLiteral("使用结束时间"));
    title.push_back(QStringLiteral("使用目标温度"));
    title.push_back(QStringLiteral("使用开始温度"));
    title.push_back(QStringLiteral("使用结束温度"));
    title.push_back(QStringLiteral("使用风速"));
    title.push_back(QStringLiteral("使用费用"));

    QString html;
    //html +="<h1 align=\"center\">" + QString("报表试用版")+"</h1>";
    html += QString("<h1 align=\"center\">%1</h1>").arg(y_m);
    html +="<h2 align=\"center\">" + strBuffer+"</h2>";
    //html +="<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title.count(); i++)
        html += QString("<th>%1</th>").arg(title.at(i));
    html +="</th>";
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.search_table_month(year,month);

    int roomnumber;
    int userid;
    int start_date;
    int end_date;
    QString start_time;
    QString end_time;
    int target_temperature;
    int start_temperature;
    int end_temperature;
    int speed;
    double fee;

    while(query.next())
    {
        html +="<tr>";

            roomnumber = query.value(0).toInt();
            userid = query.value(1).toInt();
            start_date = query.value(2).toInt();
            end_date =query.value(3).toInt();
            start_time =  query.value(4).toString();
            end_time =  query.value(5).toString();
            target_temperature = query.value(6).toInt();
            start_temperature = query.value(7).toInt();
            end_temperature = query.value(8).toInt();
            speed = query.value(9).toInt();
            fee = query.value(10).toDouble();
            QString fee1 = QString::number(fee,'f',2);
            fee = fee1.toDouble();

        html += QString("<td><h5>%1</h5></td>").arg(roomnumber);
        html += QString("<td><h5>%1</h5></td>").arg(userid);
        html += QString("<td><h5>%1</h5></td>").arg(start_date);
        html += QString("<td><h5>%1</h5></td>").arg(end_date);
        html += QString("<td><h5>%1</h5></td>").arg(start_time);
        html += QString("<td><h5>%1</h5></td>").arg(end_time);
        html += QString("<td><h5>%1</h5></td>").arg(target_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(start_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(end_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(speed);
        html += QString("<td><h5>%1</h5></td>").arg(fee);

        html +="</tr>";
     }

    double total_fee = acdb.get_fee_month(year,month);
    html +="</table> </td> <br /><br /><br /><br />";
    QString str = "总费用:"+QString::number(total_fee,'f',2);
    html +="<h2 align=\"right\">" +str+"</h2>";
    QStringList title_open;
    title_open.push_back(QStringLiteral("房间号"));
    title_open.push_back(QStringLiteral("空调开启次数"));

    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title_open.count(); i++)
        html += QString("<th>%1</th>").arg(title_open.at(i));
    html +="</th>";

    QSqlQuery query1;
    query1 = acdb.get_opentime_per_room_from_roominfo_month(year,month);


    int count;

    while(query1.next())
    {
        html +="<tr>";

            roomnumber = query1.value(0).toInt();
            count = query1.value(1).toInt();


        html += QString("<td><h3>%1</h3></td>").arg(roomnumber);
        html += QString("<td><h3>%1</h3></td>").arg(count);
        html +="</tr>";
     }
    db_mutex.unlock();
    QPrinter printer_html;
    QString pdfname = y_m + ".pdf";
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(pdfname);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();
   // QString strDoc = "test_html.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfname));
}

void Account::get_account_year(int year)
{
    /*
     *query.bindValue(0,roomnumber);//添加信息
        query.bindValue(1,userid);
        query.bindValue(2,start_date);
        query.bindValue(3,end_date);//结束日期
        query.bindValue(4,start_time);
        query.bindValue(5,end_time);//结束时间
        query.bindValue(6,target_temperature);
        query.bindValue(7,start_temperature);
        query.bindValue(8,end_temperature);//结束温度
        query.bindValue(9,speed);
        query.bindValue(10,fee);//费用
        */
    QString y_m = QString::number(year)+"年空调使用情况报表";
    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yy-MM-dd hh:mm:ss");
    QStringList title;
    title.push_back(QStringLiteral("房间号"));
    title.push_back(QStringLiteral("用户身份证号"));
    title.push_back(QStringLiteral("使用开始日期"));
    title.push_back(QStringLiteral("使用结束日期"));
    title.push_back(QStringLiteral("使用开始时间"));
    title.push_back(QStringLiteral("使用结束时间"));
    title.push_back(QStringLiteral("使用目标温度"));
    title.push_back(QStringLiteral("使用开始温度"));
    title.push_back(QStringLiteral("使用结束温度"));
    title.push_back(QStringLiteral("使用风速"));
    title.push_back(QStringLiteral("使用费用"));

    QString html;
    //html +="<h1 align=\"center\">" + QString("报表试用版")+"</h1>";
    html += QString("<h1 align=\"center\">%1</h1>").arg(y_m);
    html +="<h2 align=\"center\">" + strBuffer+"</h2>";
    //html +="<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title.count(); i++)
        html += QString("<th>%1</th>").arg(title.at(i));
    html +="</th>";
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.search_table_year(year);

    int roomnumber;
    int userid;
    int start_date;
    int end_date;
    QString start_time;
    QString end_time;
    int target_temperature;
    int start_temperature;
    int end_temperature;
    int speed;
    double fee;

    while(query.next())
    {
        html +="<tr>";

            roomnumber = query.value(0).toInt();
            userid = query.value(1).toInt();
            start_date = query.value(2).toInt();
            end_date =query.value(3).toInt();
            start_time =  query.value(4).toString();
            end_time =  query.value(5).toString();
            target_temperature = query.value(6).toInt();
            start_temperature = query.value(7).toInt();
            end_temperature = query.value(8).toInt();
            speed = query.value(9).toInt();
            fee = query.value(10).toDouble();
            QString fee1 = QString::number(fee,'f',2);
            fee = fee1.toDouble();

            html += QString("<td><h5>%1</h5></td>").arg(roomnumber);
            html += QString("<td><h5>%1</h5></td>").arg(userid);
            html += QString("<td><h5>%1</h5></td>").arg(start_date);
            html += QString("<td><h5>%1</h5></td>").arg(end_date);
            html += QString("<td><h5>%1</h5></td>").arg(start_time);
            html += QString("<td><h5>%1</h5></td>").arg(end_time);
            html += QString("<td><h5>%1</h5></td>").arg(target_temperature);
            html += QString("<td><h5>%1</h5></td>").arg(start_temperature);
            html += QString("<td><h5>%1</h5></td>").arg(end_temperature);
            html += QString("<td><h5>%1</h5></td>").arg(speed);
            html += QString("<td><h5>%1</h5></td>").arg(fee);

            html +="</tr>";
         }

    double total_fee = acdb.get_fee_year(year);
    html +="</table> </td> <br /><br /><br /><br />";
    QString str = "总费用:"+QString::number(total_fee,'f',2);
    html +="<h2 align=\"right\">" +str+"</h2>";
    QStringList title_open;
    title_open.push_back(QStringLiteral("房间号"));
    title_open.push_back(QStringLiteral("空调开启次数"));

    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title_open.count(); i++)
        html += QString("<th>%1</th>").arg(title_open.at(i));
    html +="</th>";

    QSqlQuery query1;
    query1 = acdb.get_opentime_per_room_from_roominfo_year(year);


    int count;

    while(query1.next())
    {
        html +="<tr>";

            roomnumber = query1.value(0).toInt();
            count = query1.value(1).toInt();


        html += QString("<td><h3>%1</h3></td>").arg(roomnumber);
        html += QString("<td><h3>%1</h3></td>").arg(count);
        html +="</tr>";
     }
    db_mutex.unlock();
    QPrinter printer_html;
    QString pdfname = y_m + ".pdf";
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(pdfname);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();
   // QString strDoc = "test_html.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfname));
}


bool Account:: get_bill_room_user(int roomnumber,int userid)
{
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.get_user_info_from_current(roomnumber,userid);
    db_mutex.unlock();
    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yyyy-MM-dd");
    QString start_date;
    double energy;
    double fee;
    if(query.next())
    {
        start_date = query.value(6).toString();
        energy = query.value(3).toDouble();
        fee = query.value(5).toDouble();
        int flag=query.value(4).toInt();


            QFile file;
            file.setFileName("bill.html");
            if(!file.open(QIODevice::ReadOnly))
            {
                qDebug()<<"nono bill.html"<<endl;
                return false;
            }
            //qDebug()<<"fee"<<fee<<endl;
            QTextStream in(&file);
            QString html_text1=in.readAll();
            QString html_text = html_text1.arg(userid).arg(start_date).arg(strBuffer).arg(roomnumber).arg(QString::number(energy,'f',2)).arg(QString::number(fee,'f',2));
            file.close();

            QFile file1("test_html.html");
            if(!file1.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                qDebug()<<"nono test_html"<<endl;
                return false;

            }
           QTextStream inn(&file1);
           inn<<html_text<<"\n";
           file1.close();
           system("print.bat");
           if(flag > -1)
                emit clear_buff(roomnumber);
           db_mutex.lock();
           acdb.delete_currentinfo_user(roomnumber,userid);
           db_mutex.unlock();
           return true;

    }
    else
    {
        return false;
    }


}

void Account::get_account_week(int year, int month, int day)
{
    QString strBuffer;
    QDateTime time;
    QString y_m = QString::number(year)+"年"+QString::number(month)+"月"+QString::number(day)+"日前一周空调使用报表";
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yy-MM-dd hh:mm:ss");
    QStringList title;
    title.push_back(QStringLiteral("房间号"));
    title.push_back(QStringLiteral("用户身份证号"));
    title.push_back(QStringLiteral("使用开始日期"));
    title.push_back(QStringLiteral("使用结束日期"));
    title.push_back(QStringLiteral("使用开始时间"));
    title.push_back(QStringLiteral("使用结束时间"));
    title.push_back(QStringLiteral("使用目标温度"));
    title.push_back(QStringLiteral("使用开始温度"));
    title.push_back(QStringLiteral("使用结束温度"));
    title.push_back(QStringLiteral("使用风速"));
    title.push_back(QStringLiteral("使用费用"));

    QString html;
    //html +="<h1 align=\"center\">" + QString("报表试用版")+"</h1>";
    html += QString("<h1 align=\"center\">%1</h1>").arg(y_m);
    html +="<h2 align=\"center\">" + strBuffer+"</h2>";
    //html +="<table width=\"500\" border=\"1\" align=\"center\" style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title.count(); i++)
        html += QString("<th>%1</th>").arg(title.at(i));
    html +="</th>";
    db_mutex.lock();
    QSqlQuery query;
    query = acdb.search_table_week(year,month,day);

    int roomnumber;
    int userid;
    int start_date;
    int end_date;
    QString start_time;
    QString end_time;
    int target_temperature;
    int start_temperature;
    int end_temperature;
    int speed;
    double fee;

    while(query.next())
    {
        html +="<tr>";
        for(int index = 0; index < 11; index++)
        {
            roomnumber = query.value(0).toInt();
            userid = query.value(1).toInt();
            start_date = query.value(2).toInt();
            end_date =query.value(3).toInt();
            start_time =  query.value(4).toString();
            end_time =  query.value(5).toString();
            target_temperature = query.value(6).toInt();
            start_temperature = query.value(7).toInt();
            end_temperature = query.value(8).toInt();
            speed = query.value(9).toInt();
            fee = query.value(10).toDouble();
        }
        html += QString("<td><h5>%1</h5></td>").arg(roomnumber);
        html += QString("<td><h5>%1</h5></td>").arg(userid);
        html += QString("<td><h5>%1</h5></td>").arg(start_date);
        html += QString("<td><h5>%1</h5></td>").arg(end_date);
        html += QString("<td><h5>%1</h5></td>").arg(start_time);
        html += QString("<td><h5>%1</h5></td>").arg(end_time);
        html += QString("<td><h5>%1</h5></td>").arg(target_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(start_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(end_temperature);
        html += QString("<td><h5>%1</h5></td>").arg(speed);
        html += QString("<td><h5>%1</h5></td>").arg(QString::number(fee,'f',2));

        html +="</tr>";
     }

    double total_fee = acdb.get_fee_week(year,month,day);
    html +="</table> </td> <br /><br /><br /><br />";
    QString str = "总费用:"+QString::number(total_fee,'f',2);
    html +="<h2 align=\"right\">" +str+"</h2>";
    QStringList title_open;
    title_open.push_back(QStringLiteral("房间号"));
    title_open.push_back(QStringLiteral("空调开启次数"));

    html += "<table border width=1000 align=\"center\"  style=\"border-collapse:collapse;\" bordercolor=\"gray\">";
    html += "<tr style =\"background-color:#96FED1\">";

    for(int i = 0; i<title_open.count(); i++)
        html += QString("<th>%1</th>").arg(title_open.at(i));
    html +="</th>";

    QSqlQuery query1;
    query1 = acdb.get_opentime_per_room_from_roominfo_week(year,month,day);

    int count;

    while(query1.next())
    {
        html +="<tr>";

            roomnumber = query1.value(0).toInt();
            count = query1.value(1).toInt();


        html += QString("<td><h3>%1</h3></td>").arg(roomnumber);
        html += QString("<td><h3>%1</h3></td>").arg(count);
        html +="</tr>";
     }
    db_mutex.unlock();
    QPrinter printer_html;
    QString pdfname = y_m + ".pdf";
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(pdfname);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();
   // QString strDoc = "test_html.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfname));

}
