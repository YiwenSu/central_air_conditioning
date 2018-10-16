#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include <QStackedWidget>
#include<mainwindow.h>
#include<login.h>
#include<mutex>
class base_Widget : public QStackedWidget
{
    Q_OBJECT

public:
    base_Widget(QWidget *parent = NULL);
    ~base_Widget();

private:
    void paintEvent(QPaintEvent* event); // 绘制旋转效果
private slots:
    void onRotateWindow(); // 开始旋转窗口
    void onRotateFinished(); // 窗口旋转结束

private:
    bool m_isRoratingWindow;  // 当前窗口是否正在旋转;
    int m_nextPageIndex;
    login* m_loginWindow;
    MainWindow* m_mainWindow;
};

#endif // BASE_WIDGET_H
