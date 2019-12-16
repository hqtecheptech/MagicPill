QT += core network xml sql
QT -= gui

CONFIG += c++11

TARGET = HqswServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sharehelper.cpp \
    global.cpp \
    taskmanager.cpp \
    sqlitehelper.cpp \
    plcdatamanageworker.cpp \
    datareceiver.cpp \
    datasender.cpp \
    syscontroller.cpp \
    databaseworker.cpp \
    netstatemanageworker.cpp \
    swserver.cpp

HEADERS += \
    taskmanager.h \
    plcdatamanageworker.h \
    datareceiver.h \
    dataformat.h \
    global.h \
    datasender.h \
    data.h \
    sqlitehelper.h \
    sharehelper.h \
    databaseworker.h \
    netstatemanageworker.h \
    syscontroller.h \
    swserver.h

LIBS += -L/work/nfs/forlinx/OK5718-SDK-V1.0/OK57xx-linux-fs/usr/lib -lsqlite3 \

target.path = /home/hqtech/test
INSTALLS += target
