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
 * return :            错误-1,正确0;
 * **************************************************************************/
int  mHD_Pru_Protocol_ReadSetDataAnalysis(uint8_t *buf,int rxlen)
 {
    uint16_t i;
    uint16_t name =0;
    uint16_t crc;
    int bitnum;
    uint8_t crc_L,crc_H;
    int len;

    /*************  打印设置调试信息 ********************/
        printf("MPUMode Uart Read Num %d Byte data = ",rxlen);
        for(i=0;i<rxlen;i++) {
            if(i !=rxlen -1) printf("%02X ",buf[i]);
            else printf("%02X\n",buf[i]);
        }
    /************* End Printf ************************/

    if((buf[3] != PRU_CMD_READSET ) ||  (buf[4] !=  MODULE_SETREG_ADD )) {
        return -1;
    }

    len = buf[5];
    crc = mHD_RTU_CRC16(buf,len+6);
    crc_L = crc & 0xFF;
    crc_H = crc>>8 & 0xFF;
    if((buf[len+6] !=crc_L) || (buf[len+7] !=crc_H)) {
        return -1;
    }

    name = (buf[6]<<8) | buf[7];
    bitnum = buf[9];
    switch (name) {
    case MODULE_7122:
        if(buf[5] != MODULE7122_RPARA_PDULEN) return -1;
        Dev_data.Mconfig[bitnum].Name = name;
        Dev_data.Mconfig[bitnum].Ver = buf[8];
        Dev_data.Mconfig[bitnum].BitNum = buf[9];
        Dev_data.Mconfig[bitnum].Baud = buf[10];
        for(i=0;i<4;i++) Dev_data.Mconfig[bitnum].DInFilterTime[i] = buf[11+i];
        break;
    case MODULE_8012:
        if(buf[5] != MODULE8012_RPARA_PDULEN) return -1;
        Dev_data.Mconfig[bitnum].Name = name;
        Dev_data.Mconfig[bitnum].Ver = buf[8];
        Dev_data.Mconfig[bitnum].BitNum = buf[9];
        Dev_data.Mconfig[bitnum].Baud = buf[10];
        for(i=0;i<4;i++)
        {
            Dev_data.Mconfig[bitnum].AOutVReg[i] = buf[11+2*i];
            Dev_data.Mconfig[bitnum].AOutAReg[i] = buf[12+2*i];
        }
        for(i=0;i<12;i++) Dev_data.Mconfig[bitnum].AInReg[i] = buf[19+i];
        break;
    case MODULE_7313:
        if(buf[5] != MODULE7313_RPARA_PDULEN) return -1;
        Dev_data.Mconfig[bitnum].Name = name;
        Dev_data.Mconfig[bitnum].Ver = buf[8];
        Dev_data.Mconfig[bitnum].BitNum = buf[9];
        Dev_data.Mconfig[bitnum].Baud = buf[10];
        for(i=0;i<8;i++) Dev_data.Mconfig[bitnum].DOutFunSet[i] = buf[11+i];
        for(i=0;i<12;i++) Dev_data.Mconfig[bitnum].DInFunSet[i] = buf[19+i];
        Dev_data.Mconfig[bitnum].DInFilterTime[0] = buf[31];
        for(i=0;i<6;i++) Dev_data.Mconfig[bitnum].EnSCPulseSet[i] = (buf[32+2*i]<<8) | buf[33+2*i];
        break;
    case MODULE_7314:
        if(buf[5] != MODULE7314_RPARA_PDULEN) return -1;
        Dev_data.Mconfig[bitnum].Name = name;
        Dev_data.Mconfig[bitnum].Ver = buf[8];
        Dev_data.Mconfig[bitnum].BitNum = buf[9];
        Dev_data.Mconfig[bitnum].Baud = buf[10];
        for(i=0;i<8;i++) Dev_data.Mconfig[bitnum].DOutFunSet[i] = buf[11+i];
        for(i=0;i<6;i++) Dev_data.Mconfig[bitnum].DInFunSet[i] = buf[19+i];
        for(i=0;i<2;i++) Dev_data.Mconfig[bitnum].SSIFunSet[i] = buf[25+i];
        Dev_data.Mconfig[bitnum].DInFilterTime[0] = buf[27];
        for(i=0;i<3;i++) Dev_data.Mconfig[bitnum].EnSCPulseSet[i] = (buf[28+2*i]<<8) | buf[29+2*i];
        break;
    default:   return -1;
    }
    return 0;
}

/****** 应答命令 无回复******/
//static void  mHD_DevData_ReplyAndInit(void)
//{
//   uint8_t  pru_send_buf[10];
//   pru_send_buf[0] = PUR_CMD_REPLY;  //建立与PRU处理器的连接
//   pru_send_buf[1] = 0x00;  //保留
//   pru_send_buf[2] = 0x00;        //PRU处理器波特率
//   pru_send_buf[3] = 0x00;       //Module 波特率
//   mHD_Pru_Protocol_ReplyInit(PRU_CMD_CTR,pru_send_buf);
//}
/************* 读取模块运行数据  **********************************************
 * 名称：             mHD_Pru_Protocol_ReadRunData
 * 功能：             读取模块运行数据,模块返回来的模块采集数据
 * 入口参数：      mnum 模块位号，buf 输入的数据
 * return :            -1错误，0无错误
 * **************************************************************************/
int mHD_Pru_Protocol_ReadRunData(int mnum,uint8_t *buf)
{
   uint16_t i;
   uint16_t name =0;
   uint8_t len;
   uint8_t bitnum;
   uint16_t crc;
   uint8_t crc_L,crc_H;

   if((buf[0] !=Dev_data.Mconfig[mnum].BitNum) || (buf[3] !=PRU_CMD_RWDATA) ||(buf[4] !=  MODULE_DATAREG_ADD))
   {
       return -1;
   }
   len = buf[5];
   crc = mHD_RTU_CRC16(buf,len+6);
   crc_L = crc & 0xFF;
   crc_H = crc>>8 & 0xFF;
   if((buf[len+6] !=crc_L) || (buf[len+7] !=crc_H))  return -1;
   name = (buf[1]<<8) | buf[2];
   bitnum = buf[0];

   switch (name) {
   case MODULE_7122:
       if(buf[5] != MODULE7122_RDATA_PDULEN) return -1;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[i] = (buf[6] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[8+i] = (buf[7] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (buf[8] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[24+i] = (buf[9] >> i) & 0x01;
       /***** 返回 输出变量赋值 *****************/
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (buf[10] >> i) & 0x01;
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (buf[11] >> i) & 0x01;
       break;
   case MODULE_8012:
       if(buf[5] != MODULE8012_RDATA_PDULEN) return -1;
       for(i=0;i<12;i++) Dev_data.MData[bitnum].AInData[i] =  (buf[6+2*i] << 8) | buf[7+2*i];
       /***** 返回 输出变量赋值 *****************/
       //for(i=0;i<4;i++) Dev_data.MData[bitnum].AOutData[i] = (buf[30+2*i] << 8) | buf[31+2*i];
       break;
   case MODULE_7313:
       if(buf[5] != MODULE7313_RDATA_PDULEN) return -1;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[i] = (buf[6] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[8+i] = (buf[7] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (buf[8] >> i) & 0x01;
       /***** 返回 输出变量赋值 *****************/
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (buf[9] >> i) & 0x01;
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (buf[10] >> i) & 0x01;
       for(i=0;i<6;i++) Dev_data.MData[bitnum].EnInDir[i] = (buf[11] >> i) & 0x01;
       for(i=0;i<6;i++)
       {
           Dev_data.MData[bitnum].EnPulseData[i] = buf[12+4*i] <<24 | buf[13+4*i] <<16  |
                                                                                                            buf[14+4*i] <<8 | buf[15+4*i];
       }
       for(i=0;i<6;i++) Dev_data.MData[bitnum].EnSpeedData[i] = buf[36+2*i] <<8 | buf[37+2*i];
       break;
   case MODULE_7314:
       if(buf[5] != MODULE7314_RDATA_PDULEN) return -1;
       for(i=0;i<6;i++)Dev_data.MData[bitnum].DInData[i] = (buf[6] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[6+i] = (buf[7] >> i) & 0x01;
       for(i=0;i<8;i++)Dev_data.MData[bitnum].DInData[16+i] = (buf[8] >> i) & 0x01;
       /***** 返回 输出变量赋值 *****************/
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[i] = (buf[9] >> i) & 0x01;
       //for(i=0;i<8;i++)Dev_data.MData[bitnum].DOutData[8+i] = (buf[10] >> i) & 0x01;
       for(i=0;i<3;i++) Dev_data.MData[bitnum].EnInDir[i] = (buf[11] >> i) & 0x01;
       for(i=3;i<5;i++) Dev_data.MData[bitnum].EnInDir[i] = (buf[11] >> i) & 0x01;
       Dev_data.MData[bitnum].SSIErr[0] = buf[12] & 0x07;
       Dev_data.MData[bitnum].SSIErr[1] = (buf[12] >>3) & 0x07;

       for(i=0;i<3;i++)
       {
           Dev_data.MData[bitnum].EnPulseData[i] = buf[13+4*i] <<24 | buf[14+4*i] <<16  |
                                                                                                            buf[15+4*i] <<8 | buf[16+4*i];
       }
       for(i=0;i<2;i++)
       {
           Dev_data.MData[bitnum].SSIPulseData[i] = buf[25+4*i] <<24 | buf[26+4*i] <<16  |
                                                                                                             buf[27+4*i] <<8 | buf[28+4*i];
       }
       for(i=0;i<3;i++) Dev_data.MData[bitnum].EnSpeedData[i] = buf[33+2*i] <<8 | buf[34+2*i];
       for(i=0;i<2;i++) Dev_data.MData[bitnum].SSISpeedData[i] = buf[39+2*i] <<8 | buf[40+2*i];
       break;
   default:   return -1;
   }
   return 0;
}

/****************** 获取模块运行数据命令 ********************
*名称：            mHD_Pru_Protocol_WriteRunData
*功能：            获取模块运行数据命令
*读取模块数据格式   B:0          B:1   B:2       B:3          B:4           B:5            B:6 -B:X                   B:X+1   B:X+2
*                                位号     名称H  名称L  功能码   寄存器    数据数量       写入数据                CRCL    CRCH
*模块返回数据格式   B:0          B:1   B:2       B:3          B:4           B:5            B:6 -B:X                    B:X+1   B:X+2
*                                位号     名称H  名称L  功能码   寄存器    数据数量       返回模块数据         CRCL    CRCH
*入口参数：     mnum 模块位号
*出口参数：     buf 写出数据
* Return：FALSE -1  错误，正确返回需要写出的数据数量
*********************************************************************/
int mHD_Pru_Protocol_WriteRunData(uint8_t mnum,uint8_t *buf)
{
    uint8_t i;
    uint8_t mbit8temp;
    uint16_t crc;
    //uint8_t crc_L,crc_H;
    uint8_t len;
    uint8_t mtemp =0;
    uint16_t name;

    if(mnum >= 32) return -1;

    buf[0] = Dev_data.Mconfig[mnum].BitNum;  //BitNum
    buf[3] = PRU_CMD_RWDATA;  //Function Code
    buf[4] = MODULE_DATAREG_ADD; //Reg
    name = Dev_data.Mconfig[mnum].Name;

    switch (name)
    {
        case MODULE_7122:
            len = MODULE7122_WDARA_PDULEN;                  //0x04
            buf[1] = (MODULE_7122 >>8)  & 0xFF;
            buf[2] = MODULE_7122   & 0xFF;
            buf[5] = len;
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            buf[6] =mtemp;  //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            buf[7] =mtemp;                  //开关量输出8-15通道
            buf[8] = buf[9] = 0x00;       //保留位
        break;
        case MODULE_8012:
            len = MODULE8012_WDARA_PDULEN;          //0x0A 10
            buf[1] = (MODULE_8012 >>8)  & 0xFF;
            buf[2] = MODULE_8012   & 0xFF;
            buf[5] = len;
            for(i=0;i<4;i++)
            {
                buf[6+2*i] = (Dev_data.MData[mnum].AOutData[i] >> 8)  & 0xFF;     //模拟量输出高8位
                buf[7+2*i] = Dev_data.MData[mnum].AOutData[i]  & 0xFF;              //模拟量输出低8位
            }
            buf[14] = buf[15] = 0x00;                                           //保留位
        break;
        case MODULE_7313:
            len = MODULE7313_WDARA_PDULEN;          //0x23  35
            buf[1] = (MODULE_7313 >>8)  & 0xFF;
            buf[2] = MODULE_7313   & 0xFF;
            buf[5] = len;          //0x23  35
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            buf[6] =mtemp;  //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            buf[7] =mtemp; //开关量输出8-15通道

            mbit8temp =0;
            for(i=0;i<8;i++)  mbit8temp |= (Dev_data.MData[mnum].EnInClear[i] &0x01) << i;
            buf[8] = mbit8temp;     //增量编码器清零
            for(i=0;i<8;i++)
            {
                buf[9+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 24) & 0xFF;
                buf[10+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 16) & 0xFF;
                buf[11+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 8) & 0xFF;
                buf[12+4*i] = Dev_data.MData[mnum].PulseData[i] & 0xFF;
            }
        break;
        case MODULE_7314:
            len = MODULE7313_WDARA_PDULEN;          //0x23  35
            buf[1] = (MODULE_7314 >>8)  & 0xFF;
            buf[2] = MODULE_7314   & 0xFF;
            buf[5] = len;
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[i] & 0x01) << i;
            buf[6] =mtemp;     //开关量输出0-7通道
            mtemp =0;
            for(i=0;i<8;i++) mtemp |= (Dev_data.MData[mnum].DOutData[8+i] & 0x01) << i;
            buf[7] =mtemp;     //开关量输出8-15通道
            mbit8temp =0;
            for(i=0;i<8;i++)  mbit8temp |= (Dev_data.MData[mnum].EnInClear[i] &0x01) << i;
            buf[8] = mbit8temp;  //增量编码器清零
            for(i=0;i<8;i++)
            {
                buf[9+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 24) & 0xFF;
                buf[10+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 16) & 0xFF;
                buf[11+4*i] = (Dev_data.MData[mnum].PulseData[i] >> 8) & 0xFF;
                buf[12+4*i] = Dev_data.MData[mnum].PulseData[i] & 0xFF;
            }
        break;
        default:    return -1;   //模块名称编号错误
    }
    crc = mHD_RTU_CRC16(buf, len+6); //求校验和
    buf[len+6] = crc & 0xFF;
    buf[len+7] = (crc >> 8) & 0xFF;

     return len+8; //发送长度;
}
















