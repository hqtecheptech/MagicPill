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

#include "PRU_ConfigWrite.h"
#include "PRU_Protocol.h"
#include "PRU_Fun.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

uint8_t PRU_Protocol_CmdBuf[8] ={0x13,0x04,0x00,0x02,0x00,0x00,0x0E1,0xB2};
uint8_t PRU_Protocol_PareBuf[512] = {0x00,0x15,0x00,0x0A};

PRU_ProTocol PRU_Rpmsg;

/************* 调试模式打印信息  **********************************************
 * 名称：             mHD_Debug_Printf
 * 功能：             读取系统运行配置文件如果不存在则创建
 * 入口参数：      data 需要打印的信息，len 打印个数，ser 打印编号
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回文件描述符
 * *********************************************************************************/
void mHD_Debug_Printf(uint8_t *data,uint8_t len,uint8_t ser)
{
    uint8_t i;

    if(HqDev_CmdSys.debug !=1) return;
    printf("PRU Debug serial number%d: ",ser); //Debug
    for(i=0;i<len;i++)
    {
        printf("%X ", data[i]);
    }
    printf("\n"); //Debug
}

/************* 协议应答命令 无回复  **********************************************
 * 名称：             mHD_Pru_Protocol_ReplyInit
 * 功能：             PRU 通讯协议应答回复
 * 入口参数：      cmd 命令, data 回复数据
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回文件描述符
 * *********************************************************************************/
void  mHD_Pru_Protocol_ReplyInit(uint8_t cmd,uint8_t *data)
{
    int i;
    uint16_t crc;

    PRU_Rpmsg.Txbuf[0] = 0x00;  //BitNum
    PRU_Rpmsg.Txbuf[1] = cmd;  //Function Code
    PRU_Rpmsg.Txbuf[2] = 0x00; //Reg
    PRU_Rpmsg.Txbuf[3] = PRU_CMD_CTRRETURN_LEN;  //Data Len

    for(i=0;i<4;i++) PRU_Rpmsg.Txbuf[4+i] = *(data+i);

    crc = mHD_RTU_CRC16(PRU_Rpmsg.Txbuf,8);
    PRU_Rpmsg.Txbuf[8] = crc & 0xFF;
    PRU_Rpmsg.Txbuf[9] = crc>>8 & 0xFF;
    PRU_Rpmsg.TxLen =10;
    mHD_Debug_Printf(PRU_Rpmsg.Txbuf,PRU_Rpmsg.TxLen,0);  //打印输出信息
    write(PRU_Rpmsg.Rpmsg_fd, PRU_Rpmsg.Txbuf, PRU_Rpmsg.TxLen);  //写命令到PRU 处理器
}

/************* 控制命令  **********************************************
 * 名称：             mHD_Pru_Protocol_ReplyInit
 * 功能：             发送控制命令到PRU 处理器，等待返回，如果300ms内无数据返回，
 *                         产生超时错误，如果返回数据格式及校验码错误返回，传输数据错误
 *                         如果正确，返回RPU发来的错误码
 * 入口参数：      cmd 命令, data: 需要写入的数据指针
 * return :            错误代码02超时错误，0无错误
 * *********************************************************************************/
uint8_t  mHD_Pru_Protocol_SetCmd(uint8_t cmd,uint8_t *data)
 {
     uint8_t i;
     uint16_t crc;
     uint8_t crc_L,crc_H;
     uint16_t delay =50000; // 500ms

     PRU_Rpmsg.Txbuf[0] = 0x00;  //BitNum
     PRU_Rpmsg.Txbuf[1] = cmd;  //Function Code
     PRU_Rpmsg.Txbuf[2] = 0x00; //Reg
     PRU_Rpmsg.Txbuf[3] = PRU_CMD_CTRRETURN_LEN;  //Data Len

     for(i=0;i<4;i++) PRU_Rpmsg.Txbuf[4+i] = *(data+i);

     crc = mHD_RTU_CRC16(PRU_Rpmsg.Txbuf,8);
     PRU_Rpmsg.Txbuf[8] = crc & 0xFF;
     PRU_Rpmsg.Txbuf[9] = crc>>8 & 0xFF;
     PRU_Rpmsg.TxLen =10;

     mHD_Debug_Printf(PRU_Rpmsg.Txbuf,PRU_Rpmsg.TxLen,0);  //打印输出信息

     write(PRU_Rpmsg.Rpmsg_fd, PRU_Rpmsg.Txbuf, PRU_Rpmsg.TxLen);  //写命令到PRU 处理器
     while(delay--)
     {
         PRU_Rpmsg.RxLen = read(PRU_Rpmsg.Rpmsg_fd,PRU_Rpmsg.Rxbuf,512);
         if((PRU_Rpmsg.RxLen>=8)&&(PRU_Rpmsg.RxLen<256))
         {
            mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,01);  //打印接收到的信息

             if((PRU_Rpmsg.Rxbuf[1] == cmd ) && (PRU_Rpmsg.Rxbuf[3] ==  PRU_CMD_CTRRETURN_LEN ))
             {
                 crc = mHD_RTU_CRC16(PRU_Rpmsg.Rxbuf,8);
                 crc_L = crc & 0xFF;
                 crc_H = crc>>8 & 0xFF;
                 if((PRU_Rpmsg.Rxbuf[8]==crc_L) && (PRU_Rpmsg.Rxbuf[9]==crc_H))  return PRU_Rpmsg.Rxbuf[4];  //返会RPU返回的错误代码
                 else return Err_CRC;  //返回数据校验错误
             } else return Err_Data;    //返回命令码错误
         } else
         {
             usleep(10);
             if(delay <= 10000) return Err_Timeout;    //返回超时错误,PRU未返回数据
         }
     }
     return 0;
 }

/************* 模块参数设置 **********************************************
 * 名称：             mHD_Pru_Protocol_SetData
 * 功能：             发送参数设置命令到PRU 处理器，等待返回，如果300ms内无数据返回，
 *                          产生超时错误，如果返回数据格式及校验码错误返回，传输数据错误
 *                         如果正确，返回RPU发来的错误码
 * 入口参数：      setdata: 需要写入的数据指针
 * return :            错误代码02超时错误，0无错误
 * *********************************************************************************/
uint8_t  mHD_Pru_Protocol_SetData(Mod_set  setdata)
 {
    uint8_t i;
    uint16_t crc;
    uint8_t crc_L,crc_H;
    uint32_t delay =200000; // 1000ms

   PRU_Rpmsg.Txbuf[0] = 0x00;  //位号
   PRU_Rpmsg.Txbuf[1] = PRU_CMU_WRITESET;  //位号
   PRU_Rpmsg.Txbuf[2] = 0x00;  //位号
   PRU_Rpmsg.Txbuf[3] = (setdata.name >>8) &0xFF;
   PRU_Rpmsg.Txbuf[4] = setdata.name  &0xFF;
   PRU_Rpmsg.Txbuf[5] = PRU_CMU_WRITESET; //功能码
   PRU_Rpmsg.Txbuf[6] = 0x00;  //寄存器

   PRU_Rpmsg.Txbuf[8]  = (setdata.name >>8) &0xFF;
   PRU_Rpmsg.Txbuf[9]  = setdata.name  &0xFF;
   PRU_Rpmsg.Txbuf[10]  = setdata.ver;
   PRU_Rpmsg.Txbuf[11]  = setdata.bitnum;
   PRU_Rpmsg.Txbuf[12]  = M_SetSysData.Modbaud;

    switch(setdata.name)
    {
       case 0x7122:
           PRU_Rpmsg.Txbuf[7] = PRU_CMD_SET7122_LEN;  //9
           for(i=0;i<4;i++) PRU_Rpmsg.Txbuf[13+i] =setdata.setdata[i];
           break;
       case 0x8012:
          PRU_Rpmsg.Txbuf[7] = PRU_CMD_SET8012_LEN;  //25
          for(i=0;i<20;i++) PRU_Rpmsg.Txbuf[13+i] =setdata.setdata[i];
           break;
       case 0x7313:
           PRU_Rpmsg.Txbuf[7] = PRU_CMD_SET7313_LEN;  //38
           for(i=0;i<21;i++) PRU_Rpmsg.Txbuf[13+i] =setdata.setdata[i];
           for(i=0;i<6;i++)
           {
               PRU_Rpmsg.Txbuf[34+2*i] = (setdata.esturn[i]) >>8 & 0xFF;
               PRU_Rpmsg.Txbuf[35+2*i] = (setdata.esturn[i]) & 0xFF;
           }
           break;
        case 0x7314:
           PRU_Rpmsg.Txbuf[7] = PRU_CMD_SET7314_LEN;  //38
           for(i=0;i<17;i++) PRU_Rpmsg.Txbuf[13+i] =setdata.setdata[i];
           for(i=0;i<3;i++)
           {
               PRU_Rpmsg.Txbuf[30+2*i] = (setdata.esturn[i] >>8)  & 0xFF;
               PRU_Rpmsg.Txbuf[31+2*i] = setdata.esturn[i] & 0xFF;
           }
           break;
    }

    crc = mHD_RTU_CRC16(PRU_Rpmsg.Txbuf,PRU_Rpmsg.Txbuf[7]+8);
    PRU_Rpmsg.Txbuf[PRU_Rpmsg.Txbuf[7]+8] = crc & 0xFF;
    PRU_Rpmsg.Txbuf[PRU_Rpmsg.Txbuf[7]+9] = crc>>8 & 0xFF;
    PRU_Rpmsg.TxLen =PRU_Rpmsg.Txbuf[7]+10;

    mHD_Debug_Printf(PRU_Rpmsg.Txbuf,PRU_Rpmsg.TxLen,6);  //打印设置信息
    write(PRU_Rpmsg.Rpmsg_fd, PRU_Rpmsg.Txbuf, PRU_Rpmsg.TxLen);  //写命令到PRU 处理器
    while(delay--)
    {
        PRU_Rpmsg.RxLen = read(PRU_Rpmsg.Rpmsg_fd,PRU_Rpmsg.Rxbuf,512);
        if((PRU_Rpmsg.RxLen>=8)&&(PRU_Rpmsg.RxLen<=256))
        {
            mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,4); //打印返回的信息
            if(PRU_Rpmsg.Rxbuf[1] == PRU_CMD_ERR) return Err_Data;
            else if((PRU_Rpmsg.Rxbuf[1] == PRU_CMU_WRITESET ) && (PRU_Rpmsg.Rxbuf[3] ==  PRU_CMD_SETRETURN_LEN  ))
            {
                crc = mHD_RTU_CRC16(PRU_Rpmsg.Rxbuf,8);
                crc_L = crc & 0xFF;
                crc_H = crc>>8 & 0xFF;
                if((PRU_Rpmsg.Rxbuf[8]==crc_L) && (PRU_Rpmsg.Rxbuf[9]==crc_H)) return PRU_Rpmsg.Rxbuf[4];
                else return Err_CRC;
            } else return Err_Data;
        } else
        {
            usleep(10);
            if(delay == 0) return 0x0D;
        }
    }
    return 0;
}
/************* 设置参数读取解析 **********************************************
 * 名称：             mHD_Pru_Protocol_ReadSetData
 * 功能：             发送参数设置命令到PRU 处理器，等待返回，如果300ms内无数据返回，
 *                          产生超时错误，如果返回数据格式及校验码错误返回，传输数据错误
 *                          如果正确，返回RPU发来的错误码
 * 入口参数：      setdata: 需要写入的数据指针
 * return :            错误代码02超时错误，0无错误
 * **************************************************************************/
uint8_t  mHD_Pru_Protocol_ReadSetData(uint8_t *modnum)
 {
    uint16_t i;
    uint16_t name =0;
    uint8_t len;
    uint8_t bitnum;
    uint16_t crc;
    uint8_t crc_L,crc_H;

    int fs_sel;
    fd_set fs_read;
    struct timeval time;

    FD_ZERO(&fs_read);      //指定的文件描述符集清空
    FD_SET(PRU_Rpmsg.Rpmsg_fd,&fs_read);    //用于在文件描述符集合中增加一个新的文件描述符

    time.tv_sec = 0;
    time.tv_usec = 0;
    //使用select实现串口的多路通信
    fs_sel = select(PRU_Rpmsg.Rpmsg_fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel<=0) return -1;


    PRU_Rpmsg.RxLen = read(PRU_Rpmsg.Rpmsg_fd,PRU_Rpmsg.Rxbuf,512);
    if((PRU_Rpmsg.RxLen>=8)&&(PRU_Rpmsg.RxLen<=256))
    {
        mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,3); //打印接收到的信息
        *modnum = PRU_Rpmsg.Rxbuf[0];
        if(PRU_Rpmsg.Rxbuf[0] >=32) return Err_Data;
        else if((PRU_Rpmsg.Rxbuf[3] == PRU_CMD_READSET ) && (PRU_Rpmsg.Rxbuf[4] ==  MODULE_SETREG_ADD  ))
    {
        len = PRU_Rpmsg.Rxbuf[5];
        crc = mHD_RTU_CRC16(PRU_Rpmsg.Rxbuf,len+6);
        crc_L = crc & 0xFF;
        crc_H = crc>>8 & 0xFF;
        if((PRU_Rpmsg.Rxbuf[len+6]==crc_L) && (PRU_Rpmsg.Rxbuf[len+7]==crc_H))
        {
            name = (PRU_Rpmsg.Rxbuf[1]<<8) | PRU_Rpmsg.Rxbuf[2];
            bitnum = PRU_Rpmsg.Rxbuf[0];
            switch (name) {
            case MODULE_7122:
                if(PRU_Rpmsg.Rxbuf[5] != MODULE7122_RPARA_PDULEN) return Err_Data;
                Dev_data.Mconfig[bitnum].Name = name;
                Dev_data.Mconfig[bitnum].Ver = PRU_Rpmsg.Rxbuf[8];
                Dev_data.Mconfig[bitnum].BitNum = PRU_Rpmsg.Rxbuf[9];
                Dev_data.Mconfig[bitnum].Baud = PRU_Rpmsg.Rxbuf[10];
                for(i=0;i<4;i++) Dev_data.Mconfig[bitnum].DInFilterTime[i] = PRU_Rpmsg.Rxbuf[11+i];
                break;
            case MODULE_8012:
                if(PRU_Rpmsg.Rxbuf[5] != MODULE8012_RPARA_PDULEN) return Err_Data;
                Dev_data.Mconfig[bitnum].Name = name;
                Dev_data.Mconfig[bitnum].Ver = PRU_Rpmsg.Rxbuf[8];
                Dev_data.Mconfig[bitnum].BitNum = PRU_Rpmsg.Rxbuf[9];
                Dev_data.Mconfig[bitnum].Baud = PRU_Rpmsg.Rxbuf[10];
                for(i=0;i<4;i++)
                {
                    Dev_data.Mconfig[bitnum].AOutVReg[i] = PRU_Rpmsg.Rxbuf[11+2*i];
                    Dev_data.Mconfig[bitnum].AOutAReg[i] = PRU_Rpmsg.Rxbuf[12+2*i];
                }
                for(i=0;i<12;i++) Dev_data.Mconfig[bitnum].AInReg[i] = PRU_Rpmsg.Rxbuf[19+i];
                break;
            case MODULE_7313:
                if(PRU_Rpmsg.Rxbuf[5] != MODULE7313_RPARA_PDULEN) return Err_Data;
                Dev_data.Mconfig[bitnum].Name = name;
                Dev_data.Mconfig[bitnum].Ver = PRU_Rpmsg.Rxbuf[8];
                Dev_data.Mconfig[bitnum].BitNum = PRU_Rpmsg.Rxbuf[9];
                Dev_data.Mconfig[bitnum].Baud = PRU_Rpmsg.Rxbuf[10];
                for(i=0;i<8;i++) Dev_data.Mconfig[bitnum].DOutFunSet[i] = PRU_Rpmsg.Rxbuf[11+i];
                for(i=0;i<12;i++) Dev_data.Mconfig[bitnum].DInFunSet[i] = PRU_Rpmsg.Rxbuf[19+i];
                Dev_data.Mconfig[bitnum].DInFilterTime[0] = PRU_Rpmsg.Rxbuf[31];
                for(i=0;i<6;i++) Dev_data.Mconfig[bitnum].EnSCPulseSet[i] = (PRU_Rpmsg.Rxbuf[32+2*i]<<8) | PRU_Rpmsg.Rxbuf[33+2*i];
                break;
            case MODULE_7314:
                if(PRU_Rpmsg.Rxbuf[5] != MODULE7314_RPARA_PDULEN) return Err_Data;
                Dev_data.Mconfig[bitnum].Name = name;
                Dev_data.Mconfig[bitnum].Ver = PRU_Rpmsg.Rxbuf[8];
                Dev_data.Mconfig[bitnum].BitNum = PRU_Rpmsg.Rxbuf[9];
                Dev_data.Mconfig[bitnum].Baud = PRU_Rpmsg.Rxbuf[10];
                for(i=0;i<8;i++) Dev_data.Mconfig[bitnum].DOutFunSet[i] = PRU_Rpmsg.Rxbuf[11+i];
                for(i=0;i<6;i++) Dev_data.Mconfig[bitnum].DInFunSet[i] = PRU_Rpmsg.Rxbuf[19+i];
                for(i=0;i<2;i++) Dev_data.Mconfig[bitnum].SSIFunSet[i] = PRU_Rpmsg.Rxbuf[25+i];
                Dev_data.Mconfig[bitnum].DInFilterTime[0] = PRU_Rpmsg.Rxbuf[27];
                for(i=0;i<3;i++) Dev_data.Mconfig[bitnum].EnSCPulseSet[i] = (PRU_Rpmsg.Rxbuf[28+2*i]<<8) | PRU_Rpmsg.Rxbuf[29+2*i];
                break;
            default:   return Err_Data;
            }
        } else return Err_CRC;
    } else return Err_Data;
  }
    return Err_None;
}

/****** 应答命令 无回复******/
static void  mHD_DevData_ReplyAndInit(void)
{
   uint8_t  pru_send_buf[10];
   pru_send_buf[0] = PUR_CMD_REPLY;  //建立与PRU处理器的连接
   pru_send_buf[1] = 0x00;  //保留
   pru_send_buf[2] = 0x00;        //PRU处理器波特率
   pru_send_buf[3] = 0x00;       //Module 波特率
   mHD_Pru_Protocol_ReplyInit(PRU_CMD_CTR,pru_send_buf);
}
/************* 读取模块运行数据  **********************************************
 * 名称：             mHD_Pru_Protocol_ReadRunData
 * 功能：             读取模块运行数据,模块返回来的模块采集数据
 * 入口参数：      无
 * return :            错误代码02超时错误，0无错误
 * **************************************************************************/
uint8_t  mHD_Pru_Protocol_ReadRunData(void)
{
   uint16_t i;
   uint16_t name =0;
   uint8_t len;
   uint8_t bitnum;
   uint16_t crc;
   uint8_t crc_L,crc_H;

   int fs_sel;
   fd_set fs_read;
   struct timeval time;

   FD_ZERO(&fs_read);      //指定的文件描述符集清空
   FD_SET(PRU_Rpmsg.Rpmsg_fd,&fs_read);    //用于在文件描述符集合中增加一个新的文件描述符

   time.tv_sec = 0;
   time.tv_usec = 0;
   //使用select实现串口的多路通信
   fs_sel = select(PRU_Rpmsg.Rpmsg_fd+1,&fs_read,NULL,NULL,&time);
   if(fs_sel<=0) return -1;

   PRU_Rpmsg.RxLen = read(PRU_Rpmsg.Rpmsg_fd,PRU_Rpmsg.Rxbuf,512);
   if((PRU_Rpmsg.RxLen>=8)&&(PRU_Rpmsg.RxLen<=256))
   {
       if((PRU_Rpmsg.Rxbuf[1] == PRU_CMD_RWDATA ) && (PRU_Rpmsg.Rxbuf[3] ==  PRU_CMD_SETRETURN_LEN  ))
       {
           mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,4); //打印收到的信息
           PRU_Rpmsg.Rpmsg_Write_EN = 0;
           return 0;
       }

       mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,18);
       mHD_DevData_ReplyAndInit();  //受到数据回复命令
       if(PRU_Rpmsg.Rxbuf[0] >=32) return Err_Data;

       if((PRU_Rpmsg.Rxbuf[3] == PRU_CMD_RWDATA ) && (PRU_Rpmsg.Rxbuf[4] ==  MODULE_DATAREG_ADD   ))
       {
           len = PRU_Rpmsg.Rxbuf[5];
           crc = mHD_RTU_CRC16(PRU_Rpmsg.Rxbuf,len+6);
           crc_L = crc & 0xFF;
           crc_H = crc>>8 & 0xFF;
           if((PRU_Rpmsg.Rxbuf[len+6]==crc_L) && (PRU_Rpmsg.Rxbuf[len+7]==crc_H))
           {
               name = (PRU_Rpmsg.Rxbuf[1]<<8) | PRU_Rpmsg.Rxbuf[2];
               bitnum = PRU_Rpmsg.Rxbuf[0];

               switch (name) {
               case MODULE_7122:
                   if(PRU_Rpmsg.Rxbuf[5] != MODULE7122_RDATA_PDULEN) return Err_Data;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[i] = (PRU_Rpmsg.Rxbuf[6] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[8+i] = (PRU_Rpmsg.Rxbuf[7] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (PRU_Rpmsg.Rxbuf[8] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[24+i] = (PRU_Rpmsg.Rxbuf[9] >> i) & 0x01;

                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (PRU_Rpmsg.Rxbuf[10] >> i) & 0x01;
                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (PRU_Rpmsg.Rxbuf[11] >> i) & 0x01;
                   break;
               case MODULE_8012:
                   if(PRU_Rpmsg.Rxbuf[5] != MODULE8012_RDATA_PDULEN) return Err_Data;
                   for(i=0;i<12;i++) Dev_data.MData[bitnum].AInData[i] =  (PRU_Rpmsg.Rxbuf[6+2*i] << 8) | PRU_Rpmsg.Rxbuf[7+2*i];
                   //for(i=0;i<4;i++) Dev_data.MData[bitnum].AOutData[i] = (PRU_Rpmsg.Rxbuf[30+2*i] << 8) | PRU_Rpmsg.Rxbuf[31+2*i];
                   break;
               case MODULE_7313:
                   if(PRU_Rpmsg.Rxbuf[5] != MODULE7313_RDATA_PDULEN) return Err_Data;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[i] = (PRU_Rpmsg.Rxbuf[6] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[8+i] = (PRU_Rpmsg.Rxbuf[7] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (PRU_Rpmsg.Rxbuf[8] >> i) & 0x01;

                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (PRU_Rpmsg.Rxbuf[9] >> i) & 0x01;
                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (PRU_Rpmsg.Rxbuf[10] >> i) & 0x01;
                   //               for(i=0;i<6;i++) Dev_data.MData[bitnum].EnInDir[i] = (PRU_Rpmsg.Rxbuf[11] >> i) & 0x01;

                   for(i=0;i<6;i++)
                   {
                       Dev_data.MData[bitnum].EnPulseData[i] = PRU_Rpmsg.Rxbuf[12+4*i] <<24 | PRU_Rpmsg.Rxbuf[13+4*i] <<16  |
                                                                                                                        PRU_Rpmsg.Rxbuf[14+4*i] <<8 | PRU_Rpmsg.Rxbuf[15+4*i];
                   }
                   for(i=0;i<6;i++) Dev_data.MData[bitnum].EnSpeedData[i] = PRU_Rpmsg.Rxbuf[36+2*i] <<8 | PRU_Rpmsg.Rxbuf[37+2*i];
                   break;
               case MODULE_7314:
                   if(PRU_Rpmsg.Rxbuf[5] != MODULE7314_RDATA_PDULEN) return Err_Data;
                   for(i=0;i<6;i++)Dev_data.MData[bitnum].DInData[i] = (PRU_Rpmsg.Rxbuf[6] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[6+i] = (PRU_Rpmsg.Rxbuf[7] >> i) & 0x01;
                   for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (PRU_Rpmsg.Rxbuf[8] >> i) & 0x01;

                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (PRU_Rpmsg.Rxbuf[9] >> i) & 0x01;
                   //               for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (PRU_Rpmsg.Rxbuf[10] >> i) & 0x01;

                   //               for(i=0;i<3;i++) Dev_data.MData[bitnum].EnInDir[i] = (PRU_Rpmsg.Rxbuf[11] >> i) & 0x01;
                   //               for(i=3;i<5;i++) Dev_data.MData[bitnum].EnInDir[i] = (PRU_Rpmsg.Rxbuf[11] >> i) & 0x01;

                   Dev_data.MData[bitnum].SSIErr[0] = PRU_Rpmsg.Rxbuf[12] & 0x07;
                   Dev_data.MData[bitnum].SSIErr[1] = (PRU_Rpmsg.Rxbuf[12] >>3) & 0x07;

                   for(i=0;i<3;i++)
                   {
                       Dev_data.MData[bitnum].EnPulseData[i] = PRU_Rpmsg.Rxbuf[13+4*i] <<24 | PRU_Rpmsg.Rxbuf[14+4*i] <<16  |
                                                                                                                        PRU_Rpmsg.Rxbuf[15+4*i] <<8 | PRU_Rpmsg.Rxbuf[16+4*i];
                   }
                   for(i=0;i<2;i++)
                   {
                       Dev_data.MData[bitnum].SSIPulseData[i] = PRU_Rpmsg.Rxbuf[25+4*i] <<24 | PRU_Rpmsg.Rxbuf[26+4*i] <<16  |
                                                                                                                         PRU_Rpmsg.Rxbuf[27+4*i] <<8 | PRU_Rpmsg.Rxbuf[28+4*i];
                   }

                   for(i=0;i<3;i++) Dev_data.MData[bitnum].EnSpeedData[i] = PRU_Rpmsg.Rxbuf[33+2*i] <<8 | PRU_Rpmsg.Rxbuf[34+2*i];
                   for(i=0;i<2;i++) Dev_data.MData[bitnum].SSISpeedData[i] = PRU_Rpmsg.Rxbuf[39+2*i] <<8 | PRU_Rpmsg.Rxbuf[40+2*i];
                   break;
               default:   return Err_Data;
               }
           } else return Err_CRC;
       } else return Err_Data;
   }
   return Err_None;
}
/************* 写入模块数据 **********************************************
 * 名称：             mHD_Pru_Protocol_ReadSetData
 * 功能：             发送IO模块需要写入的数据
 * 入口参数：      mnum模块位号，name模块名称
 * return :            错误代码02超时错误，0无错误
 * **************************************************************************/
int mHD_Pru_Protocol_WriteRunData(uint8_t mnum,uint16_t name)
{
    uint8_t i;
    uint8_t mbit8temp;
    uint16_t crc;
    //uint8_t crc_L,crc_H;
    uint8_t len;
    uint32_t delay;
    uint8_t mtemp =0;

    if(mnum >= 32) return -1;
    PRU_Rpmsg.Txbuf[0] = mnum;  //BitNum
    PRU_Rpmsg.Txbuf[1] = PRU_CMD_RWDATA;  //Function Code

    PRU_Rpmsg.Txbuf[2] = mnum;  //BitNum
    PRU_Rpmsg.Txbuf[5] = PRU_CMD_RWDATA;  //Function Code
    PRU_Rpmsg.Txbuf[6] = MODULE_DATAREG_ADD; //Reg

    switch (name)
    {
        case MODULE_7122:
            len = MODULE7122_WDARA_PDULEN;                  //0x04
            PRU_Rpmsg.Txbuf[3] = (MODULE_7122 >>8)  & 0xFF;
            PRU_Rpmsg.Txbuf[4] = MODULE_7122   & 0xFF;
            PRU_Rpmsg.Txbuf[7] = len;
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[8] =mtemp;  //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[9] =mtemp; //开关量输出8-15通道
            PRU_Rpmsg.Txbuf[10] = PRU_Rpmsg.Txbuf[11] = 0x00;                        //保留位
        break;
        case MODULE_8012:
            len = MODULE8012_WDARA_PDULEN;          //0x0A 10
            PRU_Rpmsg.Txbuf[3] = (MODULE_8012 >>8)  & 0xFF;
            PRU_Rpmsg.Txbuf[4] = MODULE_8012   & 0xFF;
            PRU_Rpmsg.Txbuf[7] = len;
            for(i=0;i<4;i++)
            {
                PRU_Rpmsg.Txbuf[8+2*i] = (Dev_data.MData[mnum].AOutData[i] >> 8)  & 0xFF;     //模拟量输出高8位
                PRU_Rpmsg.Txbuf[9+2*i] = Dev_data.MData[mnum].AOutData[i]  & 0xFF;              //模拟量输出低8位
            }
            PRU_Rpmsg.Txbuf[16] = PRU_Rpmsg.Txbuf[17] = 0x00;                                           //保留位
        break;
        case MODULE_7313:
            len = MODULE7313_WDARA_PDULEN;          //0x23  35
            PRU_Rpmsg.Txbuf[3] = (MODULE_7313 >>8)  & 0xFF;
            PRU_Rpmsg.Txbuf[4] = MODULE_7313   & 0xFF;
            PRU_Rpmsg.Txbuf[7] = len;          //0x23  35
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[8] =mtemp;  //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[9] =mtemp; //开关量输出8-15通道

            mbit8temp =0;
            for(i=0;i<8;i++)  mbit8temp |= (Dev_data.MData[mnum].EnInClear[i] &0x01) << i;
            PRU_Rpmsg.Txbuf[10] = mbit8temp;     //增量编码器清零
            for(i=0;i<8;i++)
            {
                PRU_Rpmsg.Txbuf[11+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 24) & 0xFF;
                PRU_Rpmsg.Txbuf[12+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 16) & 0xFF;
                PRU_Rpmsg.Txbuf[13+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 8) & 0xFF;
                PRU_Rpmsg.Txbuf[14+4*i] = Dev_data.MData[mnum].PulseData[i] & 0xFF;
            }
        break;
        case MODULE_7314:
            len = MODULE7313_WDARA_PDULEN;          //0x23  35
            PRU_Rpmsg.Txbuf[3] = (MODULE_7314 >>8)  & 0xFF;
            PRU_Rpmsg.Txbuf[4] = MODULE_7314   & 0xFF;
            PRU_Rpmsg.Txbuf[7] = len;
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[8] =mtemp;  //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            PRU_Rpmsg.Txbuf[9] =mtemp; //开关量输出8-15通道
            mbit8temp =0;
            for(i=0;i<8;i++)  mbit8temp |= (Dev_data.MData[mnum].EnInClear[i] &0x01) << i;
            PRU_Rpmsg.Txbuf[10] = mbit8temp;     //增量编码器清零
            for(i=0;i<8;i++)
            {
                PRU_Rpmsg.Txbuf[11+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 24) & 0xFF;
                PRU_Rpmsg.Txbuf[12+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 16) & 0xFF;
                PRU_Rpmsg.Txbuf[13+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 8) & 0xFF;
                PRU_Rpmsg.Txbuf[14+4*i] = Dev_data.MData[mnum].PulseData[i] & 0xFF;
            }
        break;
        default:    return -1;   //模块名称编号错误
    }
    crc = mHD_RTU_CRC16(PRU_Rpmsg.Txbuf, len+8); //求校验和
    PRU_Rpmsg.Txbuf[len+8] = crc & 0xFF;
    PRU_Rpmsg.Txbuf[len+9] = (crc >> 8) & 0xFF;

    PRU_Rpmsg.TxLen =len+10; //发送长度
    write(PRU_Rpmsg.Rpmsg_fd, PRU_Rpmsg.Txbuf, PRU_Rpmsg.TxLen);  //写命令到PRU 处理器
    mHD_Debug_Printf(PRU_Rpmsg.Txbuf,PRU_Rpmsg.TxLen,17);  //打印发送数据
    delay =10000; // 500ms 检测返回数据,最大延时500ms
    while(delay--)
    {
        if(PRU_Rpmsg.Rpmsg_Write_EN ==0) return Err_None;
        else
        {
            usleep(10);
            if(delay <= 5) return 0;
        }
    }

//        PRU_Rpmsg.RxLen = read(PRU_Rpmsg.Rpmsg_fd,PRU_Rpmsg.Rxbuf,512);
//        if((PRU_Rpmsg.RxLen>=8)&&(PRU_Rpmsg.RxLen<=256))
//        {
//             mHD_Debug_Printf(PRU_Rpmsg.Rxbuf,PRU_Rpmsg.RxLen,4); //打印收到的信息
//            if(PRU_Rpmsg.Rxbuf[0] != mnum) return Err_Data;
//            else if((PRU_Rpmsg.Rxbuf[1] == PRU_CMD_RWDATA ) && (PRU_Rpmsg.Rxbuf[3] ==  PRU_CMD_SETRETURN_LEN  ))
//            {
//                crc = mHD_RTU_CRC16(PRU_Rpmsg.Rxbuf,8);
//                crc_L = crc & 0xFF;
//                crc_H = crc>>8 & 0xFF;
//                if((PRU_Rpmsg.Rxbuf[8]==crc_L) && (PRU_Rpmsg.Rxbuf[9]==crc_H)) return PRU_Rpmsg.Rxbuf[4];
//                else return Err_CRC;
//            } else return Err_Data;
//        } else
//        {
//            usleep(10);
//            if(delay <= 20000) return 0x0D;
//        }
//    }
     return Err_None;
}
















