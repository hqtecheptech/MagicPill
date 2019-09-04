/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Module_GlobalData.c
 *       Description: 外设模块全局数据处理
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 22:18 PM"
 ****************************************************************/
#include "Module_GlobalData.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

//测试模块配配置
static void m_Module_ConfigData_TestSys(void)
{
    int i;
    // 初始化PRU处理器
    Dev_data.Pru.RBaud = 0x03;
    Dev_data.Pru.MEndBit = 0x04;
    Dev_data.Pru.MBaud = 0x03;
    Dev_data.Pru.MSetSource =0x01;  //参数设置源

    //Module 7313 Bit1 设置参数
    Dev_data.Mconfig[1].Name = 0x7313;
    Dev_data.Mconfig[1].BitNum = 0x01;
    Dev_data.Mconfig[1].Ver= 0x0A;
    Dev_data.Mconfig[1].Baud = 0x03;

    for(i=0;i<6;i++) Dev_data.Mconfig[1].DOutFunSet[i] = 0x01;       //0-5 DO输出功能
    for(i=0;i<2;i++) Dev_data.Mconfig[1].DOutFunSet[6+i] = 0x02;   //6-7 PWM功能

    for(i=0;i<10;i++) Dev_data.Mconfig[1].DInFunSet[i] = 0x01;       //0-9通道DI 输入功能
    for(i=0;i<2;i++) Dev_data.Mconfig[1].DInFunSet[10+i] = 0x02;       //10-11通道Encoder功能
    Dev_data.Mconfig[1].DInFilterTime[0] = 0x01;         //单功能DI输入滤波时间设置

    for(i=0;i<6;i++) Dev_data.Mconfig[1].EnSCPulseSet[i] = 1024;  //旋转编码器脉冲值设置

    //Module 7314 Bit1 设置参数
    Dev_data.Mconfig[2].Name = 0x7314;
    Dev_data.Mconfig[2].BitNum = 0x02;
    Dev_data.Mconfig[2].Ver= 0x0A;
    Dev_data.Mconfig[2].Baud = 0x03;

    for(i=0;i<6;i++) Dev_data.Mconfig[2].DOutFunSet[i] = 0x01;       //0-5 DO输出功能
    for(i=0;i<2;i++) Dev_data.Mconfig[2].DOutFunSet[6+i] = 0x02;   //6-7 PWM功能

    for(i=0;i<5;i++) Dev_data.Mconfig[2].DInFunSet[i] = 0x01;       //0-9通道DI 输入功能
    for(i=0;i<1;i++) Dev_data.Mconfig[2].DInFunSet[5+i] = 0x02;       //10-11通道Encoder功能
    for(i=0;i<2;i++) Dev_data.Mconfig[2].SSIFunSet[i] = 0x01;       //SSI功能为绝对值编码器
    Dev_data.Mconfig[2].DInFilterTime[0] = 0x01;         //单功能DI输入滤波时间设置

    for(i=0;i<3;i++) Dev_data.Mconfig[2].EnSCPulseSet[i] = 1024;  //旋转编码器脉冲值设置

    //Module 7122 Bit1 设置参数
    Dev_data.Mconfig[3].Name = 0x7122;
    Dev_data.Mconfig[3].BitNum = 0x03;
    Dev_data.Mconfig[3].Ver= 0x0A;
    Dev_data.Mconfig[3].Baud = 0x03;
    for(i=0;i<4;i++) Dev_data.Mconfig[3].DInFilterTime[i] = 0x01;

    //Module 8012 Bit1 设置参数
    Dev_data.Mconfig[4].Name = 0x8012;
    Dev_data.Mconfig[4].BitNum = 0x04;
    Dev_data.Mconfig[4].Ver= 0x0A;
    Dev_data.Mconfig[4].Baud = 0x03;
    for(i=0;i<4;i++) Dev_data.Mconfig[4].AOutAReg[i] = 0x0B;
    for(i=0;i<4;i++) Dev_data.Mconfig[4].AOutVReg[i] = 0x01;
    for(i=0;i<12;i++) Dev_data.Mconfig[4].AInReg[i] = 0x01;
}


//模块设置参数初始化信息
void mHD_Module_ConfigData_Init(void)
{
    m_Module_ConfigData_TestSys();  //Test 系统设置参数初始化
}













