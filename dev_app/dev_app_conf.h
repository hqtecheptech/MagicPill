
/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:   dev_app_conf.h
 *       Description:  全局编译配置文件
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/

#ifndef __DEV_APP_CONF_H
#define __DEV_APP_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "mhd_lib/inc/mhd_lib.h"
#include "HqDeviceDriver/HqDeviceDriver.h"
#include "Local_app/MPU_GlobalData.h"
#include "Local_app/Module_GlobalData.h"

 /***设备名称定义***/
//#define HQ_TEST_SYSTEM                  //测试系统

#define HQ_HLXT                                 //混料系统
//#define HQ_FPXT                                 //翻抛系统
//#define HQ_CCXT                                 //除臭系统
//#define HQ_FJXT                                   //发酵系统


 /***项目名称定义***/
 #define GSQY1_PROJECT                           //甘肃庆阳项目





#define  REMOTE433_ENABLE      1                              //启用 遥控器功能
#define  KEYLED_ENABLE              1                              //启用 按键面板功能
#define  WIRELESS24_ENABLE      1                              //启用 无线2.4G通讯功能
#define  MODULE_DEVIO              1                              //启用 控制器连接IO模块功能

#define KEYLED_BAUD                  115200                    //按键LED板串口2 通信波特率
#define REMOTE433_BAUD          9600                        //无线遥控器连接的串口 4 通信波特率

#define WIRELESS24_BAUD          115200                     //按键LED板串口2 通信波特率
#define SYSTEM_MASTIMERM     100000                     //单位微妙
#define MSG_MOD_TYPE              MSG_CTRSOFT        //设置本机类型

#if defined HQ_FPXT
#define REMOTE_SYS_SET             RATS62_FPXT          //设置遥控器类型 翻抛机
#elif defined HQ_HLXT
#define REMOTE_SYS_SET              RATS62_HLXT         //混料系统
#endif










#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif



