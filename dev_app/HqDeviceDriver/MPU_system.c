/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  MPU_system.c
 *       Description:  控制器IO系统
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 22:10 PM"
 ****************************************************************/
#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>

#include "MPU_system.h"

#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

/************* 控制器本体 GPIO 输出轮询 **********************************************
 * 名称：             mHD_MPUGPIO_FUN_OUTValue(void)
 * 功能：             控制器本体 GPIO 输出轮询
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回0
 * ********************************************************************************/
int  mHD_MPUGPIO_FUN_OUTValue(void)
{
    //static Hq_Mpu_Data old_Mpu_data;
    int i;
    int instatus;
    static int minput[6];

    for(i =0;i<6;i++)
    {
        instatus = mHD_Read_MPUGPIO_Value(i);
        if(instatus != minput[i])
        {
            minput[i] = instatus;
             if(HqDev_CmdSys.debug ==1) printf("input[%d] = %d \n",i,instatus);
        }
    }
    return 0;
//    int mHD_Read_MPUGPIO_Value(uint8_t ch)

//    for(i=0;i<8;i++)
//    {
//        if(old_Mpu_data.Data.DOutData[i] != Mpu_data.Data.DOutData[i])
//        {
//            old_Mpu_data.Data.DOutData[i] != Mpu_data.Data.DOutData[i];   //更新旧值
//            if(Mpu_data.config.DOutFunSet ==1)  //GPIO模式
//            {

//            }


//        }
//    }


//     if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}

//    Mpu_data.Data.DOutData

}

/************* 控制器本体 GPIO 输入轮询 **********************************************
 * 名称：             mHD_MPUGPIO_FUN_OUTValue(void)
 * 功能：             控制器本体 GPIO 输入轮询
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回0
 * ********************************************************************************/
int  mHD_MPUGPIO_FUN_InputValue(void)
{
    int i;
    static int minput[6];

    for(i =0;i<6;i++)
    {
        if(Mpu_data.config.DInFunSet[i] != 1) return -1;

       Mpu_data.Data.DInData[i] = mHD_Read_MPUGPIO_Value(i);
        if(Mpu_data.Data.DInData[i] != minput[i])
        {
            minput[i] = Mpu_data.Data.DInData[i];
             if(HqDev_CmdSys.debug ==1) printf("input[%d] = %d \n",i,Mpu_data.Data.DInData[i]);
        }
    }
    return 0;
}

/************* 控制器本体 GPIO 输出轮询 **********************************************
 * 名称：             mHD_MPUGPIO_FUN_OutputValue(void)
 * 功能：             控制器本体 GPIO 输出轮询
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回0
 * ********************************************************************************/
int  mHD_MPUGPIO_FUN_OutputValue(void)
{
    return 0;
}

/************* 控制器顶部 LED 指示灯 输出轮询 **********************************************
 * 名称：             mHD_MPUTOPLED_FUN_OutValue(void)
 * 功能：             控制器本体 GPIO 输出轮询
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回0
 * ********************************************************************************/
void  mHD_MPUTOPLED_FUN_OutValue(void)
{
    static HqTopLED_Cmd  old_Data;
    int i;

    HqTopLED_Data.in =0;
    for(i=0;i<6;i++)  HqTopLED_Data.in |= (Mpu_data.Data.DInData[i] &0x01) << i;  //输入状态赋值

    HqTopLED_Data.out =0;
    for(i=0;i<8;i++)  HqTopLED_Data.out |= (Mpu_data.Data.DOutData[i] &0x01) << i;  //输出状态赋值

    if((old_Data.in != HqTopLED_Data.in) ||(old_Data.out !=HqTopLED_Data.out) ||(old_Data.fun !=HqTopLED_Data.fun))
    {
        old_Data.in = HqTopLED_Data.in;
        old_Data.out = HqTopLED_Data.out;
        old_Data.fun = HqTopLED_Data.fun;
        mHD_Write_MPUTOPLED_Config(old_Data.in,old_Data.out,old_Data.fun);
    }
}
















