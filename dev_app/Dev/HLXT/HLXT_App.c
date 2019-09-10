/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  HLXT_App.c
 *       Description:  公共应用函数
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "09/05/2019 00:44 AM"
 ****************************************************************/



#include "../../dev_app_conf.h"

#ifdef HQ_HLXT

#include "HLXT_App.h"


typedef  struct {
    uint8_t firstData;
    uint8_t secData;

} mHD_HLXT_KeyLed;

mHD_HLXT_KeyLed   mHD_HLXT_KeyData;


/******************按键面板及LED解析***********************************
*名称：            mHD_Module_SetUart_Para
*功能：            串口参数设置
*入口参数：     sf=正向运行，sr=反向运行
*出口参数：    fk_sf=正向运行反馈，fk_sr = 反向运行反馈
*                        fk_mgn= 电机故障信号，fk_zgn =综合故障
*                        r_mf  = 电机正向运行,r_mr  = 电机反向运行
*******************************************************************/
void mHD_HLXT_KeybardLed_DataInit(void)
{
    int i;
    uint8_t *fdata[] = {&mHD_KBData.InPort8[0],&mHD_KBData.InPort8[1],&mHD_KBData.InPort9[0],&mHD_KBData.InPort9[1],
                                  &mHD_KBData.InPort9[2],&mHD_KBData.InPort9[3],&mHD_KBData.InPort9[4],&mHD_KBData.InPort9[5],
                                  &mHD_KBData.InPort9[6],&mHD_KBData.InPort9[7]};

    uint8_t *sdata[] = {&mHD_KBData.InPort7[7],&mHD_KBData.InPort8[2],&mHD_KBData.InPort8[3],&mHD_KBData.InPort7[0],
                                   &mHD_KBData.InPort7[1],&mHD_KBData.InPort7[2],&mHD_KBData.InPort7[3],&mHD_KBData.InPort7[4],
                                   &mHD_KBData.InPort7[5],&mHD_KBData.InPort7[6]};

    //旋转开关赋值
    for(i=0;i<10;i++)
    {
        if(*fdata[i] == 1)   mHD_HLXT_KeyData.firstData=i+1;
        else mHD_HLXT_KeyData.firstData =0;
        if(*sdata[i] == 1)   mHD_HLXT_KeyData.secData=i+1;
        else mHD_HLXT_KeyData.secData= 0;
    }




}














#endif
