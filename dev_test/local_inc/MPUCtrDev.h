#ifndef __MPUCTRDEV_H
#define __MPUCTRDEV_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define  MPU_GPIO_IN                      "in"               //输入通道
#define  MPU_GPIO_OUT                  "out"             //输出通道
#define  MPU_MODE_GPIO              "gpio"            //输入输出GPIO模式
#define  MPU_MODE_PULSE            "pulse"          //输出脉冲模式
#define  MPU_MODE_ENCODER      "encoder"     //输入增量旋转编码器模式
#define  MPU_PULL_DOWN             "down"          //输入输出下拉模式
#define  MPU_PULL_UP                     "up"              //输入输出上拉模式

#define  MPU_DO_GPIO                  "gpio"              //GPIO输出
#define  MPU_DO_TIMER                "timer"            //GPIO输出使用Timer 功能
#define  MPU_DO_TF                       "mmc0"           //GPIO输出使用TF 指示功能
#define  MPU_DO_HEART               "heartbeat"     //GPIO输出使用心跳指示功能
#define  MPU_DO_CPU                   "cpu0"             //GPIO输出使用CPU运行指示功能

void mHD_MPUGPIO_Test(void);
int mHD_Write_MPUGPIO_Config(int ch,char *inout,char *mode,char *pull);   //MPU GPIO 通道配置
int mHD_Write_MPUGPIO_Value(int ch,char * fun,int value,int delayon,int delayoff);  //MPU GPIO 输出


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
