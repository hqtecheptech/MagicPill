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
Keyboard_Uart mHD_KBPort;
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

    fd = mHD_Uart_Open(port);  //打开串口
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
    mHD_KBPort.fd = fd;
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
    len = mHD_Uart_Send(mHD_KBPort.fd,(char *)txbuf,txlen);
     if(HqDev_CmdSys.debug ==1)
     {
         printf("send data= ");
         for(i=0;i<len;i++)
         {
             if(i !=len-1) printf("%d,",txbuf[i]);
             else printf("%d\n",txbuf[i]);
         }
     }

    if(len <1) return FALSE;
    else return len;
}

/**************** 接收按键面板数据解析****************************************
* 名称：            mHD_Remote_433RXData_Analysis(void)
* 功能：            接受到的 串口数据解析
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
uint8_t *indata[9] = {mHD_KBData.InPort1,mHD_KBData.InPort2,mHD_KBData.InPort3,mHD_KBData.InPort4,
                                   mHD_KBData.InPort5,mHD_KBData.InPort6,mHD_KBData.InPort7,mHD_KBData.InPort8,mHD_KBData.InPort9};

int mHD_Keyboard_LEDRXData_AnalysisPoll(void)
{
    int i,y;
    uint16_t crc;
    uint8_t crcL,crcH;

    if(mHD_KBPort.RxComplete ==1)
    {
        mHD_KBPort.RxComplete =0;
        printf("kbdat num = %d data = ",mHD_KBPort.RxNum);
        for(i=0;i<mHD_KBPort.RxNum;i++)
        {
            printf("%d ",mHD_KBPort.Rxbuf[i]);
        }
        printf("\n ");
        if(((mHD_KBPort.Rxbuf[0] ==0x01) && (mHD_KBPort.Rxbuf[2] ==0x01)&&(mHD_KBPort.Rxbuf[3] ==0x15))&& ( (mHD_KBPort.Rxbuf[1] ==0x17)||(mHD_KBPort.Rxbuf[1] ==0x1F)))
        {
           crc =  mHD_RTU_CRC16((uint8_t *)mHD_KBPort.Rxbuf,mHD_KBPort.RxNum-2);
           crcL = crc &0xff;
           crcH = (crc >> 8) &0xff;
           if((mHD_KBPort.Rxbuf[mHD_KBPort.RxNum-1] == crcH) && (mHD_KBPort.Rxbuf[mHD_KBPort.RxNum-2] == crcL))
           {
               for(y=0;y<9;y++)  //InputData
               {
                   if(HqDev_CmdSys.debug ==1) printf("portkeyled %d data= ",y+1);
                   for(i=0;i<8;i++)
                   {
                       *(indata[y]+i) = (mHD_KBPort.Rxbuf[4+y] >>i) & 0x01;
                       if(HqDev_CmdSys.debug ==1)
                       {
                           if(i !=7)  printf("%d,",*(indata[y]+i));
                          else printf("%d.\n",*(indata[y]+i));
                       }
                   }
               }
               mHD_KBData.EnPort1Data = mHD_KBPort.Rxbuf[13];
               mHD_KBData.EnPort2Data = mHD_KBPort.Rxbuf[15];
               mHD_KBData.EnPort3Data = mHD_KBPort.Rxbuf[17];
               mHD_KBData.EnPort1Dir =  mHD_KBPort.Rxbuf[14];
               mHD_KBData.EnPort2Dir =  mHD_KBPort.Rxbuf[16];
               mHD_KBData.EnPort3Dir =  mHD_KBPort.Rxbuf[18];
               if(HqDev_CmdSys.debug ==1)  printf("En data= %d,%d,%d,%d,%d,%d\n",mHD_KBPort.Rxbuf[13],mHD_KBPort.Rxbuf[14],mHD_KBPort.Rxbuf[15],mHD_KBPort.Rxbuf[16],mHD_KBPort.Rxbuf[17],mHD_KBPort.Rxbuf[18]);
           } else return -1;
        } else return -1;
    } else return -1;\
    return 0;
}

/**************** 读取按键及LED板状态****************************************
* 名称：            mHD_Keyboare_LED_Recv
* 功能：            接收按键板数据
* 入口参数：    无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
int mHD_Keyboare_LED_Recv(void)
{
    int len;
     len = mHD_Uart_Recv(mHD_KBPort.fd, (char *)mHD_KBPort.Rxbuf,KEYLED_RX_MAX);
    if(len>0)
    {
        mHD_KBPort.RxComplete =1;
        mHD_KBPort.RxNum = len;
        return mHD_KBPort.RxNum;
    }
    return -1;
}

/**************** 线程接收按键面板数据****************************************
* 名称：            mHD_Keyboard_LED_Thread(void)
* 功能：            读取按键面板数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
void mHD_Keyboard_LED_Thread(void)
{
    int len;
    while(1)
    {
        if(mHD_KBPort.RxComplete !=1)
        {
            len = mHD_Uart_Recv(mHD_KBPort.fd, (char *)mHD_KBPort.Rxbuf,KEYLED_RX_MAX);
            if(len>0)
            {
                mHD_KBPort.RxComplete =1;
                mHD_KBPort.RxNum = len;
            }
        }
    }
}
/**************** 创建线程接收按键面板数据****************************************
* 名称：             mHD_Keyboard_LED_CreatThread(void)
* 功能：            创建线程接收按键面板数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
int mHD_Keyboard_LED_CreatThread(void)
{
    int ret;
    ret = pthread_create(&mHD_KBPort.Thread_Rev_ID,NULL,(void *) mHD_Keyboard_LED_Thread,NULL);
    if(ret !=0)
    {
        if(HqDev_CmdSys.debug ==1)
        {
            printf("Create Uart_Receive_thread error!\n");
            return -1;
        }
    }
    return 0;
}

/**************** 按键及LED板发送轮询任务 ****************************************
* 名称：            mHD_Keyboare_WriteData_Poll(void)
*******************************************************************/
int mHD_Keyboare_WriteData_Poll(void)
{
    static uint8_t old_KBData[6][8];
    uint8_t *outdata[6];
    int i,y;
    int en = 0;

    outdata[0] = mHD_KBData.OutPort1;
    outdata[1] = mHD_KBData.OutPort2;
    outdata[2] = mHD_KBData.OutPort3;
    outdata[3] = mHD_KBData.OutPort4;
    outdata[4] = mHD_KBData.OutPort5;
    outdata[5] = mHD_KBData.OutPort6;

    for(y=0;y<6;y++)
    {
        for(i=0;i<8;i++)
        {
            if(old_KBData[y][i] != *(outdata[y]+i))   en =1;
             if(en ==1) break;
        }
        if(en ==1) break;
    }

    if(en ==1)
    {
        for(y=0;y<6;y++)
        {
            for(i=0;i<8;i++) old_KBData[y][i] = *(outdata[y]+i);
        }
        return mHD_Keyboare_LED_Send();
    }else return -1;
}





