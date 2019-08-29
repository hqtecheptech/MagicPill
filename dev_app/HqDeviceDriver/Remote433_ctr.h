#ifndef __REMOTE433_H
#define __REMOTE433_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
#include     <stdint.h>      /*数据类型宏定义 */

 /* 通讯参数定义---------------------------------------------------------------------*/
 //RX
 /*接收到的命令  头命令    命令源     传感器地址     按键状态高位  按键状态低位   校验和低位
  *             bit0-DO  bit1-0x83   bit2-0x10     bit3-0x10    bit4-0x11     bit5-0xCC
  ***************************************************************************************/
#define AS62_RX_CMD  0xD0 //接收数据的头命令 翻抛遥控器,上料遥控器,中控接收器
#define AS62_RX_NUM       5     	//串口接收数据的数量
 //TX
 /*接收到的命令  接收器高地址  接收器低地址    通讯频率    头命令    数据0   数据1   数据2  数据3  数据4  数据5 校验和地位
  *              bit0-0x14     bit1-0x0B    bit2-0x0B   bit3-BO   bit4   bit5    bit6   bit7   bit8  bit9  bit10
  **********************************************************************************************************************/
 #define AS62_TX_CMD   		 0xB0	  //发送数据的头命令 翻抛遥控器,上料遥控器,中控接收器
 #define AS62_TX_SYSFUN    0xA8     //Bit4 返回
 #define AS62_TX_NUM        11     	 //发送数据的数量
 #define AS62_DEV_NUM     32         //传感器数量

 #define    RATS62_FPXT         0x0B         //翻抛系统
 #define    RATS62_HLXT        0x17        //混料系统
 #define    RATS62_SLXT         0x15          //上料系统
 #define    RATS62_YTHXT      0x19      //一体化系统
 #define    RATS62_FLXT         0x1B         //制肥系统



 typedef struct {
     uint8_t    BT_JT;                     //急停按键,按下为0
     uint8_t    BT_11;                    //第1排左起第1个按键 按下为1
     uint8_t    BT_12;                    //第1排左起第2个按键
     uint8_t    BT_13;                    //第1排左起第3个按键
     uint8_t    BT_21;                    //第2排左起第1个按键
     uint8_t    BT_22;                    //第2排左起第2个按键
     uint8_t    BT_23;                    //第2排左起第3个按键
     uint8_t    BT_24;                    //第2排左起第4个按键
     uint8_t    BT_31;                    //第3排左起第1个按键
     uint8_t    BT_32;                    //第3排左起第2个按键
     uint8_t    BT_33;                    //第3排左起第3个按键
     uint8_t    BT_34;                    //第3排左起第4个按键
     uint8_t    BT_41;                    //第4排左起第1个按键
     uint8_t    BT_42;                    //第4排左起第2个按键
     uint8_t    BT_43;                    //第4排左起第3个按键
     uint8_t    BT_44;                    //第4排左起第4个按键
     uint8_t    speed;                    //速度 0-7
     uint8_t    dev;                        //设备选择 0-3
     uint8_t    sec_dev;                 //子设备选择 0-7
     int linkstatus;                        //按键指示灯板连接状态
     int fd;                                     //串口设备标识
 } Remote433_data;

 typedef enum {
     ATS62_Comm      =0,           //通信模式
     ATS62_Setting      =1,            //设置模式
     ATS62_FPXT         = 0x0B,         //翻抛系统
     ATS62_HLXT         = 0x17,         //混料系统
     ATS62_SLXT         = 0x15,          //上料系统
     ATS62_YTHXT      = 0x19,       //一体化系统
     ATS62_FLXT         = 0x1B,          //制肥系统
 } W433Hz_ModuleCtr;

int  mHD_Remote_433_Init(char *port,int speed); //433MHz遥控器接收初始化
int mHD_Remote_433_Set(uint16_t add,uint8_t speed,uint8_t chan,uint8_t option);  //设置433MHz遥控器无线参数

int mHD_Remote_433Dev_Set(uint8_t dev);  //设置遥控器对应的设备
int mHD_Remote_433_Recv(void);   //接收遥控器数据

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
