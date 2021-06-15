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
    user.cpp \
    doublecyemform.cpp \
    doubleemform.cpp \
    singleemform.cpp \
    runtimesettingdelegate.cpp \
    mixsettingdialog.cpp \
    keyboard.cpp \
    alerthistorydialog.cpp \
    alerthistoryform.cpp \
    yhcrunstatusdialog.cpp \
    fpjrunstatusdialog.cpp \
    controstatusdialog.cpp \
    mixdatarangesetdialog.cpp

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
    user.h \
    doublecyemform.h \
    doubleemform.h \
    singleemform.h \
    runtimesettingdelegate.h \
    mixsettingdialog.h \
    keyboard.h \
    alerthistorydialog.h \
    alerthistoryform.h \
    yhcrunstatusdialog.h \
    fpjrunstatusdialog.h \
    controstatusdialog.h \
    mixdatarangesetdialog.h

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
    mixerdlg.ui \
    doublecyemform.ui \
    doubleemform.ui \
    singleemform.ui \
    mixsettingdialog.ui \
    keyboard.ui \
    alerthistorydialog.ui \
    alerthistoryform.ui \
    yhcrunstatusdialog.ui \
    fpjrunstatusdialog.ui \
    controstatusdialog.ui \
    mixdatarangesetdialog.ui

RESOURCES += \
    resource.qrc

LIBS += -L/work/nfs/forlinx/OK5718-SDK-V1.0/OK57xx-linux-fs/usr/lib -lsqlite3 \

target.path = /home/hqtech/test/zhenninyhc
INSTALLS += target
