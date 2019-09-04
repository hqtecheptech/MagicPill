/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Hardware_Config.c
 *       Description:  配置系统IO模块
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "09/05/2019 00:44 AM"
 ****************************************************************/



#include "../../../dev_app_conf.h"

#if defined HQ_FPXT && defined GSQY1_PROJECT

#include "GSQY1_FPXT_Hardware_Config.h"


/******************系统硬件IO系统硬件初始化功能**************************************
*名称：            mHD_Dev_Hardware_Config_Init(void)
*功能：            IO系统硬件功能初始化
*入口参数：     无
*出口参数：     无
* 返回：            无
*******************************************************************/

void mHD_Dev_Hardware_Config_Init(void)    //甘肃庆阳混料系统硬件配置文件
{
    int i;
    // 初始化PRU处理器
    Dev_data.Pru.RBaud = 0x08;
    Dev_data.Pru.MEndBit = 0x07;
    Dev_data.Pru.MBaud = 0x08;
    Dev_data.Pru.MSetSource =0x01;  //参数设置源

    //Module 7313 Bit1 设置参数
    Dev_data.Mconfig[1].Name = 0x7313;
    Dev_data.Mconfig[1].BitNum = 0x01;
    Dev_data.Mconfig[1].Ver= 0x0A;
    Dev_data.Mconfig[1].Baud = 0x08;

    for(i=0;i<8;i++) Dev_data.Mconfig[1].DOutFunSet[i] = 0x01;       //0-8 DO输出功能
    for(i=0;i<12;i++) Dev_data.Mconfig[1].DInFunSet[i] = 0x01;       //0-11 相对编码器输入功能
    Dev_data.Mconfig[1].DInFilterTime[0] = 0x01;         //单功能DI输入滤波时间设置
    for(i=0;i<1;i++) Dev_data.Mconfig[1].EnSCPulseSet[i] = 1024;  //旋转编码器脉冲值设置
    //Module 7122 Bit1 设置参数
    Dev_data.Mconfig[2].Name = 0x7122;
    Dev_data.Mconfig[2].BitNum = 0x02;
    Dev_data.Mconfig[2].Ver= 0x0A;
    Dev_data.Mconfig[2].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[2].DInFilterTime[i] = 0x01;
    //Module 7122 Bit1 设置参数
    Dev_data.Mconfig[3].Name = 0x7122;
    Dev_data.Mconfig[3].BitNum = 0x03;
    Dev_data.Mconfig[3].Ver= 0x0A;
    Dev_data.Mconfig[3].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[3].DInFilterTime[i] = 0x01;
    //Module 7122 Bit1 设置参数
    Dev_data.Mconfig[4].Name = 0x7122;
    Dev_data.Mconfig[4].BitNum = 0x04;
    Dev_data.Mconfig[4].Ver= 0x0A;
    Dev_data.Mconfig[4].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[4].DInFilterTime[i] = 0x01;

    //Module 8012 Bit1 设置参数
    Dev_data.Mconfig[5].Name = 0x8012;
    Dev_data.Mconfig[5].BitNum = 0x05;
    Dev_data.Mconfig[5].Ver= 0x0A;
    Dev_data.Mconfig[5].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[5].AOutAReg[i] = 0x0B;
    for(i=0;i<4;i++) Dev_data.Mconfig[5].AOutVReg[i] = 0x01;
    for(i=0;i<12;i++) Dev_data.Mconfig[5].AInReg[i] = 0x01;
    //Module 8012 Bit1 设置参数
    Dev_data.Mconfig[6].Name = 0x8012;
    Dev_data.Mconfig[6].BitNum = 0x06;
    Dev_data.Mconfig[6].Ver= 0x0A;
    Dev_data.Mconfig[6].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[6].AOutAReg[i] = 0x0B;
    for(i=0;i<4;i++) Dev_data.Mconfig[6].AOutVReg[i] = 0x01;
    for(i=0;i<12;i++) Dev_data.Mconfig[6].AInReg[i] = 0x01;
    //Module 8012 Bit1 设置参数
    Dev_data.Mconfig[7].Name = 0x8012;
    Dev_data.Mconfig[7].BitNum = 0x07;
    Dev_data.Mconfig[7].Ver= 0x0A;
    Dev_data.Mconfig[7].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[7].AOutAReg[i] = 0x0B;
    for(i=0;i<4;i++) Dev_data.Mconfig[7].AOutVReg[i] = 0x01;
    for(i=0;i<12;i++) Dev_data.Mconfig[7].AInReg[i] = 0x01;
}
#endif














