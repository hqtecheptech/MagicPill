/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Sys_Run.c
 *       Description:  系统运行
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 22:28 PM"
 ****************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "Sys_Run.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"
#include"../dev_app_conf.h"  //配置文件

/************* 系统设备任务轮询 **********************************************
 * 名称：             mHD_Sys_Dev_Run
 * 功能：             系统设备任务轮询
 * 入口参数：      无
 * 出口参数：      0 = 完成，-1 =失败
 * ********************************************************************************/
int  mHD_Sys_Dev_Run(void)
{
    /* Debug 键盘输入命令轮询 */
    if(HqDev_CmdSys.debug ==1)  mHD_keyboard_Cmd(MSG_MOD_TYPE);
    /* 连接IO模块数据消息轮询*/
     if((HqDev_CmdSys.linkmodule ==1)&&(MODULE_DEVIO ==1))
     {
         mHD_Readmsg_Poll(MSG_MOD_TYPE );   /*** 消息队列轮询 ***/
         mHD_CompDev_SigToRPU();
     }
    /* 控制器本体GPIO 输入状态轮询*/
    mHD_MPUGPIO_FUN_InputValue(); //控制器本体 GPIO 输入轮询
    /*** 控制器顶部 LED 指示灯 输出轮询 ****/
    mHD_MPUTOPLED_FUN_OutValue(); //控制器顶部 LED 指示灯 输出轮询
    /***接遥控器数据***/
    if(REMOTE433_ENABLE) mHD_Remote_433_Recv(); // 接收遥控器数据
    /*** 按键及LED板数据收发 ***/
    if(KEYLED_ENABLE)
    {
        mHD_Keyboare_LED_Recv();//读取按键及LED板状态
        mHD_Keyboare_WriteData_Poll(); //按键及LED板发送轮询任务
    }


    return 0;

}

/***************** 控制器本体 GPIO 4种输出模式 使用例程 *****************/
//#define  MPU_DO_GPIO                  "gpio"              //GPIO输出
//#define  MPU_DO_TIMER                "timer"            //GPIO输出使用Timer 功能
//#define  MPU_DO_TF                       "mmc0"           //GPIO输出使用TF 指示功能
//#define  MPU_DO_HEART               "heartbeat"     //GPIO输出使用心跳指示功能
//    for(i=0;i<4;i++)  mHD_Write_MPUGPIO_Value(i,MPU_DO_GPIO,0,0,0);  //GPIO 功能
//    for(i=0;i<4;i++)  mHD_Write_MPUGPIO_Value(i,MPU_DO_TIMER,1,200,500);  //timer 功能 亮200ms 灭500ms
//    for(i=0;i<4;i++)  mHD_Write_MPUGPIO_Value(i,MPU_DO_TF ,1,0,0);  // "mmc0" 功能 TF SD卡读写指示
//    for(i=0;i<4;i++)  mHD_Write_MPUGPIO_Value(i,MPU_DO_HEART ,1,0,0);  //  "heartbeat" 功能 心跳指示功能
/***************** 控制器本体 脉冲输出   使用例程 *****************/
//    for(i=0;i<4;i++) mHD_Write_MPUPulse_Enable(i,1);  //首先打开通道   ch4-7,1=打开 0=关闭
//    for(i=0;i<4;i++) mHD_Write_MPUPulse_Value(i,2,50,1);  //输出通道 4-7 输出频率为2Hz,占空比50% 正输出
//    for(i=0;i<4;i++) mHD_Write_MPUPulse_Enable(i,0);;  //关闭输出













