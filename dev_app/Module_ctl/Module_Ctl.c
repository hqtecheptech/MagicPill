/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  PRU_Protocol.c
 *       Description:  读写配置文件
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/

#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>

#include "Module_Ctl.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"


#define PRU_UART             "/dev/ttyS0"    //按键LED板连接的 串口2 RS232电平


//int Pru_fd;
//int mHD_Pru_Test(void)
//{
//   int len;
//    char txbuf[] = "This is a pru uart0 test\n";

//    len = mHD_Uart_Send(Pru_fd,txbuf,strlen(txbuf));
//    if(len <1) return FALSE;
//    return 0;
//}


//void Pru_Uart_Test(void)
//{
//    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
//    int err;               //返回调用函数的状态

//    fd = mHD_Uart_Open(PRU_UART );  //打开串口
//    if(fd<0 )
//    {
//         if(HqDev_CmdSys.debug ==1) printf("Can't Open Serial Port");
//        return ;
//    }

//    err = mHD_Uart_Init(fd,500000,0,8,1,'N',0,1); //初始化串口
//    if(err <0)
//    {
//         if(HqDev_CmdSys.debug ==1) printf("set serial parameter error!\n");
//         return ;
//    }
//    Pru_fd = fd;

//    mHD_Pru_Test( );


//    return ; //返回文件串口设备文件描述

//}




