#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMouseEvent>
#include <centralac.h>
#include <mutex>
#include <QObject>
extern std::mutex db_mutex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CAC= new CentralAC();
    CAC->ac_timer = new QTimer();
    CAC->temper_timer = new QTimer();
    page_account=new Account;
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentIndex(0);
    m_isPressed = false;
    ui->room_view_2->setAlternatingRowColors(true);
    QFont font = ui->room_view_2->horizontalHeader()->font();           //设置表头字体加粗
    font.setBold(true);
    ui->room_view_2->horizontalHeader()->setFont(font);
    ui->room_view_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->room_view_2->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->room_view_2->setFocusPolicy(Qt::NoFocus);
    ui->room_view_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //connect(page_account, SIGNAL(clear_buff(int)),this, SLOT(clear_buffer2(int)));
    //渐变效果
    timer_flag=0;
    opacity=0.0;
    effect=new QGraphicsOpacityEffect(this);
    effect2=new QGraphicsOpacityEffect(this);
    warning_timer=new QTimer;
    QObject::connect(warning_timer,SIGNAL(timeout()),this,SLOT(fade_on_fade_out()));
    warning_timer->start(50);
    //gif效果
    gif_effect=new QGraphicsOpacityEffect(this);
    gif_timer=new QTimer;
    fade_timer=new QTimer;
    QObject::connect(gif_timer,SIGNAL(timeout()),this,SLOT(gif_move()));
    gif_timer->start(2500);
    QObject::connect(fade_timer,SIGNAL(timeout()),this,SLOT(gif_fade()));
    gif_count=1;

    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(1, 300);
    ui->room_line1_2->setValidator(aIntValidator);
    QIntValidator* aIntValidator2 = new QIntValidator;
    aIntValidator2->setRange(1, 300);
    ui->room_line1->setValidator(aIntValidator2);
    QIntValidator* aIntValidator3 = new QIntValidator;
    aIntValidator3->setRange(1, 65535);
    ui->user_line1->setValidator(aIntValidator3);
    QIntValidator* aIntValidator4 = new QIntValidator;
    aIntValidator4->setRange(1, 65535);
    ui->user_line2->setValidator(aIntValidator4);

    // 画心电图
    series = new QSplineSeries();
    series->setName("spline");
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple spline chart example");
    chart->createDefaultAxes();
    chart->axisY()->setRange(0, 10);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(481,201);
    baseLayout = new QGridLayout(); //便于显示，创建网格布局
    baseLayout->addWidget(chartView, 1, 0);
    ui->graphicsView_10->setLayout(baseLayout); //显示到QWidget控件
}
MainWindow::~MainWindow()
{
    //qDebug()<<"主界面死亡！";
    delete CAC;
    delete ui;
}

void MainWindow::gif_move()
{
    gif_timer->stop();
    fade_timer->start(50);
    gif_opa=1.0;
    gif_count++;
    if(gif_count==6)
        gif_count=1;
    switch(gif_count)
    {
      case 1:
            ui->view2->raise();
            ui->view1->raise();
            break;
      case 2:
            ui->view3->raise();
            ui->view2->raise();
            break;
      case 3:
            ui->view4->raise();
            ui->view3->raise();
            break;
      case 4:
            ui->view5->raise();
            ui->view4->raise();
            break;
      case 5:
            ui->view1->raise();
            ui->view5->raise();
            break;
    }
}

void MainWindow::gif_fade()
{
    if(gif_opa<=0)
    {
        fade_timer->stop();
        gif_timer->start(2000);
        gif_opa=1;
    }
    else
    {
        gif_opa-=0.05;
        gif_effect->setOpacity(gif_opa);
        switch(gif_count)
        {
          case 1:
                ui->view1->setGraphicsEffect(gif_effect);
                break;
          case 2:
                ui->view2->setGraphicsEffect(gif_effect);
                break;
          case 3:
                ui->view3->setGraphicsEffect(gif_effect);
                break;
          case 4:
                ui->view4->setGraphicsEffect(gif_effect);
                break;
          case 5:
                ui->view5->setGraphicsEffect(gif_effect);
                break;
        }
    }
}

void MainWindow::fade_on_fade_out()
{
    if(opacity>=0.9)
    {
        timer_flag=1;
    }
    if(opacity<=0.1)
    {
        timer_flag=0;
    }
    if(!timer_flag)
    {
        opacity+=0.05;
        effect->setOpacity(opacity);
        ui->warning->setGraphicsEffect(effect);
        effect2->setOpacity(1-opacity);
        //ui->warning2->setGraphicsEffect(effect2);
    }
    else
    {
        opacity-=0.05;
        effect->setOpacity(opacity);
        ui->warning->setGraphicsEffect(effect);
        effect2->setOpacity(1-opacity);
        //ui->warning2->setGraphicsEffect(effect2);
    }
}

void MainWindow::set_page(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

void MainWindow::on_login_clicked()
{

}

void MainWindow::on_exit_h_clicked()
{
    emit rotateWindow();
}

void MainWindow::on_room_info_toggled(bool checked)
{
    if(!checked)
        return;
}

void MainWindow::ac_timeout()
{
    CAC->ac_timeout();
    QString time_now=CAC->getdate_string();
    ui->day_label->setText(time_now);
    int temp=CAC->getdate();
    if((temp%100)<10)
        time_now=time_now.mid(9,1);
    else
    {
        time_now=time_now.mid(8,2);
    }

    ui->room_view_2->horizontalHeader()->setVisible(true);
    ui->room_view_2->clearContents();
    ui->room_view_2->setRowCount(0);
    currentinfo *cur=NULL;
    vector<currentinfo> current_info_total= cur->get_roominfo_total();
    int rowcount=0;
    QString temp2;
    //qDebug()<<"total size:"<<current_info_total.size();
    for(unsigned int i=0;i<current_info_total.size();i++)
    {
        rowcount = ui->room_view_2->rowCount();
        //qDebug()<<"动态:"<<rowcount;
        ui->room_view_2->insertRow(rowcount);

        temp2=QString::number(current_info_total[i].get_roomnumber(), 10);
        ui->room_view_2->setItem(i,0,new QTableWidgetItem(temp2));
        temp2=QString::number(current_info_total[i].get_current_temperature(), 10);
        if(current_info_total[i].get_current_temperature()==-1)
            temp2="None";
        ui->room_view_2->setItem(i,1,new QTableWidgetItem(temp2));
        temp2=QString::number(current_info_total[i].get_current_energy(),10,2);
        ui->room_view_2->setItem(i,2,new QTableWidgetItem(temp2));
        temp2=QString::number(current_info_total[i].get_fee(), 10,2);
        ui->room_view_2->setItem(i,3,new QTableWidgetItem(temp2));
        temp2=QString::number(current_info_total[i].get_speed(), 10);
        if(current_info_total[i].get_speed()==-1)
            temp2="Close";
        ui->room_view_2->setItem(i,4,new QTableWidgetItem(temp2));
        if(i%2 == 1)
            for(int j=0 ; j<5 ;j++)
               ui->room_view_2->item(i,j)->setBackgroundColor(QColor(255,255,255));
        else
            for(int j=0 ; j<5 ;j++)
               ui->room_view_2->item(i,j)->setBackgroundColor(QColor(245,245,245));

    }

    vector<int>CPU_info=CAC->get_CPU();
    for(unsigned int j=0;j<current_info_total.size();j++)
    {
        for(unsigned int i=0;i<CPU_info.size();i++)
        {
            if(ui->room_view_2->item(j,0)->text().toInt()==CPU_info[i])
            {
                for(int k=0;k<5;k++)
                    ui->room_view_2->item(j,k)->setBackgroundColor(QColor(255,225,255));
            }
        }
    }
}

void MainWindow::on_current_info_clicked()
{
    //ui->layer->setCurrentIndex(1);
}

void MainWindow::on_room_info_clicked()
{
    //ui->layer->setCurrentIndex(2);
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_current_info_toggled(bool checked)
{
    if(checked)
        return;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos() + movePoint;
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x(), widgetPos.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void MainWindow::on_test_btn_clicked()
{
    emit rotateWindow();
}

void MainWindow::on_print_bill_clicked()
{
    if(CAC->getswitch()==0)
        QMessageBox::about(NULL, "WARNING", "Central conditioner is closing");
    else
    {
        if(ui->user_line1->text()=="" || ui->room_line1->text()=="")
        {
            QMessageBox::about(NULL, "WARNING", "Please input every blank!");
            ui->user_line1->clear();
            ui->room_line1->clear();
        }
        else if(page_account->get_bill_room_user(ui->room_line1->text().toInt(),ui->user_line1->text().toInt()))
        {
            ui->user_line1->clear();
            ui->room_line1->clear();
        }
        else
        {
            QMessageBox::about(NULL, "WARNING", "Input wrong information!");
            ui->room_line1->clear();
            ui->user_line1->clear();
        }

    }

}

void MainWindow::on_register_btn_clicked()
{
    if(CAC->getswitch()==0)
        QMessageBox::about(NULL, "WARNING", "Central conditioner is closing");
    else
    {
        if(ui->user_line2->text()=="" || ui->room_line1_2->text()=="")
        {
             QMessageBox::about(NULL, "WARNING", "Please input every blank!");
             ui->user_line2->clear();
             ui->room_line1_2->clear();
        }
        else
        {
            bool flag;
            db_mutex.lock();
            flag = page_db.insert_currentinfo(ui->room_line1_2->text().toInt(),ui->user_line2->text().toInt(),-1,-1,0,0,CAC->getdate_string());
            db_mutex.unlock();
            if(!flag)
            {
                qDebug()<<"注册新用户失败！";
                QMessageBox::about(NULL, "WARNING", "Room has been registered!");
                ui->user_line2->clear();
                ui->room_line1_2->clear();
            }
            else
            {
                ui->room_line1_2->clear();
                ui->user_line2->clear();
            }

        }

    }
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    if(checked)
    {
        ui->account_page->setCurrentIndex(1);
    }
    else
    {
        ui->account_page->setCurrentIndex(0);
    }
}

void MainWindow::on_account_week_toggled(bool checked)
{
    if(checked)
    {
        ui->account_page->setCurrentIndex(2);
    }
    else
    {
        ui->account_page->setCurrentIndex(0);
    }
}

void MainWindow::on_account_month_toggled(bool checked)
{
    if(checked)
    {
        ui->account_page->setCurrentIndex(3);
    }
    else
    {
        ui->account_page->setCurrentIndex(0);
    }
}

void MainWindow::on_account_year_toggled(bool checked)
{
    if(checked)
    {
        ui->account_page->setCurrentIndex(4);
    }
    else
    {
        ui->account_page->setCurrentIndex(0);
    }
}

void MainWindow::on_calendar1_clicked(const QDate &date)
{

    page_account->get_account_day(date.year(),date.month(),date.day());
    ui->account_page->setCurrentIndex(0);
}

void MainWindow::on_calendar2_clicked(const QDate &date)
{
    page_account->get_account_week(date.year(),date.month(),date.day());
    ui->account_page->setCurrentIndex(0);
}

void MainWindow::on_calendar3_clicked(const QDate &date)
{
    page_account->get_account_month(date.year(),date.month());
    ui->account_page->setCurrentIndex(0);
}

void MainWindow::on_calendar4_clicked(const QDate &date)
{
    page_account->get_account_year(date.year());
    ui->account_page->setCurrentIndex(0);
}

void MainWindow::clear_buffer2(int roomnumber)
{
    CAC->roomLogOut(roomnumber);
    return;
}

void MainWindow::on_AC_on_clicked()
{
    if(this->start_ac_flag == 0)
    {
        CAC->run();
        CAC->ac_timer->start(1000);
        CAC->temper_timer->start(CAC->getInterval()*1000/2);//500ms
        connect(CAC->ac_timer, SIGNAL(timeout()), this, SLOT(ac_timeout()));
        connect(CAC->temper_timer, SIGNAL(timeout()), this, SLOT(temper_timeout()));

        QString rate = QString::number(CAC->getInterval(),10);
        ui->rate_label->setText(rate);
        ui->stackedWidget_2->setCurrentIndex(0);
        this->start_ac_flag = 1;
        qDebug()<<CAC;
        warning_timer->stop();
        opacity=0.0;
        effect->setOpacity(opacity);
        ui->warning->setGraphicsEffect(effect);
        effect2->setOpacity(opacity);
        //ui->warning2->setGraphicsEffect(effect2);
        ui->room_view_2->horizontalHeader()->setVisible(1);
        ui->label_flag->setVisible(0);
    }
    else
    {
        QMessageBox::about(NULL, "WARNING", "You have already turned on the conditioner!");
    }

}

void MainWindow::temper_timeout()
{
    CAC->temper_timeout();
}

void MainWindow::on_AC_off_clicked()
{
    if(this->start_ac_flag == 1)
   {
        CAC->ac_timer->stop();
        CAC->temper_timer->stop();
        this->CAC->close();
        this->start_ac_flag = 0;
        warning_timer->start(50);
        opacity=0.0;
        effect->setOpacity(opacity);
        ui->warning->setGraphicsEffect(effect);
        effect2->setOpacity(1-opacity);
        //ui->warning2->setGraphicsEffect(effect2);
        ui->room_view_2->clearContents();
        ui->room_view_2->setRowCount(0);
   }
  else
      QMessageBox::about(NULL, "WARNING", "You have already turned off the conditioner!");

}

void MainWindow::on_pushButton_2_clicked()
{
    CAC->sub_rate();
    ui->rate_label->setText(QString::number(CAC->getInterval(),10));
    if(CAC->getInterval() == 1)
         QMessageBox::about(NULL, "WARNING", "Updated rate arrives to the limit!");
}

void MainWindow::on_add_rate_button_clicked()
{
    //qDebug()<<"lala";
    CAC->add_rate();
    //qDebug()<<CAC->getInterval();
    ui->rate_label->setText(QString::number(CAC->getInterval(),10));
}

void MainWindow::on_warm_button_clicked()
{
    CAC->change_mode_to_warm();
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_cool_button_clicked()
{
    CAC->change_mode_to_cold();
    ui->stackedWidget_2->setCurrentIndex(0);
}




void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_room_line1_textEdited(const QString &arg1)
{

}


