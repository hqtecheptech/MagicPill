/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Wireless2.4_Comm.c
 *       Description:  2.4G通讯
 *       Version:  1.0.0(08/25/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/25/2019 23:31 PM"
 ****************************************************************/

#include "Wireless24_Comm.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

/************* 2.4G通讯接收初始化 **********************************************
 * 名称：             mHD_Keyboare_LED_Init
 * 功能：             按键板通信端口初始化
 * 入口参数：      char * port      串口号(ttyS0,ttyS1,ttyS3,ttyS4,ttyS5,ttyS6,ttyS7)
 *                          注：keyboard_led 默认连接到串口2, “/dev/ttyS1"
 *                          speed      串口速度
 * 出口参数：      串口设备文件描述，错误返回为-1
 * return :             FALSE -1  错误，正确返回文件描述符
 * 打开流程          开始->打开串口----------------->设置串口----------------->结束
 *                                      |                                    |
 *                                      ->失败(返回错误)           ->失败(返回错误)
 * ********************************************************************************/
int mHD_Wireless_24_Init(char *port,int speed)
{
    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
    int err;               //返回调用函数的状态

    fd = mHD_Uart_Open(port);  //打开串口
    if(fd<0 )
    {
         if(HqDev_CmdSys.debug ==1) printf("Can't Open Serial Port");
         return -1;
    }

    err = mHD_Uart_Init(fd,speed,0,8,1,'N',0,1); //初始化串口
    if(err <0) {printf("set serial parameter error!\n"); return -1;}
    //mHD_Rem433Data.fd = fd;
    return fd; //返回文件串口设备文件描述
    return 0;
}














