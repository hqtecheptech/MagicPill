/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  MPU_GlobalData.c
 *       Description: 控制器本体全局数据处理
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 22:18 PM"
 ****************************************************************/
#include "MPU_GlobalData.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

//MPUIO设置参数初始化信息
void mHD_MPU_ConfigData_Init(void)
{
    /******* OUT CH0 - CH1 只能设置为 GPIO 输出模式 ****/
    //输出0 通道配置
    Mpu_data.config.DOutFunSet[0] = 1;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[0] =0;   //0=downpull,1=pupull
    //输出1 通道配置
    Mpu_data.config.DOutFunSet[1] = 1;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[1] =0;   //0=downpull,1=pupull

     /******* OUT CH2 - CH3 可设置为 GPIO或Pulse 输出模式 ****/
    //输出2 通道配置
    Mpu_data.config.DOutFunSet[2] = 1;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[2] =0;   //0=downpull,1=pupull
    //输出3 通道配置
    Mpu_data.config.DOutFunSet[3] = 1;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[3] =0;   //0=downpull,1=pupull

    /******* OUT CH4 - CH7 只能设置为 Pulse 脉冲输出模式 ****/
    //输出4 通道配置
    Mpu_data.config.DOutFunSet[4] = 2;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[4] =0;   //0=downpull,1=pupull
    //输出5 通道配置
    Mpu_data.config.DOutFunSet[5] = 2;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[5] =0;   //0=downpull,1=pupull
    //输出6 通道配置
    Mpu_data.config.DOutFunSet[6] = 2;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[6] =0;   //0=downpull,1=pupull
    //输出7 通道配置
    Mpu_data.config.DOutFunSet[7] = 2;  //1=DO,2=PWM
    Mpu_data.config.DOutPullSet[7] =0;   //0=downpull,1=pupull

    //输入0通道配置 注:编码器模式需要占2个通道 0,1
    Mpu_data.config.DInFunSet[0] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[0] = 0;   //0=downpull,1=pupull
    //输入1通道配置 注:编码器模式需要占2个通道 0,1
    Mpu_data.config.DInFunSet[1] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[1] = 0;   //0=downpull,1=pupull

    //输入2通道配置 注:编码器模式需要占2个通道 2,3
    Mpu_data.config.DInFunSet[2] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[2] = 0;   //0=downpull,1=pupull
    //输入3通道配置 注:编码器模式需要占2个通道 2,3
    Mpu_data.config.DInFunSet[3] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[3] = 0;   //0=downpull,1=pupull

    //输入4通道配置 注:编码器模式需要占2个通道 4,5
    Mpu_data.config.DInFunSet[4] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[4] = 0;   //0=downpull,1=pupull
    //输入5通道配置 注:编码器模式需要占2个通道 4,5
    Mpu_data.config.DInFunSet[5] = 1;   //1=DI,2=Encoder
    Mpu_data.config.DInPullSet[5] = 0;   //0=downpull,1=pupull

    //编码器单圈最大脉冲值设置
    //编码器0通道
    Mpu_data.config.EnSCPulseSet[0] = 1024;
    //编码器1通道
    Mpu_data.config.EnSCPulseSet[1] = 1024;
    //编码器2通道
    Mpu_data.config.EnSCPulseSet[2] = 1024;
    //循环设置方法
//    int i;
//    for(i=0;i<MPU_DOUT_MAX;i++)  {Mpu_data.config.DOutFunSet[i] = 1; Mpu_data.config.DOutPullSet[i] =0;}//1=DO,2=PWM 0=downpull,1=pupull
//    for(i=0;i<MPU_DIN_MAX;i++)  {Mpu_data.config.DInFunSet[i] = 1; Mpu_data.config.DInPullSet[i] = 0;} //1=DI,2=Encoder 0=downpull,1=pupull
//    for(i=0;i<MPU_EIN_MAX;i++)  Mpu_data.config.EnSCPulseSet[i] = 1024;

     if(HqDev_CmdSys.debug==1) printf("MPU SetData OK!");
}













