#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T16:48:34
#
#-------------------------------------------------

QT       += core gui charts network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hqsw
CONFIG += c++11


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
    ufftank.cpp \
    fftank.cpp \
    centerwidget.cpp \
    tcpclientsocket.cpp \
    global.cpp \
    tankstatebarchart.cpp \
    environment.cpp \
    fercontroldialog.cpp \
    fancontroldialog.cpp \
    mycustomslider.cpp \
    requestworkthread.cpp \
    historydialog.cpp \
    mytimerthread.cpp \
    reportdialog.cpp \
    identity.cpp \
    user.cpp \
    logindialog.cpp \
    adduserdialog.cpp \
    realtimedialog.cpp \
    settingdialog.cpp \
    runtimesettingdelegate.cpp \
    customfersettingdialog.cpp \
    alertquerydialog.cpp \
    tanghe_centerwidget.cpp \
    deocontroldialog.cpp \
    tankairvalve.cpp \
    pipevalve.cpp \
    deodorationfan.cpp \
    deosettingdialog.cpp \
    renhuai_centerwidget.cpp \
    fanvalvecontroldialog.cpp \
    rotatefan.cpp \
    deodorationfanform.cpp \
    tankairvalveform.cpp \
    testdialog.cpp \
    renhuainew.cpp \
    renhuaitabwidget.cpp \
    renhuaifercontroltabpagewidget.cpp \
    loginhistoryform.cpp \
    ferfanvalvecontroldialog.cpp \
    sharedfancontroldialog.cpp \
    alerthistoryform.cpp \
    deocontroltabpagewidget.cpp \
    environmentparams.cpp \
    singelfertankvalvecontroldialog.cpp \
    requestworker.cpp \
    xinyifercontroltabwidget.cpp \
    sensorcontroldialog.cpp \
    checkboxdelegate.cpp \
    parseserverdataworker.cpp \
    singledeodorationfan.cpp \
    basepartwidget.cpp \
    testpart.cpp \
    basetankwidget.cpp \
    fercaseform.cpp \
    fercase.cpp \
    ferstep.cpp \
    ferstepform.cpp \
    ferconfigdialog.cpp \
    ferconfig.cpp

HEADERS  += mainwindow.h \
    ufftank.h \
    fftank.h \
    centerwidget.h \
    tcpclientsocket.h \
    dataformat.h \
    global.h \
    tankstatebarchart.h \
    environment.h \
    fercontroldialog.h \
    fancontroldialog.h \
    mycustomslider.h \
    requestworkthread.h \
    historydialog.h \
    mytimerthread.h \
    reportdialog.h \
    identity.h \
    user.h \
    logindialog.h \
    adduserdialog.h \
    realtimedialog.h \
    settingdialog.h \
    runtimesettingdelegate.h \
    customfersettingdialog.h \
    alertquerydialog.h \
    tanghe_centerwidget.h \
    deocontroldialog.h \
    tankairvalve.h \
    pipevalve.h \
    deodorationfan.h \
    deosettingdialog.h \
    renhuai_centerwidget.h \
    fanvalvecontroldialog.h \
    rotatefan.h \
    deodorationfanform.h \
    tankairvalveform.h \
    testdialog.h \
    renhuainew.h \
    renhuaitabwidget.h \
    renhuaifercontroltabpagewidget.h \
    loginhistoryform.h \
    ferfanvalvecontroldialog.h \
    sharedfancontroldialog.h \
    alerthistoryform.h \
    deocontroltabpagewidget.h \
    environmentparams.h \
    singelfertankvalvecontroldialog.h \
    requestworker.h \
    xinyifercontroltabwidget.h \
    sensorcontroldialog.h \
    checkboxdelegate.h \
    parseserverdataworker.h \
    singledeodorationfan.h \
    basepartwidget.h \
    testpart.h \
    basetankwidget.h \
    fercaseform.h \
    fercase.h \
    ferstep.h \
    ferstepform.h \
    ferconfigdialog.h \
    ferconfig.h

FORMS    += mainwindow.ui \
    fftank.ui \
    centerwidget.ui \
    environment.ui \
    fercontroldialog.ui \
    fancontroldialog.ui \
    historydialog.ui \
    reportdialog.ui \
    logindialog.ui \
    adduserdialog.ui \
    realtimedialog.ui \
    settingdialog.ui \
    customfersettingdialog.ui \
    alertquerydialog.ui \
    tanghe_centerwidget.ui \
    tankstatebarchart.ui \
    deocontroldialog.ui \
    tankairvalve.ui \
    pipevalve.ui \
    deodorationfan.ui \
    deosettingdialog.ui \
    ufftank.ui \
    renhuai_centerwidget.ui \
    fanvalvecontroldialog.ui \
    rotatefan.ui \
    deodorationfanform.ui \
    tankairvalveform.ui \
    testdialog.ui \
    renhuainew.ui \
    renhuaitabwidget.ui \
    renhuaifercontroltabpagewidget.ui \
    loginhistoryform.ui \
    ferfanvalvecontroldialog.ui \
    sharedfancontroldialog.ui \
    alerthistoryform.ui \
    deocontroltabpagewidget.ui \
    singelfertankvalvecontroldialog.ui \
    xinyifercontroltabwidget.ui \
    sensorcontroldialog.ui \
    singledeodorationfan.ui \
    testpart.ui \
    fercaseform.ui \
    ferstepform.ui \
    ferconfigdialog.ui

RESOURCES += \
    src.qrc

LIBS += -L/work/nfs/forlinx/OK5718-SDK-V1.0/OK57xx-linux-fs/usr/lib -lsqlite3 \

target.path = /home/hqtech/test/fermentation
INSTALLS += target

#LIBS += -L/home/hqtech/test/fermentation/ -lControllerCommon

#INCLUDEPATH += /home/hqtech/test/fermentation
#DEPENDPATH += /home/hqtech/test/fermentation

#unix:!macx: LIBS += -L$$PWD/../../build-ControllerCommon-OK5718-Debug/ -lControllerCommon

#INCLUDEPATH += $$PWD/../../build-ControllerCommon-OK5718-Debug
#DEPENDPATH += $$PWD/../../build-ControllerCommon-OK5718-Debug
