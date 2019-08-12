#ifndef __MPU_GPIO_H
#define __MPU_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


 int mHD_Read_GPIO(int gpio);    //读取GPIO 输入状态
 int mHD_SetGPIO_Value(int gpio,int value); //设置GPIO输出


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
