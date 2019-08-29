/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Remote433_ctr.c
 *       Description:  433MHz遥控器
 *       Version:  1.0.0(08/25/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/25/2019 23:31 PM"
 ****************************************************************/

#include "Remote433_ctr.h"
#include "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

#define WIRE433_SETEN   94   //控制引脚 GPIO3_30 拉高设置模式，拉低通讯模式

Remote433_data mHD_Rem433Data;

/************* 433MHz遥控器接收初始化 **********************************************
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
int  mHD_Remote_433_Init(char *port,int speed)
{
    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
    int err;               //返回调用函数的状态

    mHD_Request_GPIO(WIRE433_SETEN);                             //申请GPIO
    mHD_SetGPIO_Direction(WIRE433_SETEN,"out");           //设置GPIO方向为输出
    mHD_SetGPIO_Value(WIRE433_SETEN,ATS62_Comm);    //设置GIPO状态为通讯模式

    fd = mHD_Uart__Open(port);  //打开串口
    if(fd<0 ) { printf("Can't Open Serial Port");return -1;}

    err = mHD_Uart_Init(fd,speed,0,8,1,'N'); //初始化串口
    if(err <0) {printf("set serial parameter error!\n"); return -1;}
    mHD_Rem433Data.fd = fd;
    return fd; //返回文件串口设备文件描述
}
/****************设置433MHz遥控器无线参数*************
* 名称：            mHD_Keyboare_LED_Send
* 功能：            发送数据
* 入口参数：     add 无线模块地址设置
*                         speed  7:6 00:8N1,01:8O1,10:8E1,11:8N1
*                                     5:3 000=1200,001=2400,010=4800,011=9600,100=19200,101=38400,110=57600,111=115200 串口波特率
*                                     2:0 000=0.3K,001=1.2K,010=2.4K,011=4.8K,100=9.6K,101=19.2K  传输空中速率
*                         chan: 0x00-0x1FH  410-441MHz   通信频率 = 410M+chan*1M
*                         option: 7  0=透明传输,1=定点传输
*                                      6  1=TXD,AUX推挽输出,RXD 上拉输入,0=TXD,AUX 开路输出,RXD 开路输入  IO 驱动方式
*                                      5:3 000=250ms,001=500ms,010=750ms,011=1000ms,100=1250ms,101=1500ms,110=1750ms,111=2000ms 无线唤醒时间
*                                      2  0=关闭 FEC,1=打开 FEC
*                                      1:0  00=30dBm,01=27dBm,10=24dBm,11=21dBm  发射功率
* 出口参数：    正确返回发送数据的长度，错误返回为-1
* 设置命令共6个byte  bit0 固定为0xC0,bit1固定为0x14
*                                    bit3 固定为0x1C 8N1,9600,9.6K空速
*                                    bit5 固定为0xC0 定点传输,TXD,AUX推挽输出,RXD 上拉输入,250ms,关闭 FEC,30dBm
*                                    bit2=bit4  FPXT=0x0B 翻抛系接
*                                                     HLXT=0x17 混料系统
*                                                     SLXT=0x15 上料系统接收器
*                                                     YTH =0x19 一体化系统
*                                                     FLXT=0x1B 肥料系统
*   设置命令    地址高     地址低      空速9.6K    通讯频率    定向传输
 *  bit0-0xCO  bit1-0x14  bit2-0x0B   bit3-0x1C  bit4-0x0B  bit5-0xC0
 *************************************************************************/
int mHD_Remote_433_Set(uint16_t add,uint8_t speed,uint8_t chan,uint8_t option )
{
    uint8_t txbuf[6];
    int txlen;
    int len;

    txbuf[0] = 0xC0;
    txbuf[1] = (add>>8) & 0xFF;
    txbuf[2] = add & 0xFF;
    txbuf[3] = speed;
    txbuf[4] =  chan;
    txbuf[5] =  option;
    txlen =6;

    mHD_SetGPIO_Value(WIRE433_SETEN,ATS62_Setting);    //设置GIPO状态
    mHD_msleep(100);
    len = mHD_Uart_Send(mHD_Rem433Data.fd,(char *)txbuf,txlen);
    if(len <1) return FALSE;
    else return TRUE;
}

/****************设置遥控器对于的设备*************
* 名称：            mHD_Remote_433Dev_Set
* 功能：            发送数据
* 入口参数：    dev 设备设置
*                                                     FPXT=0x0B 翻抛系接
*                                                     HLXT=0x17 混料系统
*                                                     SLXT=0x15 上料系统接收器
*                                                     YTH =0x19 一体化系统
*                                                     FLXT=0x1B 肥料系统
 *************************************************************************/
int mHD_Remote_433Dev_Set(uint8_t dev)
{
    uint8_t txbuf[6];
    int txlen;
    int len;
    txbuf[0] = 0xC0;
    txbuf[1] = 0x14;
    txbuf[2] = txbuf[4] =  dev;
    txbuf[3] = 0x1C;
    txbuf[5] = 0xC0;
    txlen =6;
    mHD_SetGPIO_Value(WIRE433_SETEN,ATS62_Setting);    //设置GIPO状态
    mHD_msleep(100);
    len = mHD_Uart_Send(mHD_Rem433Data.fd,(char *)txbuf,txlen);
    if(len <1) return FALSE;

    /* 打印设置数据*/
//    int i;
//    printf("Setting data= ");
//    for(i=0;i<len;i++) {
//        if(i !=len-1) printf("%X,",txbuf[i]);
//        else printf("%X\n",txbuf[i]);
//    }
   /* 等待设置参数返回数据 */
//    uint8_t rcv_buf[256];
//    int rxlen =-1;
//    while(rxlen<0) {
//        rxlen = mHD_Uart_Recv(mHD_Rem433Data.fd, (char *)rcv_buf,sizeof(rcv_buf));
//        if(rxlen>0) {
//            //打印返回数据
//            printf("Setting Retrun data= ");
//            for(i=0;i<rxlen;i++) {
//                if(i !=rxlen-1) printf("%X,",rcv_buf[i]);
//                else printf("%X\n",rcv_buf[i]);
//            }
//        }
//    }
    /* 读取设置参赛 */
//    txbuf[0] = txbuf[1] =txbuf[2] =0xC1;
//    len = mHD_Uart_Send(mHD_Rem433Data.fd,(char *)txbuf,3);
//    rxlen =-1;
//    while(rxlen<0) {
//        rxlen = mHD_Uart_Recv(mHD_Rem433Data.fd, (char *)rcv_buf,sizeof(rcv_buf));
//        if(rxlen>0) {
//            //控制显示接收到的遥控器数据
//            printf("Read Setting Return data= ");
//            for(i=0;i<rxlen;i++) {
//                if(i !=rxlen-1) printf("%X,",rcv_buf[i]);
//                else printf("%X\n",rcv_buf[i]);
//            }
//        }
//    }
  mHD_SetGPIO_Value(WIRE433_SETEN,ATS62_Comm);    //设置GIPO状态为通讯模式
   return 0;
}

/**************** 接受遥控器数据****************************************
* 名称：            mHD_Remote_433_Recv
* 功能：            读取遥控器数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
int mHD_Remote_433_Recv(void)
{
    char rcv_buf[256];
    int len = 0;
    int i;
    uint16_t sum =0;
    uint8_t   sumL =0;

    len = mHD_Uart_Recv(mHD_Rem433Data.fd, rcv_buf,sizeof(rcv_buf));
    if(len>0)
    {
        //控制台显示接收到的遥控器数据
//        printf("Remote data= ");
//        for(i=0;i<len;i++) {
//            if(i !=len-1)  printf("%X,",rcv_buf[i]);
//            else printf("%X\n",rcv_buf[i]);
//        }
        //解析接收到的数据
        if(rcv_buf[0] !=AS62_RX_CMD) return -1;  //头命令错误退出
        for(i=0;i<AS62_RX_NUM-1;i++) sum +=rcv_buf[i]; //求校验和
        sumL = sum & 0xFF;
        if(rcv_buf[AS62_RX_NUM-1] != sumL) return -1; 		//校验和错误
        //数据正确赋值
        mHD_Rem433Data.BT_11 = rcv_buf[1] & 0x01;
        mHD_Rem433Data.BT_12 = (rcv_buf[1]>>1) & 0x01;
        mHD_Rem433Data.BT_13 = (rcv_buf[1]>>2) & 0x01;
        mHD_Rem433Data.BT_JT  = (rcv_buf[1]>>3) & 0x01;

         mHD_Rem433Data.BT_21 = (rcv_buf[1]>>4) & 0x01;
         mHD_Rem433Data.BT_22 = (rcv_buf[1]>>5) & 0x01;
         mHD_Rem433Data.BT_23 = (rcv_buf[1]>>6) & 0x01;
         mHD_Rem433Data.BT_24 = (rcv_buf[1]>>7) & 0x01;

         mHD_Rem433Data.BT_31 = (rcv_buf[2]>>0) & 0x01;
         mHD_Rem433Data.BT_32 = (rcv_buf[2]>>1) & 0x01;
         mHD_Rem433Data.BT_33 = (rcv_buf[2]>>2) & 0x01;
         mHD_Rem433Data.BT_34 = (rcv_buf[2]>>3) & 0x01;

         mHD_Rem433Data.BT_41 = (rcv_buf[2]>>4) & 0x01;
         mHD_Rem433Data.BT_42 = (rcv_buf[2]>>5) & 0x01;
         mHD_Rem433Data.BT_43 = (rcv_buf[2]>>6) & 0x01;
         mHD_Rem433Data.BT_44 = (rcv_buf[2]>>7) & 0x01;

         mHD_Rem433Data.sec_dev = (rcv_buf[3]>>0) & 0x07;
         mHD_Rem433Data.speed = (rcv_buf[3]>>3) & 0x07;
         mHD_Rem433Data.dev = (rcv_buf[3]>>6) & 0x03;

         //控制台显示解析后遥控器数据
          if(HqDev_CmdSys.debug ==1)
          {
              printf("Remote analyze data\n ");
              printf("JT:         %d,   \n",mHD_Rem433Data.BT_JT);
              printf("Dev:      %d,   \n",mHD_Rem433Data.dev );
              printf("SecDev: %d,   Speed: %d     \n",mHD_Rem433Data.sec_dev ,mHD_Rem433Data.speed);
              printf("1:  %d, %d, %d,      \n",mHD_Rem433Data.BT_11,mHD_Rem433Data.BT_12,mHD_Rem433Data.BT_13);
              printf("2:  %d, %d, %d, %d\n",mHD_Rem433Data.BT_21,mHD_Rem433Data.BT_22,mHD_Rem433Data.BT_23,mHD_Rem433Data.BT_24);
              printf("3:  %d, %d, %d, %d\n",mHD_Rem433Data.BT_31,mHD_Rem433Data.BT_32,mHD_Rem433Data.BT_33,mHD_Rem433Data.BT_34);
              printf("4:  %d, %d, %d, %d\n",mHD_Rem433Data.BT_41,mHD_Rem433Data.BT_42,mHD_Rem433Data.BT_43,mHD_Rem433Data.BT_44);
          }
    }
    return 0;
}















