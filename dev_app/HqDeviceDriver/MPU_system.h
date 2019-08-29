#ifndef __MPU_SYSTEM_H
#define __MPU_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


 int  mHD_MPUGPIO_FUN_OUTValue(void);  //控制器本体 GPIO 输出轮询
 int  mHD_MPUGPIO_FUN_InputValue(void); //控制器本体 GPIO 输入轮询
 void  mHD_MPUTOPLED_FUN_OutValue(void); //控制器顶部 LED 指示灯 输出轮询



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
