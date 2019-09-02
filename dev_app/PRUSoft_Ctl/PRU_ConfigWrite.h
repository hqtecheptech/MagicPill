#ifndef __PRU_CONFIGWRITE_H
#define __PRU_CONFIGWRITE_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

 typedef struct {
     uint8_t     bitnum;
     uint16_t   name;
     uint8_t     ver;
     uint8_t     setdata[64];
     uint16_t   esturn[12];
 } Mod_set;

 typedef struct {
     uint8_t  Devbaud;
     uint8_t  Modbaud;
 } Mod_system;


 extern Mod_system M_SetSysData;
 extern Mod_set M_SetData[];

 uint8_t mHD_MPU_Read_SetFile(void); //读取设置参数,填充到SetData 数组

 int  mHD_Read_Config_Conf(void); //读取系统运行配置文本文件 debug信息
 int  mHD_Read_Run_Data(void); //读取系统运行配置二进制文件
 int  mHD_Write_Run_Data(void); //写入系统运行配置二进制文件


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
