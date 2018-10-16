#include "base_widget.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QMouseEvent>

base_Widget::base_Widget(QWidget *parent)
    : QStackedWidget(parent)
    , m_isRoratingWindow(false)
    , m_nextPageIndex(0)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setProperty("rotateValue", 0);                                                        // 给窗口设置rotateValue属性
    m_loginWindow = new login(this);
    connect(m_loginWindow, SIGNAL(rotateWindow()), this, SLOT(onRotateWindow()));
    connect(m_loginWindow,SIGNAL(exit()),this,SLOT(close()));
    m_mainWindow = new MainWindow(this);
    connect(m_mainWindow, SIGNAL(rotateWindow()), this, SLOT(onRotateWindow()));
    connect(m_loginWindow, SIGNAL(page(int)), m_mainWindow, SLOT(set_page(int)));
    this->addWidget(m_loginWindow);
    this->addWidget(m_mainWindow);
    this->setFixedSize(QSize(m_loginWindow->width(), m_loginWindow->height()));
}

base_Widget::~base_Widget()
{
    qDebug()<<"界面堆栈死亡！";
    delete m_mainWindow;
    delete m_loginWindow;
}

void base_Widget::onRotateWindow()
{
    if (m_isRoratingWindow)                                                                 // 如果窗口正在旋转，直接返回
    {
        return;
    }
    m_isRoratingWindow = true;
    m_nextPageIndex = (currentIndex() + 1) >= count() ? 0 : (currentIndex() + 1);
    QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "rotateValue");
    rotateAnimation->setDuration(1500);                                                     // 设置旋转持续时间;
    rotateAnimation->setEasingCurve(QEasingCurve::InCubic);                                 // 设置旋转角度变化趋势;
    rotateAnimation->setStartValue(0);                                                      // 设置旋转角度范围;
    rotateAnimation->setEndValue(180);
    connect(rotateAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    connect(rotateAnimation, SIGNAL(finished()), this, SLOT(onRotateFinished()));
    currentWidget()->hide();                                                                 // 隐藏当前窗口，通过不同角度的绘制来达到旋转的效果;
    rotateAnimation->start();
}

void base_Widget::paintEvent(QPaintEvent* event)
{
    if (m_isRoratingWindow)
    {
        int rotateValue = this->property("rotateValue").toInt();            // 小于90度时
        if (rotateValue <= 90)
        {
            QPixmap rotatePixmap(currentWidget()->size());
            currentWidget()->render(&rotatePixmap);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
        else
        {
            QPixmap rotatePixmap(widget(m_nextPageIndex)->size());           // 大于90度时
            widget(m_nextPageIndex)->render(&rotatePixmap);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(rotateValue + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, rotatePixmap);
        }
    }
    else
    {
        return;
    }
}

void base_Widget::onRotateFinished()
{
    m_isRoratingWindow = false;
    setCurrentWidget(widget(m_nextPageIndex));
    repaint();
}
