/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  SysTime_Callback.c
 *       Description:  系统定时器回调函数
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/


#include <stdio.h>
#include <stdint.h>

#include "SysTime_Callback.h"
#include "HqDeviceDriver/Remote433_ctr.h"

extern  int module_cnt_test;
/*** 系统定时中断调用函数 *****/
void mHD_Systme_Handle(void)
{
    module_cnt_test++;
    mHD_mHD_Remote_433_TopLed(); //遥控器指示等功能
}
