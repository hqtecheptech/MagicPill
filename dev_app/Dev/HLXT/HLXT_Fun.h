#ifndef __HLXT_FUN_H
#define __HLXT_FUN_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

 typedef  struct {
     uint8_t sf;            //正向运行给定
     uint8_t sr;            //反向运行给定
     uint8_t vf;            //正向阀开启给定
     uint8_t vr;            //反向阀开启给定
     uint8_t fk_sf;         //正向运行反馈
     uint8_t fk_sr;         //反向运行反馈
     uint8_t fk_mgn;    //电机故障反馈
     uint16_t pt;           //延时设定时间
     uint16_t spt;          //首次运行时间
 } mHD_DevRun_R;

 typedef  struct {
     uint8_t fk_tfgn;  //正向超时反馈
     uint8_t fk_trgn;  //反向超时反馈
     uint8_t r_mf;      //正向输出
     uint8_t r_mr;     //反向输出
     uint8_t fvalve;    //正向阀
     uint8_t rvalve;    //反向阀
     uint8_t uvalve;   //起压阀
     uint8_t hvalve;   //保压阀
     uint16_t pvalve;    //比例流量阀
 } mHD_DevRun_F;

 typedef  struct {
     uint8_t in;               //输入条件
     uint16_t pt;            //设定时间
     uint16_t spt;          //首次运行时间
     uint8_t out;           //条件判断输出
 } mHD_Delay_Time;

 //mHD_PDJ_F mHD_HLXT_SXPD_Ctr(mHD_PDJ_R buf);  //双向皮带机控制




#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
