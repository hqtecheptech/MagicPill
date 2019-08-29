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

#include "Sys_Init.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"
#include "Local_app/MPU_GlobalData.h"
#include "Local_app/Module_GlobalData.h"

/*** 按键及LED板连接设置****/
#define KEYLED_UART             "/dev/ttyS1"    //按键LED板连接的 串口2 RS232电平
#define KEYLED_BAUD             115200           //按键LED板串口2 通信波特率
/*** 遥控器连接设置****/
#define REMOTE433_UART      "/dev/ttyS3"   //无线遥控器连接的串口 4 TTL电平
#define REMOTE433_BAUD      9600              //无线遥控器连接的串口 4 通信波特率
W433Hz_ModuleCtr   RemoteSysSet = ATS62_FPXT;   //设置遥控器类型
/*** 2.4G无线通信模块连接接设置****/
#define WIRELESS24_UART     "/dev/ttyS4"    //按键LED板连接的 串口5 TTL电平
#define WIRELESS24_BAUD     115200           //按键LED板串口2 通信波特率

/*** 系统主定时器 时基设置****/
#define SYSTEM_MASTIMERM     100000          //单位微妙

msgname Msg_ModType = msg_CtrSoft;      //设置消息类型

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
    int gpio_run2 = -1;
     /*** 运行配置文件初始化 ***/
    mHD_Read_Config_Conf();       //初始化运行配置文件
    /*** step0: 初始化 控制器本体功能 ***/
        //0.0: 初始化 控制器本体GPIO输入输出功能
    mHD_MPU_ConfigData_Init();  //MPU控制器IO配置参赛赋值
    mHD_MPU_ConfigModule_Set();     //写入控制器系统 通过配置驱动模块hq_gpio_set 写入系统内核配置GPIO功能
        //0.1: 初始化控制器控制模块按键驱动板
    mHD_Keyboard_Led_Init(KEYLED_UART,KEYLED_BAUD);                 //设置连接按键LED板的串口,及通信波特率
        //0.2:初始化无线遥控器433MHz接口
    mHD_Remote_433_Init(REMOTE433_UART,REMOTE433_BAUD);   //设置接收433MHz遥控器的串口,及通信波特率
    mHD_Remote_433Dev_Set(RemoteSysSet );                                           //设置接收遥控器的类型
        //0.3:初始化无线通讯2.4G接口
    mHD_Wireless_24_Init(WIRELESS24_UART,WIRELESS24_BAUD);
        //0.4:初始化激光开关扫描传感器接口

        //0.5:初始化无线热电堆温度传感器接口

        //0.6:初始化远程IO模块连接接口

        //0.7:初始化系统主定时器及延时定时器
    mHD_Timer_SystemMaster_Init(SYSTEM_MASTIMERM);  //设定并启动系统主定时器
    mHD_TimeTON_Init(); //初始化开延时定时器及关延时定时器
        //0.8:初始化键盘调试环境 Debug 开关设置
   if(HqDev_CmdSys.debug ==1) mHD_Debug_KeyboardInput_Init(); //键盘输入 调试

    /* step1: 初始化控制器连接模块*/
        //1.0:初始化接收信号,初始化共享内存通信系统
   gpio_run2 = mHD_Read_GPIO(BUTRUN_MPU_2);  //读取设置按键 RUN_2 状态
   if(HqDev_CmdSys.linkmodule ==1) //如果连接外设IO模块系统
   {
       mHD_ModuleSoftware_Init(Msg_ModType,&Run_data.Pid[1]);   //初始化模块系统
       mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //读取共享内存区
       if(gpio_run2 == 0)  //Run_2 引脚设在为ON 按键控制设置参数
       {
           mHD_Module_ConfigData_Init();   //初始化配置参数
           mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //写入共享内存区
           mHD_Send_Msg_Cmd(Msg_WritePara,Msg_ModType);  //发送写入配置命令同时读取参数
       } else  //配置运行文件设置
       {
           if(HqDev_CmdSys.setmodule ==1)  //软件运行配置文件 设置模块参数
           {
               mHD_Module_ConfigData_Init();   //初始化配置参数
               mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //写入共享内存区
               mHD_Send_Msg_Cmd(Msg_WritePara,Msg_ModType);  //发送写入配置命令
           }
           if(HqDev_CmdSys.readmodule ==1 ) //软件运行配置文件 读取模块参数
           {
               mHD_Send_Msg_Cmd(Msg_ReadPara,Msg_ModType);  //读取模块设置参数
           }
       }
       /* 检测模块运行状态 */
       mHD_msleep(500);
       mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //读取共享内存区

       /* 检测模块参数配置状态 */
       if(Dev_data.Pru.MRSetStatus !=1)   //如果没有设置 根据设置参数设置模块参数
       {
           mHD_Module_ConfigData_Init();   //初始化配置参数
           mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //写入共享内存区
           mHD_Send_Msg_Cmd(Msg_WritePara,Msg_ModType);  //发送写入配置命令
       }
       if(Dev_data.Pru.MRSetStatus !=1) //如果没有读取 读取模块参数
       {
           mHD_Send_Msg_Cmd(Msg_ReadPara,Msg_ModType);  //读取模块设置参数
       }
       if(Dev_data.Pru.MScanStatus !=1)  //如果自动扫描没有开启,开启扫描
       {
           mHD_Send_Msg_Cmd(Msg_StartScan,Msg_ModType);   //启动模块自动运行命令
       }
       mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //读取共享内存区
   }
    return 0;
}

















