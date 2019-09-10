/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Pub_Function.c
 *       Description:  公共功能函数
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "09/05/2019 00:44 AM"
 ****************************************************************/


#include "../inc/Dev_Fun.h"

/******************电机控制***********************************
*名称：            mHD_Motor_Ctr
*功能：            电机正反向控制
*入口参数：     buf 输入条件
*出口参数：     无
* 返回：           设备运行输出
*******************************************************************/
mHD_DevRun_F mHD_Motor_Ctr(mHD_DevRun_R buf)
{
    mHD_DevRun_F out;
    //正向运行
    if((buf.sf) && (buf.fk_mgn)&&(buf.fk_sr==0) )  out.r_mf =1;
    else out.r_mf =0;

    //反向运行
    if((buf.sr) && (buf.fk_mgn)&&(buf.fk_sf==0) ) out.r_mr =1;
    else out.r_mr =0;

    return out;
}

/******************超时检测***********************************
*名称：            mHD_DevDelay_Time
*功能：            设备延时运h
*入口参数：     in=输入条件，time 延时时间 单位秒
*出口参数：     out =输出条件
* 返回：            0 = 无输出，1= 输出
*******************************************************************/
int mHD_DevDelay_Time(mHD_Delay_Time buf)
{
    if((buf.in !=0)&&(buf.pt >=buf.spt))  return 1;
    else return 0;
}

/****************** 液压设备控制***********************************
*名称：             mHD_Hydro_Ctr
*功能：            液压设备控制
*入口参数：     buf 输入参数
*出口参数：     无
* 返回：            设备运行输出
*******************************************************************/
mHD_DevRun_F mHD_Hydro_Ctr(mHD_DevRun_R buf)
{
    mHD_DevRun_F out;

    if(((buf.vf) ||(buf.vr) )&& (buf.fk_mgn)&&(buf.fk_sr==0) )  out.r_mf =1;
    else out.r_mf =0;

     if((buf.fk_sf !=0)&&(buf.pt >=buf.spt))
     {
         if(buf.vf !=0) {
             out.rvalve =0;  out.fvalve =1;
             out.hvalve =1;  out.uvalve =1;
         }else if(buf.vr !=0) {
             out.rvalve =1;  out.fvalve =0;
             out.hvalve =1;  out.uvalve =1;
         }
     } else {
            out.rvalve =0;  out.fvalve =0;
            out.hvalve =0;  out.uvalve =0;
     }
    return out;
}















