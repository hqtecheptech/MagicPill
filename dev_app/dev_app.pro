TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

target.path = /home/hqtech/ctrsoft
INSTALLS += target

SOURCES += main.c \
    HqDeviceDriver/keyboard_led_ctr.c \
    HqDeviceDriver/Remote433_ctr.c \
    SysInitRun/Sys_Init.c \
    SysInitRun/Sys_Run.c \
    mhd_lib/src/ComFun.c \
    mhd_lib/src/CRC16.c \
    mhd_lib/src/DelayAndTiming.c \
    mhd_lib/src/File_ReadandWrite.c \
    mhd_lib/src/Mpu_gpio.c \
    mhd_lib/src/Public_DataType.c \
    mhd_lib/src/Share_Memory.c \
    mhd_lib/src/Uart_Port.c \
    HqDeviceDriver/MPUCtrDev.c \
    HqDeviceDriver/Module_system.c \
    HqDeviceDriver/keyboard_input.c \
    Local_app/Module_GlobalData.c \
    Local_app/MPU_GlobalData.c \
    HqDeviceDriver/Wireless24_Comm.c \
    HqDeviceDriver/ConFile_conf.c \
    HqDeviceDriver/MPU_system.c \
    SysTime_Callback.c \
    PRUSoft_Ctl/PRU_Protocol.c \
    PRUSoft_Ctl/PRU_ConfigWrite.c \
    PRUSoft_Ctl/PRU_Fun.c \
    PRUSoft_Ctl/PRU_Ctl.c \
    PRUSoft_Ctl/PRU_Uart.c \
    Module_ctl/Module_Ctl.c \
    Module_ctl/Module_Uart.c \
    Dev/HLXT/HLXT_App.c \
    Dev/HLXT/HLXT_Fun.c \
    Dev/HLXT/GXQY1_Project/GSQY1_HLXT_App.c \
    Dev/HLXT/GXQY1_Project/GSQY1_HLXT_Hardware_Config.c \
    Dev/FPXT/FPXT_Fun.c \
    Dev/FPXT/FPXT_App.c \
    Dev/FPXT/GXQY1_Project/GSQY1_FPXT_App.c \
    Dev/FPXT/GXQY1_Project/GSQY1_FPXT_Hardware_Config.c \
    mhd_lib/src/Dev_Fun.c

HEADERS += \
    HqDeviceDriver/keyboard_led_ctr.h \
    HqDeviceDriver/Remote433_ctr.h \
    SysInitRun/Sys_Init.h \
    SysInitRun/Sys_Run.h \
    mhd_lib/inc/ComFun.h \
    mhd_lib/inc/CRC16.h \
    mhd_lib/inc/DelayAndTiming.h \
    mhd_lib/inc/File_ReadandWrite.h \
    mhd_lib/inc/mhd_lib.h \
    mhd_lib/inc/Mpu_gpio.h \
    mhd_lib/inc/Public_DataType.h \
    mhd_lib/inc/Share_Memory.h \
    mhd_lib/inc/Uart_Port.h \
    HqDeviceDriver/MPUCtrDev.h \
    HqDeviceDriver/Module_system.h \
    HqDeviceDriver/keyboard_input.h \
    HqDeviceDriver/HqDeviceDriver.h \
    Local_app/Module_GlobalData.h \
    Local_app/MPU_GlobalData.h \
    Local_app/HQDevApp_conf.h \
    HqDeviceDriver/Wireless24_Comm.h \
    HqDeviceDriver/ConFile_conf.h \
    HqDeviceDriver/MPU_system.h \
    dev_app_conf.h \
    SysTime_Callback.h \
    PRUSoft_Ctl/PRU_Protocol.h \
    PRUSoft_Ctl/PRU_ConfigWrite.h \
    PRUSoft_Ctl/PRU_Fun.h \
    PRUSoft_Ctl/PRU_Ctl.h \
    PRUSoft_Ctl/PRU_Uart.h \
    Module_ctl/Module_Ctl.h \
    Module_ctl/Module_Uart.h \
    Dev/HLXT/HLXT_App.h \
    Dev/HLXT/HLXT_Fun.h \
    Dev/HLXT/GXQY1_Project/GSQY1_HLXT_App.h \
    Dev/HLXT/GXQY1_Project/GSQY1_HLXT_Hardware_Config.h \
    Dev/FPXT/FPXT_Fun.h \
    Dev/FPXT/FPXT_App.h \
    Dev/FPXT/GXQY1_Project/GSQY1_FPXT_App.h \
    Dev/FPXT/GXQY1_Project/GSQY1_FPXT_Hardware_Config.h \
    mhd_lib/inc/Dev_Fun.h
