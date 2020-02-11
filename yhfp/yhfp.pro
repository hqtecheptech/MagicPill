#-------------------------------------------------
#
# Project created by QtCreator 2019-05-09T17:24:18
#
#-------------------------------------------------

QT       += core gui charts network xml sql

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
    datareceiver.cpp \
    global.cpp \
    syscontroller.cpp \
    ../dev_app/mhd_lib/src/ComFun.c \
    ../dev_app/mhd_lib/src/CRC16.c \
    ../dev_app/mhd_lib/src/File_ReadandWrite.c \
    ../dev_app/mhd_lib/src/Mpu_gpio.c \
    ../dev_app/mhd_lib/src/Public_DataType.c \
    ../dev_app/mhd_lib/src/Share_Memory.c \
    ../dev_app/mhd_lib/src/Uart_Port.c \
    fpjc.cpp \
    smallplate.cpp \
    fpjwatchs.cpp \
    sqlitehelper.cpp \
    databaseworker.cpp \
    historydlg.cpp \
    controlhistory.cpp \
    mixerdlg.cpp \
    uiglobal.cpp \
    tcpclientsocket.cpp \
    parseserverdataworker.cpp \
    identity.cpp \
    user.cpp

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
    datareceiver.h \
    global.h \
    dataformat.h \
    syscontroller.h \
    ../dev_app/mhd_lib/inc/ComFun.h \
    ../dev_app/mhd_lib/inc/CRC16.h \
    ../dev_app/mhd_lib/inc/File_ReadandWrite.h \
    ../dev_app/mhd_lib/inc/mhd_lib.h \
    ../dev_app/mhd_lib/inc/Mpu_gpio.h \
    ../dev_app/mhd_lib/inc/Public_DataType.h \
    ../dev_app/mhd_lib/inc/Share_Memory.h \
    ../dev_app/mhd_lib/inc/Uart_Port.h \
    fpjc.h \
    smallplate.h \
    fpjwatchs.h \
    sqlitehelper.h \
    databaseworker.h \
    historydlg.h \
    controlhistory.h \
    mixerdlg.h \
    uiglobal.h \
    tcpclientsocket.h \
    parseserverdataworker.h \
    identity.h \
    user.h

FORMS    += mainwindow.ui \
    yhcc.ui \
    yhcdial.ui \
    yhcwatchs.ui \
    ylzsplate.ui \
    wdywplate.ui \
    dydlplate.ui \
    zsylchart.ui \
    lrzsplate.ui \
    fpjc.ui \
    smallplate.ui \
    fpjwatchs.ui \
    historydlg.ui \
    controlhistory.ui \
    mixerdlg.ui

RESOURCES += \
    resource.qrc

LIBS += -L/work/nfs/forlinx/OK5718-SDK-V1.0/OK57xx-linux-fs/usr/lib -lsqlite3 \

target.path = /home/hqtech/test/yhc
INSTALLS += target
