/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  keyboard_led.c
 *       Description:  Usat232 连接串口按键及指示灯板
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/

#include "keyboard_led_ctr.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

Keyboard_data mHD_KBData;
int mHD_Keyboare_Led_fd = -1;   //keyboarde_led 设备串口文件描述

/************* 按键板及LED指示灯初始化 **********************************************
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
int  mHD_Keyboard_Led_Init(char *port,int speed)
{
    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
    int err;               //返回调用函数的状态

    fd = mHD_Uart__Open(port);  //打开串口
    if(fd<0 )
    {
         if(HqDev_CmdSys.debug ==1) printf("Can't Open Serial Port");
        return -1;
    }

    err = mHD_Uart_Init(fd,speed,0,8,1,'N'); //初始化串口
    if(err <0)
    {
         if(HqDev_CmdSys.debug ==1) printf("set serial parameter error!\n");
         return -1;
    }
    mHD_KBData.fd = fd;
    return fd; //返回文件串口设备文件描述
}

/**************** 写入按键及LED板LED指示灯状态*************
* 名称：            mHD_Keyboare_LED_Send
* 功能：            发送数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************/
int mHD_Keyboare_LED_Send(void)
{
    int len = 0;
    int txlen;
    int i;
    uint8_t  txdata[6] = {0,0,0,0,0,0};
    uint8_t  txbuf[12];
    uint16_t crc;
    uint16_t crcL,crcH;
    for(i=0;i<8;i++)
    {
        txdata[0]  |= (mHD_KBData.OutPort1[i] &0x01) <<i;
        txdata[1]  |= (mHD_KBData.OutPort2[i] &0x01) <<i;
        txdata[2]  |= (mHD_KBData.OutPort3[i] &0x01) <<i;
        txdata[3]  |= (mHD_KBData.OutPort4[i] &0x01) <<i;
        txdata[4]  |= (mHD_KBData.OutPort5[i] &0x01) <<i;
        txdata[5]  |= (mHD_KBData.OutPort6[i] &0x01) <<i;
    }

    txbuf[0] = 0x01; txbuf[1] = 0x17; txbuf[2] = 0x01; txbuf[3] = 0x06;
    for(i=0;i<6;i++)  txbuf[4+i] = txdata[i];
    crc =  mHD_RTU_CRC16(txbuf,10);
    crcL = crc &0xff;
    crcH = (crc >> 8) &0xff;
    txbuf[10] = crcL; txbuf[11] = crcH;
    txlen =12;
    len = mHD_Uart_Send(mHD_KBData.fd,(char *)txbuf,txlen);
//     printf("send data= ");
//     for(i=0;i<len;i++)
//     {
//         if(i !=len-1) printf("%d,",txbuf[i]);
//         else printf("%d\n",txbuf[i]);
//     }
    if(len <1) return FALSE;
    else return len;
}

/**************** 读取按键及LED板状态****************************************
* 名称：            mHD_Keyboare_LED_Recv
* 功能：            接收按键板数据
* 入口参数：    无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
int mHD_Keyboare_LED_Recv(void)
{
    char rcv_buf[256];
    int len = 0;
    int i,y;
    uint16_t crc;
    uint8_t crcL,crcH;

    uint8_t *indata[9];
    indata[0] = mHD_KBData.InPort1;
    indata[1] = mHD_KBData.InPort2;
    indata[2] = mHD_KBData.InPort3;
    indata[3] = mHD_KBData.InPort4;
    indata[4] = mHD_KBData.InPort5;
    indata[5] = mHD_KBData.InPort6;
    indata[6] = mHD_KBData.InPort7;
    indata[7] = mHD_KBData.InPort8;
    indata[8] = mHD_KBData.InPort9;

    len = mHD_Uart_Recv(mHD_KBData.fd, rcv_buf,sizeof(rcv_buf));
    if(len>0)
    {
        if(((rcv_buf[0] ==0x01) && (rcv_buf[2] ==0x01)&&(rcv_buf[3] ==0x15))&& ( (rcv_buf[1] ==0x17)||(rcv_buf[1] ==0x1F)))
        {
           crc =  mHD_RTU_CRC16((uint8_t *)rcv_buf,len-2);
           crcL = crc &0xff;
           crcH = (crc >> 8) &0xff;
           if((rcv_buf[len-1] == crcH) && (rcv_buf[len-2] == crcL))
           {
               for(y=0;y<9;y++)  //InputData
               {
                   //printf("port%d data= ",y+1);
                   for(i=0;i<8;i++)
                   {
                       *(indata[y]+i) = (rcv_buf[4+y] >>i) & 0x01;
//                       if(i !=7)  printf("%d,",*(indata[y]+i));
//                       else printf("%d.\n",*(indata[y]+i));
                   }
               }
               mHD_KBData.EnPort1Data = rcv_buf[13];
               mHD_KBData.EnPort2Data = rcv_buf[15];
               mHD_KBData.EnPort3Data = rcv_buf[17];
               mHD_KBData.EnPort1Dir =  rcv_buf[14];
               mHD_KBData.EnPort2Dir =  rcv_buf[16];
               mHD_KBData.EnPort3Dir =  rcv_buf[18];
               //printf("En data= %d,%d,%d,%d,%d,%d\n",rcv_buf[13],rcv_buf[14],rcv_buf[15],rcv_buf[16],rcv_buf[17],rcv_buf[18]);
           } else return -1;
        } else return -1;
    } else return -1;
    return len;
}

/**************** 按键及LED板 轮询任务 ****************************************
* 名称：            mHD_Keyboare_LED_Poll(void)
*******************************************************************/
int mHD_Keyboare_LED_Poll(void)
{
    return 0;
}














