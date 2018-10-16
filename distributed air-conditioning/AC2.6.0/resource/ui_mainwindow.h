/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *turnonBtn;
    QLabel *label;
    QPushButton *speedBtn;
    QPushButton *modeBtn;
    QPushButton *IDBtn;
    QPushButton *turnonBtn_2;
    QToolButton *toolButton;
    QLabel *roomtem;
    QPushButton *temup;
    QPushButton *temdown;
    QLabel *time;
    QPushButton *blue;
    QPushButton *orange;
    QPushButton *pink;
    QLabel *mode_cold;
    QLabel *mode_warm;
    QLabel *speed_0;
    QLabel *speed_1;
    QLabel *speed_2;
    QLabel *speed_3;
    QLabel *tem_label;
    QLabel *tem_room_label;
    QLabel *error_label;
    QLabel *info_label;
    QLabel *warning_label;
    QLabel *success_label;
    QLabel *unuse;
    QLabel *ID_label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(717, 541);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        MainWindow->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        turnonBtn = new QPushButton(centralWidget);
        turnonBtn->setObjectName(QStringLiteral("turnonBtn"));
        turnonBtn->setGeometry(QRect(240, 450, 251, 71));
        turnonBtn->setCursor(QCursor(Qt::PointingHandCursor));
        turnonBtn->setStyleSheet(QLatin1String("QPushButton\n"
"{\n"
"	border-image: url(:/button/turnon.png);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"border-image: url(:/button/turnonhover.png);\n"
"}"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 371, 231));
        label->setStyleSheet(QStringLiteral("border-image: url(:/temperture.png);"));
        speedBtn = new QPushButton(centralWidget);
        speedBtn->setObjectName(QStringLiteral("speedBtn"));
        speedBtn->setGeometry(QRect(20, 310, 121, 121));
        speedBtn->setCursor(QCursor(Qt::PointingHandCursor));
        speedBtn->setStyleSheet(QStringLiteral("border-image: url(:/speedBtn.png);"));
        modeBtn = new QPushButton(centralWidget);
        modeBtn->setObjectName(QStringLiteral("modeBtn"));
        modeBtn->setGeometry(QRect(140, 310, 131, 121));
        modeBtn->setCursor(QCursor(Qt::PointingHandCursor));
        modeBtn->setStyleSheet(QStringLiteral("border-image: url(:/modeBtn.png);"));
        IDBtn = new QPushButton(centralWidget);
        IDBtn->setObjectName(QStringLiteral("IDBtn"));
        IDBtn->setGeometry(QRect(270, 310, 121, 121));
        IDBtn->setStyleSheet(QStringLiteral("border-image: url(:/IDBtn.png);"));
        turnonBtn_2 = new QPushButton(centralWidget);
        turnonBtn_2->setObjectName(QStringLiteral("turnonBtn_2"));
        turnonBtn_2->setGeometry(QRect(240, 450, 251, 71));
        turnonBtn_2->setCursor(QCursor(Qt::PointingHandCursor));
        turnonBtn_2->setStyleSheet(QStringLiteral("border-image: url(:/button/turnoff.png);"));
        toolButton = new QToolButton(centralWidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(650, 20, 41, 41));
        toolButton->setCursor(QCursor(Qt::PointingHandCursor));
        toolButton->setStyleSheet(QLatin1String("QToolButton\n"
"{\n"
"	\n"
"	border-image: url(:/button/menunprmal.png);\n"
"}\n"
"QToolButton:hover\n"
"{\n"
"	\n"
"	border-image: url(:/button/menuhover.png);\n"
"}"));
        toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        roomtem = new QLabel(centralWidget);
        roomtem->setObjectName(QStringLiteral("roomtem"));
        roomtem->setGeometry(QRect(410, 220, 281, 211));
        roomtem->setStyleSheet(QStringLiteral("border-image: url(:/roomtem.png);"));
        temup = new QPushButton(centralWidget);
        temup->setObjectName(QStringLiteral("temup"));
        temup->setGeometry(QRect(310, 320, 51, 41));
        temup->setCursor(QCursor(Qt::PointingHandCursor));
        temup->setStyleSheet(QStringLiteral("border-image: url(:/button/temup.png);"));
        temdown = new QPushButton(centralWidget);
        temdown->setObjectName(QStringLiteral("temdown"));
        temdown->setGeometry(QRect(310, 370, 51, 41));
        temdown->setCursor(QCursor(Qt::PointingHandCursor));
        temdown->setStyleSheet(QStringLiteral("border-image: url(:/button/temdown.png);"));
        time = new QLabel(centralWidget);
        time->setObjectName(QStringLiteral("time"));
        time->setGeometry(QRect(410, 80, 281, 41));
        time->setStyleSheet(QLatin1String("border-image: url(:/time.png);\n"
"font: 12pt \"Lato\";\n"
"color: rgb(0, 161, 133);"));
        time->setAlignment(Qt::AlignCenter);
        blue = new QPushButton(centralWidget);
        blue->setObjectName(QStringLiteral("blue"));
        blue->setGeometry(QRect(430, 140, 61, 61));
        blue->setCursor(QCursor(Qt::PointingHandCursor));
        blue->setStyleSheet(QStringLiteral("border-image: url(:/blue.png);"));
        orange = new QPushButton(centralWidget);
        orange->setObjectName(QStringLiteral("orange"));
        orange->setGeometry(QRect(520, 140, 61, 61));
        orange->setCursor(QCursor(Qt::PointingHandCursor));
        orange->setStyleSheet(QLatin1String("\n"
"border-image: url(:/orange.png);"));
        pink = new QPushButton(centralWidget);
        pink->setObjectName(QStringLiteral("pink"));
        pink->setGeometry(QRect(610, 140, 61, 61));
        pink->setCursor(QCursor(Qt::PointingHandCursor));
        pink->setStyleSheet(QStringLiteral("border-image: url(:/pink.png);"));
        mode_cold = new QLabel(centralWidget);
        mode_cold->setObjectName(QStringLiteral("mode_cold"));
        mode_cold->setGeometry(QRect(180, 330, 51, 51));
        mode_cold->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_ac_unit_white_18dp.png);"));
        mode_warm = new QLabel(centralWidget);
        mode_warm->setObjectName(QStringLiteral("mode_warm"));
        mode_warm->setGeometry(QRect(180, 330, 51, 51));
        mode_warm->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_brightness_5_white_18dp.png);"));
        speed_0 = new QLabel(centralWidget);
        speed_0->setObjectName(QStringLiteral("speed_0"));
        speed_0->setGeometry(QRect(60, 330, 51, 51));
        speed_0->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_looks_white_18dp.png);"));
        speed_1 = new QLabel(centralWidget);
        speed_1->setObjectName(QStringLiteral("speed_1"));
        speed_1->setGeometry(QRect(60, 330, 51, 51));
        speed_1->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_looks_one_white_18dp.png);"));
        speed_2 = new QLabel(centralWidget);
        speed_2->setObjectName(QStringLiteral("speed_2"));
        speed_2->setGeometry(QRect(60, 330, 51, 51));
        speed_2->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_looks_two_white_18dp.png);"));
        speed_3 = new QLabel(centralWidget);
        speed_3->setObjectName(QStringLiteral("speed_3"));
        speed_3->setGeometry(QRect(60, 330, 51, 51));
        speed_3->setStyleSheet(QStringLiteral("border-image: url(:/status/baseline_looks_3_white_18dp.png);"));
        tem_label = new QLabel(centralWidget);
        tem_label->setObjectName(QStringLiteral("tem_label"));
        tem_label->setGeometry(QRect(140, 130, 131, 81));
        tem_label->setStyleSheet(QLatin1String("font: 1075 57pt \"Lato\";\n"
"color: rgb(0, 161, 133);"));
        tem_room_label = new QLabel(centralWidget);
        tem_room_label->setObjectName(QStringLiteral("tem_room_label"));
        tem_room_label->setGeometry(QRect(490, 300, 131, 91));
        tem_room_label->setStyleSheet(QLatin1String("font: 1075 65pt \"Lato\";\n"
"color: rgb(255, 255, 255);"));
        error_label = new QLabel(centralWidget);
        error_label->setObjectName(QStringLiteral("error_label"));
        error_label->setGeometry(QRect(20, 20, 371, 41));
        error_label->setStyleSheet(QLatin1String("border-image: url(:/error.png);\n"
"font: 8pt \"Lato\";\n"
"color: rgb(255,255,255);"));
        error_label->setAlignment(Qt::AlignCenter);
        info_label = new QLabel(centralWidget);
        info_label->setObjectName(QStringLiteral("info_label"));
        info_label->setGeometry(QRect(20, 20, 371, 41));
        info_label->setStyleSheet(QLatin1String("border-image: url(:/info.png);\n"
"font: 8pt \"Lato\";\n"
"color: rgb(255,255,255);"));
        info_label->setAlignment(Qt::AlignCenter);
        warning_label = new QLabel(centralWidget);
        warning_label->setObjectName(QStringLiteral("warning_label"));
        warning_label->setGeometry(QRect(20, 20, 371, 41));
        warning_label->setStyleSheet(QLatin1String("border-image: url(:/warning.png);\n"
"font: 8pt \"Lato\";\n"
"color: rgb(255,255,255);"));
        warning_label->setAlignment(Qt::AlignCenter);
        success_label = new QLabel(centralWidget);
        success_label->setObjectName(QStringLiteral("success_label"));
        success_label->setGeometry(QRect(20, 20, 371, 41));
        success_label->setStyleSheet(QLatin1String("border-image: url(:/success.png);\n"
"font: 8pt \"Lato\";\n"
"color: rgb(255,255,255);"));
        success_label->setAlignment(Qt::AlignCenter);
        unuse = new QLabel(centralWidget);
        unuse->setObjectName(QStringLiteral("unuse"));
        unuse->setGeometry(QRect(620, 290, 21, 21));
        unuse->setStyleSheet(QStringLiteral("border-image: url(:/white_wendu.png);"));
        ID_label = new QLabel(centralWidget);
        ID_label->setObjectName(QStringLiteral("ID_label"));
        ID_label->setGeometry(QRect(410, 20, 241, 41));
        ID_label->setStyleSheet(QLatin1String("border-image: url(:/menu-2.png);\n"
"font: 8pt \"Lato\";\n"
"color: rgb(255,255,255);"));
        ID_label->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        turnonBtn_2->raise();
        turnonBtn->raise();
        label->raise();
        speedBtn->raise();
        modeBtn->raise();
        IDBtn->raise();
        toolButton->raise();
        roomtem->raise();
        temup->raise();
        temdown->raise();
        time->raise();
        blue->raise();
        orange->raise();
        pink->raise();
        mode_cold->raise();
        mode_warm->raise();
        speed_0->raise();
        speed_1->raise();
        speed_2->raise();
        speed_3->raise();
        tem_label->raise();
        tem_room_label->raise();
        error_label->raise();
        info_label->raise();
        warning_label->raise();
        success_label->raise();
        unuse->raise();
        ID_label->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        turnonBtn->setText(QString());
        label->setText(QString());
        speedBtn->setText(QString());
        modeBtn->setText(QString());
        IDBtn->setText(QString());
        turnonBtn_2->setText(QString());
        toolButton->setText(QString());
        roomtem->setText(QString());
        temup->setText(QString());
        temdown->setText(QString());
        time->setText(QApplication::translate("MainWindow", "time", Q_NULLPTR));
        blue->setText(QString());
        orange->setText(QString());
        pink->setText(QString());
        mode_cold->setText(QString());
        mode_warm->setText(QString());
        speed_0->setText(QString());
        speed_1->setText(QString());
        speed_2->setText(QString());
        speed_3->setText(QString());
        tem_label->setText(QApplication::translate("MainWindow", "15", Q_NULLPTR));
        tem_room_label->setText(QApplication::translate("MainWindow", "13", Q_NULLPTR));
        error_label->setText(QString());
        info_label->setText(QString());
        warning_label->setText(QString());
        success_label->setText(QString());
        unuse->setText(QString());
        ID_label->setText(QApplication::translate("MainWindow", "id", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
