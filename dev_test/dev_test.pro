TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

target.path = /home/hqtech/luozx
INSTALLS += target

SOURCES += main.c \
    ../mhd_lib/src/ComFun.c \
    ../mhd_lib/src/CRC16.c \
    ../mhd_lib/src/File_ReadandWrite.c \
    ../mhd_lib/src/Public_DataType.c \
    ../mhd_lib/src/Share_Memory.c \
    ../mhd_lib/src/Mpu_gpio.c\
    local_src/mapp.c \
    local_src/CommFun.c \
    local_src/keyboard_input.c \
    local_src/MPUCtrDev.c \
    local_src/mapp_init.c \
    Dev/FPJ/FPJ.c \
    Dev/HLJ/HLXT.c \
    Dev/YXC/YXC.c


HEADERS += \
    ../mhd_lib/inc/ComFun.h \
    ../mhd_lib/inc/CRC16.h \
    ../mhd_lib/inc/File_ReadandWrite.h \
    ../mhd_lib/inc/mhd_lib.h \
    ../mhd_lib/inc/Public_DataType.h \
    ../mhd_lib/inc/Share_Memory.h \
    ../mhd_lib/inc/Mpu_gpio.h\
    local_inc/mapp.h \
    local_inc/CommFun.h \
    local_inc/keyboard_input.h \
    local_inc/MPUCtrDev.h \
    local_inc/mapp_init.h \
    local_inc/DEV_CONF.h \
    Dev/FPJ/FPJ.h \
    Dev/HLJ/HLXT.h \
    Dev/YXC/YXC.h


