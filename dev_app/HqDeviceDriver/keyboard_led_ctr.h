#ifndef __KEYBOARD_LED_H
#define __KEYBOARD_LED_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

 #include     <stdint.h>      /*数据类型宏定义 */

 typedef struct {
     uint8_t  InPort1[8];             //Port1输入通道值
     uint8_t  InPort2[8];             //Port2输入通道值
     uint8_t  InPort3[8];             //Port3输入通道值
     uint8_t  InPort4[8];             //Port4输入通道值
     uint8_t  InPort5[8];             //Port5输入通道值
     uint8_t  InPort6[8];             //Port6输入通道值
     uint8_t  InPort7[8];             //Port7输入通道值
     uint8_t  InPort8[8];             //Port8输入通道值
     uint8_t  InPort9[8];             //Port9输入通道值

     uint8_t OutPort1[8];          //Port1输出通道值
     uint8_t OutPort2[8];          //Port2输出通道值
     uint8_t OutPort3[8];          //Port3输出通道值
     uint8_t OutPort4[8];          //Port4输出通道值
     uint8_t OutPort5[8];          //Port5输出通道值
     uint8_t OutPort6[8];          //Port6输出通道值

     uint8_t  EnPort1Data;            //CN1旋转编码器值
     uint8_t  EnPort2Data;            //CN1旋转编码器值
     uint8_t  EnPort3Data;            //CN1旋转编码器值

     uint8_t EnPort1Dir;                //CN1旋转编码器方向  0=未选择 1=正向选择 2=反向旋转
     uint8_t EnPort2Dir;                //CN1旋转编码器方向
     uint8_t EnPort3Dir;                //CN1旋转编码器方向

     int fd;                                      //设备串口文件描述
     int linkstatus;                         //按键指示灯板连接状态
 } Keyboard_data;

 /************* mHD_Keyboard_Led__Init **********************************************
  *  char * port      串口号(ttyS0,ttyS1,ttyS3,ttyS4,ttyS5,ttyS6,ttyS7)
  *                          注：keyboard_led 默认连接到串口2, “/dev/ttyS1"
  *  speed      串口速度
  * return :     FALSE -1  错误，正确返回文件描述符
  * ********************************************************************************/
 int  mHD_Keyboard_Led_Init(char *port,int speed);  //按键板及LED指示灯初始化
 int mHD_Keyboare_LED_Recv(void); // 接收按键板数据 recv_buf 接收缓存区
 int mHD_Keyboare_LED_Send(void);  //写入按键及LED板LED指示灯状态

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
