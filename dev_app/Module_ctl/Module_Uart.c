/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Module_Uart.c
 *       Description:  设置moduole IO 模块通讯
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

#include    <fcntl.h>           /*文件控制定义*/
#include    <errno.h>         /*错误号定义*/
#include     <termios.h>    /*PPSIX终端控制定义*/
#include     <stdint.h>      /*数据类型宏定义 */

#include "Module_Uart.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"
#include "../PRUSoft_Ctl/PRU_ConfigWrite.h"
#include "../PRUSoft_Ctl/PRU_Protocol.h"

#define Module_UART             "/dev/ttyS0"    //按键LED板连接的 串口2 RS232电平

Module_Uart ModUart_data;
ModUart_Para ModUart_para;

/******************模块串口参数设置**************************************
*名称：            mHD_Module_SetUart_Para
*功能：            串口参数设置
*入口参数：     upara 参数结构体
*出口参数：FALSE -1  错误，正确返回0
*******************************************************************/
int mHD_Module_SetUart_Para(ModUart_Para upara)
{
    int err;
    err = mHD_Uart_Init(ModUart_para.Fd,upara.Baud,upara.Flow_ctrl,upara.Databits,upara.Stopbits,upara.Parity,upara.Wtime,upara.Wlen); //初始化串口
    if(err <0)
    {
        if(HqDev_CmdSys.debug ==1) printf("set Module serial parameter error!\n");
        return -1 ;
    }
    return err;
}

/******************打开模块串口**************************************
*名称：            mHD_Module_Uart_Init()
*功能：           串口初始化
*入口参数：    无
*出口参数：FALSE -1  错误，正确返回串口描述
*******************************************************************/
int mHD_Module_Uart_Open(void)
{
    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug

    fd = mHD_Uart_Open(Module_UART );  //打开串口
    if(fd<0 )
    {
         if(HqDev_CmdSys.debug ==1) printf("Can't Open Module Serial Port");
        return -1;
    }
    return  fd ; //返回文件串口设备文件描述
}

/******************读取模块串口数据**************************************
*名称：            mHD_Module_SetUart_Para
*功能：            读取模块串口数据
*入口参数：     无
*出口参数：FALSE -1  错误，正确返回0
*******************************************************************/
int mHD_Module_ReadUart_Data(void)
{
    return 0;
}

/******************写入模块串口数据**************************************
*名称：            mHD_Module_WriteUart_Data
*功能：            写入模块串口数据
*入口参数：     无
*出口参数：FALSE -1  错误，正确返回写入长度
*******************************************************************/
int mHD_Module_WriteUart_Data(void)
{
    int len;
    len = mHD_Uart_Send(ModUart_para.Fd, (char *)ModUart_data.Txbuf,ModUart_data.TxNum);
    memset(ModUart_data.Txbuf,0x00,ModUart_data.TxNum);  //清空缓存区
    ModUart_data.TxNum =0;  //复位发送计数器

    return len;
}


/******************模块参数设置**************************************
*名称：            mHD_Module_Write_ParaSet
*功能：            写入模块参数设置
*入口参数：     无
*出口参数：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_Write_ParaSet(void)
{
    return 0;
}

/******************模块参数读取 **************************************
*名称：            mHD_Module_Read_ParaSet
*功能：            读取模块参数设置
*入口参数：     无
*出口参数：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_Read_ParaSet(void)
{
    return 0;

}



/******************模块设置参数赋值 **************************************
*名称：            mHD_Module_Read_ParaSet
*功能：            读取模块参数设置
*入口参数：     num 模块位号
*出口参数：FALSE -1  错误，正确返回0
*********************************************************************/
static int mHD_Module_ParaSet_Data(int num)
{
    int i;
    uint16_t crc;
    int len;

    ModUart_data.Txbuf[0] = 0x00;  //位号
    ModUart_data.Txbuf[1] = (Dev_data.Mconfig[num].Name >>8) &0xFF;   //名称高位
    ModUart_data.Txbuf[2] = Dev_data.Mconfig[num].Name  &0xFF;           //名称低位
    ModUart_data.Txbuf[3] = MODULE_CMU_WRITESET; //设置参数功能码
    ModUart_data.Txbuf[4] = 0x00;  //寄存器

    ModUart_data.Txbuf[6]  = ModUart_data.Txbuf[1];
    ModUart_data.Txbuf[7]  = ModUart_data.Txbuf[2];
    ModUart_data.Txbuf[8]  =Dev_data.Mconfig[num].Ver;
    ModUart_data.Txbuf[9]  =Dev_data.Mconfig[num].BitNum;
    ModUart_data.Txbuf[10]  =Dev_data.Pru.MBaud;

    switch (Dev_data.Mconfig[num].Name) {
    case MODULE_7122:
        ModUart_data.Txbuf[5] = MODULE_CMD_SET7122_LEN;  //9
        for(i=0;i<4;i++) ModUart_data.Txbuf[11+i] = Dev_data.Mconfig[num].DInFilterTime[i];
        break;
    case MODULE_8012:
        ModUart_data.Txbuf[5] = MODULE_CMD_SET8012_LEN;  //25
        for(i=0;i<4;i++)
        {
            ModUart_data.Txbuf[11+2*i] = Dev_data.Mconfig[num].AOutVReg[i]; //模拟量输出电压范围设置
            ModUart_data.Txbuf[11+2*i+1] = Dev_data.Mconfig[num].AOutAReg[i]; //模拟量输出电压范围设置
        }
        for(i=0;i<12;i++) ModUart_data.Txbuf[19+i] = Dev_data.Mconfig[num].AInReg[i]; //模拟量输入范围设置
        break;
    case MODULE_7313:
        ModUart_data.Txbuf[5] = MODULE_CMD_SET7313_LEN;  //38
        for(i=0;i<8;i++) ModUart_data.Txbuf[11+i]  = Dev_data.Mconfig[num].DOutFunSet[i];    //DO输出功能设置
        for(i=0;i<12;i++) ModUart_data.Txbuf[19+i] = Dev_data.Mconfig[num].DInFunSet[i];     //DI输入功能设置
        ModUart_data.Txbuf[31] =  Dev_data.Mconfig[num].DInFilterTime[0];  //普通DI输入通道滤波时间设置
        for(i=0;i<6;i++)
        {
            ModUart_data.Txbuf[32+2*i] = (Dev_data.Mconfig[num].EnSCPulseSet[i] >>8) & 0xFF;
            ModUart_data.Txbuf[33+2*i] =Dev_data.Mconfig[num].EnSCPulseSet[i] & 0xFF;
        }
        break;
    case MODULE_7314:
        ModUart_data.Txbuf[5] = MODULE_CMD_SET7314_LEN;  //28
        for(i=0;i<8;i++) ModUart_data.Txbuf[11+i] = Dev_data.Mconfig[num].DOutFunSet[i];//DO输出功能设置
        for(i=0;i<6;i++) ModUart_data.Txbuf[19+i] = Dev_data.Mconfig[num].DInFunSet[i];      //DI输入功能设置
        for(i=0;i<2;i++) ModUart_data.Txbuf[25+i] = Dev_data.Mconfig[num].SSIFunSet[i];     //SSI输入功能设置
        ModUart_data.Txbuf[27] =  Dev_data.Mconfig[i].DInFilterTime[0];                               //普通DI输入通道滤波时间设置
        for(i=0;i<3;i++)
        {
            ModUart_data.Txbuf[28+2*i] = (Dev_data.Mconfig[num].EnSCPulseSet[i] >>8) & 0xFF;
            ModUart_data.Txbuf[29+2*i] = Dev_data.Mconfig[num].EnSCPulseSet[i] & 0xFF;
        }
        break;
    default:
        if(HqDev_CmdSys.debug ==1) printf("Read Module Set File Error!\n");
        return -1;
    }

    crc = mHD_RTU_CRC16(ModUart_data.Txbuf,ModUart_data.Txbuf[5]+6);
    ModUart_data.Txbuf[ModUart_data.Txbuf[5]+6] = crc & 0xFF;
    ModUart_data.Txbuf[ModUart_data.Txbuf[5]+7] = crc>>8 & 0xFF;
    ModUart_data.TxNum =ModUart_data.Txbuf[5]+8;

    /*************  打印设置调试信息 ********************/
    if(HqDev_CmdSys.debug ==1) {
        printf("MPUMode Uart Send Num %d Byte data = ",ModUart_data.TxNum);
        for(i=0;i<ModUart_data.TxNum;i++) {
            if(i !=ModUart_data.TxNum -1) printf("%02X ",ModUart_data.Txbuf[i]);
            else printf("%02X\n",ModUart_data.Txbuf[i]);
        }
    }
    /************* End Printf ************************/
    len = mHD_Module_WriteUart_Data();   //串口发送参数设置信息
    return len;
}

/****************** 读取配置文件赋值到参数设置变量**************************************
*名称：            mHD_Module_Read_SetFileTOPar
*功能：            读取配置文件赋值到参数变量组 Dev_Data.Mconfig
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_Read_SetFileTOPar(void)
{
    uint8_t  modnum =0;
    int i,j;
    modnum = mHD_MPU_Read_SetFile();     //读取设置文件
    if(modnum !=0)
    {
        Dev_data.Pru.RBaud =  M_SetSysData.Devbaud;        //PRU处理器波特率
        Dev_data.Pru.MBaud = M_SetSysData.Modbaud;       //Module 波特率
        Dev_data.Pru.MEndBit = modnum;                              //模块开始位号
        for(i=1;i<modnum+1;i++)
        {
            Dev_data.Mconfig[i].Baud = M_SetSysData.Modbaud;       //Module 波特率
            Dev_data.Mconfig[i].BitNum = M_SetData[i].bitnum;
            Dev_data.Mconfig[i].Name = M_SetData[i].name;
            Dev_data.Mconfig[i].Ver = M_SetData[i].ver;
            if(M_SetData[i].name ==MODULE_7122)
            {
                for(j=0;j<4;j++) Dev_data.Mconfig[i].DInFilterTime[j] = M_SetData[i].setdata[j];
            }
            else if(M_SetData[i].name ==MODULE_8012)
            {
                for(j=0;j<4;j++)
                {
                    Dev_data.Mconfig[i].AOutVReg[j] = M_SetData[i].setdata[j*2];
                    Dev_data.Mconfig[i].AOutAReg[j] = M_SetData[i].setdata[j*2+1];
                }
                for(j=0;j<12;j++) Dev_data.Mconfig[i].AInReg[j] = M_SetData[i].setdata[8+j];
            }
            else if(M_SetData[i].name ==MODULE_7313)
            {
                for(j=0;j<8;j++)  Dev_data.Mconfig[i].DOutFunSet[j] =  M_SetData[i].setdata[j];
                for(j=0;j<12;j++)  Dev_data.Mconfig[i].DInFunSet[j] =  M_SetData[i].setdata[8+j];
                Dev_data.Mconfig[i].DInFilterTime[0] = M_SetData[i].setdata[20];
                for(j=0;j<6;j++)  Dev_data.Mconfig[i].EnSCPulseSet[j] =  M_SetData[i].esturn[j];
            }
            else if(M_SetData[i].name ==MODULE_7314)
            {
                for(j=0;j<8;j++)  Dev_data.Mconfig[i].DOutFunSet[j] =  M_SetData[i].setdata[j];
                for(j=0;j<6;j++)  Dev_data.Mconfig[i].DInFunSet[j] =  M_SetData[i].setdata[8+j];
                for(j=0;j<2;j++)  Dev_data.Mconfig[i].SSIFunSet[j] = M_SetData[i].setdata[14+j];
                Dev_data.Mconfig[i].DInFilterTime[0] = M_SetData[i].setdata[16];
                for(j=0;j<3;j++)  Dev_data.Mconfig[i].EnSCPulseSet[j] =  M_SetData[i].esturn[j];
            }
            else return -1;
        }
    }
    return 0;
}
/****************** 初始化模块 **************************************
*名称：            mHD_Module_Read_SetFileTOPar
*功能：            读取配置文件赋值到参数变量组 Dev_Data.Mconfig
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
void mHD_Module_SetGPIO_Pluse(int gpio)
{
    int len;
    int i;

    mHD_Request_GPIO(gpio);                         //申请GPIO
    mHD_SetGPIO_Direction(gpio,"out");           //设置GPIO方向为输出
    mHD_SetGPIO_Value(gpio,0);    //设置GIPO状态 0
    mHD_msleep(2);
    len =3;
    for(i=0;i<len;i++)
    {
        mHD_SetGPIO_Value(gpio,1);    //设置GIPO状态 1
        usleep(900);
        mHD_SetGPIO_Value(gpio,0);    //设置GIPO状态 0
        usleep(900);
    }
    mHD_Uninstall_GPIO(gpio);                           //卸载GPIO
}
/****************** uart 收发控制 **************************************
*名称：            mHD_Module_Read_SetFileTOPar
*功能：            读取配置文件赋值到参数变量组 Dev_Data.Mconfig
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
void mHD_Set_RTXEN(int value)
{
    mHD_SetGPIO_Value(MODULE_RTXEN,value);    //设置GIPO状态
}

int mHD_Module_Read(char *rxbuf,int len)
{
    return mHD_Uart_Recv(ModUart_para.Fd, rxbuf,len);
}

/****************** MPU Uart 串口写入模块运行数据命令 ********************
*名称：            mHD_Module_WriteRunData(uint8_t bitnum)
*功能：            写入模块运行数据命令
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_WriteRunData(uint8_t mnum)
{
    int i;
    int len;
    uint8_t txbuf[128];
    uint8_t rxbuf[128];
    int rxlen;
    int txlen;
    int err;
    int recv_len;

    switch (Dev_data.Mconfig[mnum].Name) {
    case 0x8012: recv_len = 42; break;
    case 0x7314: recv_len = 45; break;
    case 0x7122: recv_len = 16; break;
    case 0x7313: recv_len = 50; break;
    default: return -1; break;
    }

    if(Dev_data.MData[mnum].ErrCode>5) return -1;   //错误最大数量为联系5次
    tcflush(ModUart_para.Fd, TCIOFLUSH);   //清空输入输出缓存

    //获取模块输出数据
    len = mHD_Pru_Protocol_WriteRunData(mnum,txbuf);
    if(len< 6) { //最少发送6个byte
        Dev_data.MData[mnum].ErrCode++; return -1;
        printf("Tx Data err\n");
    }
    memcpy(ModUart_data.Txbuf,txbuf,len);  //接收到的数据转移到TX发送区
    ModUart_data.TxNum = len;
    //串口发送数据
    txlen = mHD_Module_WriteUart_Data();     //串口发送数据;
    if(txlen !=len) {
        printf("Tx Send err\n");
        Dev_data.MData[mnum].ErrCode++ ; //写入数据错误
        return -1;
    }

    //串口接收数据
    for(i=0;i<50000;i++)
    {
        /*** 读取指定长度的数据 ***********/
        rxlen = m_Uart_Safe_Recv_FixLen(ModUart_para.Fd,rxbuf,recv_len);  //串口读取数据
        ModUart_data.RxNum = rxlen;
        if(rxlen>0)  break;
    }
    if(rxlen < 8) {
        Dev_data.MData[mnum].ErrCode++; //写入数据错误
        return -1;
    }

    err = mHD_Pru_Protocol_ReadRunData(mnum,rxbuf); //接收到的数据赋值
    if(err <0) {
        Dev_data.MData[mnum].ErrCode++; //写入数据错误
        return -1;
    }

    return 0;
}

/****************** MPU Uart 串口读取模块参数设置命令 ********************
*名称：            mHD_Module_ReadParaData
*功能：            读取模块参数设置命令
*读取模块数据格式   B:0   B:1   B:2      B:1    B:2     B:3     B:4 -B:7  B:8   B:9
*                 位号     名称H  名称L  功能码   寄存器    数据数量    无功能       CRCL  CRCH
*模块返回数据格式   B:0   B:1   B:2       B:1    B:2     B:3     B:4 -B:X  B:8   B:9
*                  位号     功能码   寄存器    数据数量    数据       CRCL  CRCH
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_ReadParaData(uint8_t bitnum)
{
    uint8_t i;
    uint16_t crc16;
    int len;

    ModUart_data.Txbuf[0] = bitnum;
    ModUart_data.Txbuf[1] = 0x00;
    ModUart_data.Txbuf[2] = 0x00;
    ModUart_data.Txbuf[3] = MODULE_CMD_READSET;
    ModUart_data.Txbuf[4] = MODULE_SETREG_ADD;
    ModUart_data.Txbuf[5] = MODULE_SETREG_DATALEN;

    for(i=4;i<8;i++) ModUart_data.Txbuf[6+i] = MODULE_SETREG_NCDATA;

    crc16 = mHD_RTU_CRC16(ModUart_data.Txbuf, 10); //求校验和
    ModUart_data.Txbuf[10] = crc16 & 0xFF;
    ModUart_data.Txbuf[11] = (crc16 >> 8) & 0xFF;
    ModUart_data.TxNum = 12;

    len = mHD_Module_WriteUart_Data();   //串口发送数据;
    return len;
}

/****************** MPU Uart 串口连接设置参数返回值检测 ****************
*名称：            mHD_MPUModule_SetPara_Back(void)
*功能：            参数设置返回读取及检测
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_MPUModule_ReadPara_Back(int num)
{
    int rxlen;
    int err;
    uint8_t buf[128];
    int i;

    for(i=0;i<50000;i++)
    {

        rxlen = mHD_Module_Read((char *)buf,128);
        if(rxlen>0)  break;
    }
    if(rxlen <=0) return -1;   //接收错误返回
    else   //读取到有效数据
    {
        err = mHD_Pru_Protocol_ReadSetDataAnalysis(buf, rxlen);
        if(err<0) return -1;
        else printf("Module Read Parameter Module %d Complete !\n",num);
    }
    return 0;
}

/****************** MPU Uart 串口连接设置参数返回值检测 ************************
*名称：            mHD_MPUModule_SetPara_Back(void)
*功能：            参数设置返回读取及检测
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_MPUModule_SetPara_Back(void)
{
    int rxlen;
    int y,i;
    uint8_t buf[128];


    for(i=0;i<50000;i++)
    {
        rxlen = m_Uart_Safe_Recv_FixLen(ModUart_para.Fd,buf,12);  //串口读取数据
        if(rxlen>0)  break;
    }
    if(rxlen>0)  //读取到有效数据
    {
        if(rxlen != MODULE_SETPARA_BACK_LEN)  return -1;
        if((buf[0] !=0) ||(buf[3] !=0x15) ||(buf[4] !=0) ||(buf[5] !=4) ||(buf[6] !=0)) return -1;

        printf("module rx num %d = ",rxlen);
        for(y=0;y<rxlen;y++)
        {
            if(y !=rxlen-1) printf("%02X ",buf[y]);
            else printf("%02X\n",buf[y]);
        }
        return 0;
    } else return -1;
}



/****************** MPUModuelUart 串口连接 读取设置参数 ************************
*名称：            mHD_MPUModule_SetPara(void)
*功能：            MPU Uart 连接IO 模块参数设置
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_MPUModule_ReadPara(void)
{
    int i,y;
    int err;
    int rerr;
    for(i=1;i<Dev_data.Pru.MEndBit+1;i++)
    {
        /**** 发送读取命令 *****/
        printf("Read Module Para %d !\n",i);
        for(y=0;y<3;y++)
        {
            err = mHD_Module_ReadParaData(i);  //写入设置参数命令
            if(err < 0)  {
                Dev_data.Mconfig[i].MRPara = 1;   //写命令错误
                rerr = -1;
                mHD_msleep(50);    //延时50ms 重新读取
            } else {
                err = mHD_MPUModule_ReadPara_Back(i); //读取并解析设置参数
                if(err <0) {
                    Dev_data.Mconfig[i].MRPara = 1;   //写命令正确
                    rerr = -1;
                    mHD_msleep(50);    //延时50ms 重新读取
                } else {
                    Dev_data.Mconfig[i].MRPara = 0;   //写命令正确
                    rerr =0;
                    break;   //退出本次循环
                }
            }
        }
        mHD_msleep(500);
    }
    return rerr;
}


/****************** MPUModuelUart 串口连接设置参数 ************************
*名称：            mHD_MPUModule_SetPara(void)
*功能：            MPU Uart 连接IO 模块参数设置
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_MPUModule_SetPara(void)
{
    int setparten = 0;  //设置使能
    int i;
    int err = 0;
    int rxlen;
    int err_send_cnt = 0;  //写入参数错误无次数
    int err_read_cnt =0;   //读取参数错误次数
    int setmode = 0;        //设置参数模式开关
    int rdataen = -1;
    int radrun;

    radrun =  mHD_Read_Run_Data();       //读取二进制配置文件
    if(radrun ==0)  //读参数成功 赋值变量
    {
        Dev_data.Pru.MEndBit = HqRpuRun_data.MEndBit;  //模块数量
        Dev_data.Pru.MRSetStatus =1; //参数设置成功成功
        Dev_data.Pru.RBaud = HqRpuRun_data.Devbaud;         //PRU处理器波特率
        Dev_data.Pru.MBaud = HqRpuRun_data.Modbaud;     //Module 波特率
        Run_data.ModuleNum = HqRpuRun_data.MEndBit;  //读取结束模块
        rdataen =1 ; //读取运行配置成功
    }

    if(mHD_Read_GPIO(BUTRUN_RPU_1) ==0)  //RUNSET_1 开关 配置文件设置参数
    {
        printf("Module SetPara Configuration File\n");
        setparten =1;
        err =  mHD_Module_Read_SetFileTOPar();   //读取设置文件到设置参数组
        if(err <0 ) return -1;
    }
    else  if((mHD_Read_GPIO(BUTRUN_MPU_2) ==0) ||(HqDev_CmdSys.setmodule ==1) //RUNSET_2 开关或者运行配置文件参数设置模块
                ||(rdataen !=1))
    {
        printf("Module SetPara Setting Parameters\n");
        setparten =1;
        mHD_Module_ConfigData_Init();
    }

    if(setparten ==1) {
        for(i=1;i<Dev_data.Pru.MEndBit+1;i++) {
            if(setmode ==0) {
                mHD_Module_SetGPIO_Pluse(MODULE_SETEN);  //设置模块为参数配置模式
                mHD_msleep(500);  //延时需要 > 500ms
                setmode =1;
            }

            printf("Set Module %d !\n",i);

            err =  mHD_Module_ParaSet_Data(i);  //赋值设置参数
            if(err <0)  {
                printf("Module %d Parameter Set Failure !\n",i);  err_send_cnt++;
                if(err_send_cnt<4)  {
                    printf("Module %d Parameter Set Failure Reset %d!\n",i,err_send_cnt);
                    i =0; setmode =0; mHD_msleep(3000);  //延时3S 等待模块复位
                    continue;  //跳出本次循环 重新开始设置
                } else  {
                     printf("Module %d Parameter Set Failure !\n",i);
                     err = -1;
                     break;  //跳出整个循环
                }
            } else err =0;

            //读取并判断返回参数
            rxlen = mHD_MPUModule_SetPara_Back();
            if(rxlen <0)  { //接送返回数据错误
                printf("Module %d Parameter Read Failure !\n",i);  err_read_cnt++;
                if(err_read_cnt<4)  {
                    printf("Module %d Parameter Read Failure Reset %d!\n",i,err_read_cnt);
                    i =0; setmode =0; mHD_msleep(3000);  //延时3S 等待模块复位
                    continue;  //跳出本次循环 重新开始设置
                } else  {
                     printf("Module %d Parameter Read Failure !\n",i);
                     err = -1;
                     break;  //跳出整个循环
                }
            } else err = 0;

            mHD_msleep(1200);  //延时需要 >1100ms
        }
    }
    return err;
}

/****** 模块数据读写扫描线程函数 ***************/
void mHD_Module_RWData_Thread(void)
{
    int pollcnt =1;
    int err;
    while(1)
    {
        err = mHD_Module_WriteRunData(pollcnt++);
        if(err <0)
        {
            if(HqDev_CmdSys.debug ==1) printf("Module Poll RWData Erro !\n");
        }
        if(pollcnt>4) pollcnt=1;
    }
}
/****** 模块数据读写线程创建函数 ***************/
int mHD_Module_RWData_CreatThread(void)
{
    int ret;

    ret = pthread_create(&ModUart_data.Thread_RW_ID,NULL,(void *) mHD_Module_RWData_Thread,NULL);
    if(ret !=0) {
        return -1;
    } else return 0;
}

/****************** 初始化模块 **************************************
*名称：            mHD_Module_Read_SetFileTOPar
*功能：            读取配置文件赋值到参数变量组 Dev_Data.Mconfig
*入口参数：     无
*出口参数：     无
* Return：FALSE -1  错误，正确返回0
*********************************************************************/
int mHD_Module_Init(uint32_t baud)
{
    int err;
    int fd;

    ModUart_para.Baud = baud;
    ModUart_para.Flow_ctrl =0;
    ModUart_para.Databits =8;
    ModUart_para.Stopbits =1;
    ModUart_para.Parity = 'N';
    ModUart_para.Wtime =0;
    ModUart_para.Wlen =1;

    fd = mHD_Uart_Open(Module_UART );  //打开串口
    if(fd <0)  return -1;
    ModUart_para.Fd =fd;
    err = mHD_Module_SetUart_Para(ModUart_para);  //设置串口参数
     if(err <0)  return -1;

    err = mHD_MPUModule_SetPara();  //模块参数设置
    if(err <0)  {//设置失败
        Dev_data.Pru.MWSetStatus =0; //参数设置失败
        printf("Module Parameter setting failed !\n");
        return -1;
    } else { //设置成功
        Dev_data.Pru.MWSetStatus =1; //参数设置成功
        HqRpuRun_data.setmodule = 0;  //不设置参数
        HqRpuRun_data.setstatus = 1;     //已完成设置
        HqRpuRun_data.Devbaud = Dev_data.Pru.RBaud;  //MPU 波特率
        HqRpuRun_data.Modbaud = Dev_data.Pru.MBaud;  //模块波特率
        HqRpuRun_data.MStartBit =1;  //永远从1开始扫描模块
        HqRpuRun_data.MEndBit = Dev_data.Pru.MEndBit ;   //连接模块数量
        mHD_Write_Run_Data() ;   //写入配置文件
        printf("Module Parameter Set Complete !\n");
    }

    //读取设置参数 必须读取,读取不成功,不能轮询模块
    err = mHD_MPUModule_ReadPara();
    if(err <0)
    {
        Dev_data.Pru.MRSetStatus =0;  //参数读取失败
        printf("Module Read Parameter failed !\n");
        return -1;
    }else {
        Dev_data.Pru.MRSetStatus =1;  //参数读取成功
        printf("Module Read Parameter Complete!\n");
    }

   //加载线程模块自动扫描运行
   mHD_Module_RWData_CreatThread();

   if(Dev_data.Pru.MWSetStatus ==1) HqTopLED_Data.fun = mHD_Bit8Set(HqTopLED_Data.fun,4,1);   //f.4  参数配置完成指示
   if(Dev_data.Pru.MRSetStatus ==1) HqTopLED_Data.fun = mHD_Bit8Set(HqTopLED_Data.fun,5,1);   //f.5  参数读取完成指示


    return 0;
}

