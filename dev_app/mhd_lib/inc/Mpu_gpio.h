#ifndef __MPU_GPIO_H
#define __MPU_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


 int mHD_Request_GPIO(int gpio);  //申请GPIO 返回 0成功 -1 失败
 int mHD_Uninstall_GPIO(int gpio);   //卸载GPIO 返回 0成功 -1 失败
 int mHD_SetGPIO_Direction(int gpio,char *direction);  //设置GPIO方向 direction gpio "in"  "out" 返回 0成功 -1 失败
 int mHD_GetGPIO_Value(int gpio);   //读取GPIO状态
 int mHD_SetGPIO_Value(int gpio,int value);  //设置GPIO输出  返回  -1 失败  0设置成功

int mHD_Read_GPIO(int gpio);    //读取GPIO 输入状态 完成后卸载设备
int mHD_Set_GPIO(int gpio,int value);  //设置GPIO状态 完成后卸载设备


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
