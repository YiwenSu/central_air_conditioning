#-------------------------------------------------
#
# Project created by QtCreator 2018-06-02T15:26:16
#
#-------------------------------------------------

QT       += core gui
QT       += sql
LIBS += -lws2_32
QT += charts
qtHaveModule(printsupport): QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = air_main
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    database.cpp \
    centralac.cpp \
    req.cpp \
    account.cpp \
    login.cpp \
    base_widget.cpp \
    centralac_com.cpp \
    currentinfo.cpp
    target.path = $$[QT_INSTALL_EXAMPLES]/charts/splinechart
    INSTALLS += target

HEADERS += \
        mainwindow.h \
    database.h \
    centralac.h \
    req.h \
    account.h \
    login.h \
    base_widget.h \
    centralac_com.h \
    currentinfo.h

FORMS += \
        mainwindow.ui \
    login.ui

RESOURCES += \
    bottom.qrc

DISTFILES += \
    materials/try.png \
    materials/logo.png \
    materials/icon.jpg
