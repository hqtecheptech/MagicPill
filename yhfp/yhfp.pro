#-------------------------------------------------
#
# Project created by QtCreator 2019-05-09T17:24:18
#
#-------------------------------------------------

QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yhfp
CONFIG += c++11


TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    yhcc.cpp \
    yhcdial.cpp \
    yhcwatchs.cpp \
    ylzsplate.cpp \
    plateframe.cpp \
    wdywplate.cpp \
    dydlplate.cpp \
    zsylchart.cpp \
    netstatemanageworker.cpp \
    lrzsplate.cpp \
    sharehelper.cpp \
    plcdatamanageworker.cpp \
    datasender.cpp \
    taskmanager.cpp \
    datareceiver.cpp

HEADERS  += mainwindow.h \
    yhcc.h \
    yhcdial.h \
    yhcwatchs.h \
    ylzsplate.h \
    plateframe.h \
    wdywplate.h \
    dydlplate.h \
    zsylchart.h \
    netstatemanageworker.h \
    lrzsplate.h \
    data.h \
    sharehelper.h \
    plcdatamanageworker.h \
    datasender.h \
    taskmanager.h \
    datareceiver.h

FORMS    += mainwindow.ui \
    yhcc.ui \
    yhcdial.ui \
    yhcwatchs.ui \
    ylzsplate.ui \
    wdywplate.ui \
    dydlplate.ui \
    zsylchart.ui \
    lrzsplate.ui

RESOURCES += \
    resource.qrc

target.path = /home/hqtech/test
INSTALLS += target
