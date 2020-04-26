QT += core network xml sql
QT -= gui


CONFIG += c++11

TARGET = swmonitor
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sharehelper.cpp \
    global.cpp

HEADERS += \
    sharehelper.h \
    data.h \
    dataformat.h \
    global.h

target.path = /home/root
INSTALLS += target
