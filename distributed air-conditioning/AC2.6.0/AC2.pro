#-------------------------------------------------
#
# Project created by QtCreator 2018-06-16T15:16:19
#
#-------------------------------------------------

QT       += core gui
LIBS += -lws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AC2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ac.cpp \
    ac_com.cpp \
    accom.cpp \
    login.cpp \
    waitdialog.cpp \
    workthread.cpp

HEADERS  += mainwindow.h \
    ac.h \
    ac_com.h \
    accom.h \
    login.h \
    waitdialog.h \
    workthread.h \
    resource/ui_login.h \
    resource/ui_mainwindow.h

FORMS    += mainwindow.ui \
    login.ui \
    waitdialog.ui

RESOURCES += \
    resources.qrc \
    resource/resourses.qrc

DISTFILES += \
    resource/wait.gif \
    resource/button/blue.png \
    resource/button/home.png \
    resource/button/homehover.png \
    resource/button/hover.png \
    resource/button/menuhover.png \
    resource/button/menunprmal.png \
    resource/button/navi.png \
    resource/button/orange.png \
    resource/button/pink.png \
    resource/button/temdown.png \
    resource/button/temup.png \
    resource/button/turnoff.png \
    resource/button/turnon.png \
    resource/button/turnonhover.png \
    resource/digits/1.png \
    resource/digits/2.png \
    resource/digits/3.png \
    resource/login/background.png \
    resource/login/button.png \
    resource/login/facebook.png \
    resource/login/forget.png \
    resource/login/inputline.png \
    resource/login/loginback.png \
    resource/login/title.png \
    resource/status/baseline_ac_unit_white_18dp.png \
    resource/status/baseline_brightness_5_white_18dp.png \
    resource/status/baseline_looks_3_white_18dp.png \
    resource/status/baseline_looks_one_white_18dp.png \
    resource/status/baseline_looks_two_white_18dp.png \
    resource/status/baseline_looks_white_18dp.png \
    resource/status/blue.png \
    resource/status/orange.png \
    resource/status/pink.png \
    resource/1.png \
    resource/BACK.png \
    resource/background.png \
    resource/bill.png \
    resource/blue.png \
    resource/dot.png \
    resource/energy.png \
    resource/error.png \
    resource/holiday.png \
    resource/home.png \
    resource/homehover.png \
    resource/huakuai.png \
    resource/IDBtn.png \
    resource/info.png \
    resource/menu-1.png \
    resource/menu-2.png \
    resource/modeBtn.png \
    resource/orange.png \
    resource/pink.png \
    resource/realback.png \
    resource/roomtem.png \
    resource/speedBtn.png \
    resource/success.png \
    resource/temperture.png \
    resource/time.png \
    resource/warning.png \
    resource/white_wendu.png \
    resource/button/buttons.psd \
    resource/button/hoverpsd.psd \
    resource/button/normal.psd \
    resource/digits/dangerbtn.psd \
    resource/digits/数字.psd \
    resource/alert.psd \
    resource/blue.psd \
    resource/temperture.psd \
    resource/kongtiao.png
