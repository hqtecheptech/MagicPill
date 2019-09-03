#ifndef __MODULE_UART_H
#define __MODULE_UART_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

#define BUTRUN_RPU_1   193
#define BUTRUN_MPU_2  194

#define MODULE_RTXEN         101                   //GPIO 4_5  0x4A00 3578
#define MODULE_SETEN         100                   //GPIO 4_4  0x4A00 3574

#define MODULE_TX_MAX          128       //发送缓存区数量
#define MODULE_RX_MAX          128       //接收缓存区数量

#define MODULE_SETPARA_BACK_LEN   12    //模块参数设置返回的总字节数



#define MODULE_CMD_CTR                     0x13     //PRU 控制命令
#define MODULE_CMD_READSET             0x14    //RPU 读取设置参数命令
#define MODULE_CMU_WRITESET           0x15    //PRU 写入设置参数命令
#define MODULE_CMU_READATA             0x16   //PRU 读取模块数据命令
#define MODULE_CMD_RWDATA              0x17   //PRU读写模块数据命令
#define MODULE_CMD_ERR                      0x41  //错误返回

#define MODULE_SETREG_ADD          0x00    //模块设置寄存器地址
#define MODULE_DATAREG_ADD         0x01    //模块设置寄存器地址
#define MODULE_SETREG_DATALEN      0x04    //读取模块设置参数数据长度
#define MODULE_SETREG_NCDATA       0x00    //读取模块数据位保留



#define MODULE_SETREG_ADD                       0x00    //模块设置寄存器地址
#define MODULE_DATAREG_ADD                    0x01    //模块设置寄存器地址

#define MODULE_7122                                    0x7122   //模块名称
#define MODULE_8012                                    0x8012   //模块名称
#define MODULE_7313                                    0x7313   //模块名称
#define MODULE_7314                                    0x7314   //模块名称

#define MODULE7122_WDARA_PDULEN        0x04    //7122 写入数据长度 04
#define MODULE8012_WDARA_PDULEN        0x0A    //8012 写入数据长度 10
#define MODULE7313_WDARA_PDULEN        0x23    //8012 写入数据长度 35
#define MODULE7314_WDARA_PDULEN        0x23    //8012 写入数据长度 35

#define MODULE7122_RPARA_PDULEN        0x09    //7122 读取到的设置参数的数据长度 09
#define MODULE8012_RPARA_PDULEN        0x19    //8012 读取到的设置参数的数据长度 25
#define MODULE7313_RPARA_PDULEN        0x26    //8012 读取到的设置参数的数据长度 38
#define MODULE7314_RPARA_PDULEN        0x1C    //8012 读取到的设置参数的数据长度 28

#define MODULE7122_RDATA_PDULEN        0x08    //7122 读取到的设置参数的数据长度 08
#define MODULE8012_RDATA_PDULEN        0x22    //8012 读取到的设置参数的数据长度 34
#define MODULE7313_RDATA_PDULEN        0x2A    //8012 读取到的设置参数的数据长度 42
#define MODULE7314_RDATA_PDULEN        0x25    //8012 读取到的设置参数的数据长度 37

#define PRU_CMD_CTRRETURN_LEN             0x04  //PRU控制命令数据长度
#define PRU_CMD_SETRETURN_LEN             0x04  //PRU控制命令数据长度

#define MODULE_CMD_SET7122_LEN             0x09  //PRU控制命令数据长度
#define MODULE_CMD_SET8012_LEN             0x19  //PRU控制命令数据长度  25
#define MODULE_CMD_SET7313_LEN             0x26  //PRU控制命令数据长度  38
#define MODULE_CMD_SET7314_LEN             0x1C  //PRU控制命令数据长度  28

 typedef struct {
     uint8_t      Num;
     uint16_t    Name;
     uint8_t      Ver;
     uint8_t      Data[64];
     uint16_t    Esturn[12];
     uint8_t      Baud;
 } Mod_SetPara;

 typedef struct {
     int Fd;                                   //串口描述
     uint32_t Baud;                   //串口波特率
     int Flow_ctrl;                       //串口控制流
     int Databits;                         //串口数据位
     int Stopbits;                        //串口停止位
     int Parity;                            //串口校验位
     uint8_t Wtime;                    //串口等待时间
     uint8_t Wlen;                       //串口接收长度
 } ModUart_Para;

 typedef struct {
     uint8_t Rxbuf[MODULE_RX_MAX];    //串口接收缓存区
     int RxComplete;                   //串口接收完成
     int RxNum;                           //串口接收数量
     uint8_t Txbuf[MODULE_TX_MAX];    //串口发送缓存区
     int TxComplete;                   //串口发送完成
     int TxNum;                            //串口发送数量
     int Paraerr;                              //参数配置错误 -1 = 设置参数错误，-2= 读取参数 错误，0= 无错误

     pthread_t Thread_Read_ID;    //接收线程ID
     pthread_t Thread_Wtite_ID;    //发送线程ID
     pthread_t Thread_RW_ID;    //发送线程ID
 } Module_Uart;

 int mHD_Module_SetUart_Para(ModUart_Para upara);  //模块串口参数设置
 int mHD_Module_Uart_Open(void);  //打开模块串口
 int mHD_Module_SetUart_Para(ModUart_Para upara); //模块串口参数设置
 int mHD_Module_ReadUart_Data(void); //读取模块串口数据
 int mHD_Module_WriteUart_Data(void); //写入模块串口数据
 int mHD_Module_Read_ParaSet(void); //模块参数读取

 int mHD_Module_Init(uint32_t baud);  //初始化模块



extern Module_Uart ModUart_data;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
