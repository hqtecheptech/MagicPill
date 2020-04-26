#-------------------------------------------------
#
# Project created by QtCreator 2019-05-09T17:24:18
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = swtest
CONFIG += c++11


TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sharehelper.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    data.h \
    sharehelper.h \
    global.h \
    dataformat.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

#LIBS += -L/work/nfs/forlinx/OK5718-SDK-V1.0/OK57xx-linux-fs/usr/lib -lsqlite3 \

target.path = /home/hqtech/swtest
INSTALLS += target
