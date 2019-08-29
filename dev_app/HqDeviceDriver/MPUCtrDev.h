#ifndef __MPUCTRDEV_H
#define __MPUCTRDEV_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

 typedef unsigned char		uint8_t;
 typedef unsigned short int	uint16_t;
 typedef unsigned int		uint32_t;

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

void mHD_MPUGPIO_Test(void);


int mHD_Write_MPUGPIO_Config(int ch,char *inout,char *mode,char *pull);   //MPU GPIO 通道配置
int mHD_Write_MPUGPIO_Value(int ch,char * fun,int value,int delayon,int delayoff);  //MPU GPIO 输出
int mHD_Write_MPUTOPLED_Config(int in,int out,int fun); //MPU GPIO LED 指示灯设置

int mHD_Write_MPUPulse_Request(int ch,int exp);  //申请PWM通道
int mHD_Write_MPUPulse_Enable(int ch,int enable); //打开PWM 输出通道
int mHD_Write_MPUPulse_Value(int ch,uint32_t period ,uint8_t duty,uint8_t polarty); //设置PWM输出参数

void mHD_MPU_ConfigModule_Set(void);  //MPUIO运行模式设置

int mHD_Read_MPUGPIO_Value(int ch);  //MPUGPIO 输入状态


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
