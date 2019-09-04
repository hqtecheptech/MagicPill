/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Sys_Init.c
 *       Description:  系统初始化
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 15:41 PM"
 ****************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Sys_Init.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"
#include "Local_app/MPU_GlobalData.h"
#include "Local_app/Module_GlobalData.h"
#include "../PRUSoft_Ctl/PRU_Fun.h"
#include "../PRUSoft_Ctl/PRU_Ctl.h"
#include "../Module_ctl/Module_Uart.h"


#include"../dev_app_conf.h"  //配置文件


#define KEYLED_UART             "/dev/ttyS1"    //按键LED板连接的 串口2 RS232电平
#define REMOTE433_UART      "/dev/ttyS3"   //无线遥控器连接的串口 4 TTL电平
#define WIRELESS24_UART     "/dev/ttyS4"    //按键LED板连接的 串口5 TTL电平
#define MODULE_MPUGPIO   "insmod /home/hqtech/hqmod/hq_gpio_set.ko"
#define MODULE_GPIOLED    "insmod /home/hqtech/hqmod/hq_gpioled_tca6424.ko"

/************* 系统设备初始化 **********************************************
 * 名称：             mHD_Sys_Dev_Init
 * 功能：             系统设备初始化
 * 入口参数：      无
 * 出口参数：      0 = 初始化完成，-1 =初始化失败
 * return :            FALSE -1  错误，TURE = 成功
 * 初始化流程      开始->打开串口----------------->设置串口----------------->结束
 *                                      |                                    |
 *                                      ->失败(返回错误)           ->失败(返回错误)
 * ********************************************************************************/
//#define BUTRUN_RPU_1   193   //RUN_1
#define BUTRUN_MPU_2  194     //RUN_2

int  mHD_Sys_Dev_Init(void)
{
    //初始化系统主定时器及延时定时器
    mHD_Timer_SystemMaster_Init(SYSTEM_MASTIMERM);  //设定并启动系统主定时器
    mHD_TimeTON_Init(); //初始化开延时定时器及关延时定时器
    /***** 加载模块****************/
    system(MODULE_MPUGPIO);   //加载MPU GPIO SET 模块
    system(MODULE_GPIOLED);     //加载 MPU TOP LED SET 模块
     /*** 运行配置文件初始化 ***/
    mHD_Read_RUNConfig_Conf();       //初始化运行配置文件
    /*** 初始化 控制器本体功能 ***/
        // 初始化 控制器本体GPIO输入输出功能
    mHD_MPU_ConfigData_Init();  //MPU控制器IO配置参赛赋值
    mHD_MPU_ConfigModule_Set();     //写入控制器系统 通过配置驱动模块hq_gpio_set 写入系统内核配置GPIO功能
        //初始化控制器控制模块按键驱动板

    /* step1: 初始化控制器连接模块*/
   if((HqDev_CmdSys.linkmodule ==1) &&(MODULE_DEVIO ==1))//如果连接外设IO模块系统
   {
      mHD_Module_Init(115200);
   }

   if(KEYLED_ENABLE ==1)
   {
       mHD_Keyboard_Led_Init(KEYLED_UART,KEYLED_BAUD);  //设置连接按键LED板的串口,及通信波特率
       mHD_Keyboard_LED_CreatThread(); //创建线程接收按键面板数据
   }
       //初始化无线遥控器433MHz接口
   if(REMOTE433_ENABLE ==1)
   {
       mHD_Remote_433_Init(REMOTE433_UART,REMOTE433_BAUD);   //设置接收433MHz遥控器的串口,及通信波特率
       mHD_Remote_433Dev_Set(REMOTE_SYS_SET );                                           //设置接收遥控器的类型
       mHD_Remote_433Recv_CreatThread(); //创建线程接收遥控器数据
   }
       //初始化无线通讯2.4G接口
   if(WIRELESS24_ENABLE == 1) mHD_Wireless_24_Init(WIRELESS24_UART,WIRELESS24_BAUD);
       //初始化激光开关扫描传感器接口

       //初始化无线热电堆温度传感器接口

       //初始化远程IO模块连接接口

       //初始化键盘调试环境 Debug 开关设置
  //if(HqDev_CmdSys.debug ==1) mHD_Debug_KeyboardInput_Init(); //键盘输入 调试

    return 0;
}





















