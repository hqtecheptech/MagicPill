#ifndef __PRU_PROTOCOL_H
#define __PRU_PROTOCOL_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define PRU_CMD_CTR                     0x13     //PRU 控制命令
#define PRU_CMD_READSET             0x14    //RPU 读取设置参数命令
#define PRU_CMU_WRITESET           0x15    //PRU 写入设置参数命令
#define PRU_CMU_READATA             0x16   //PRU 读取模块数据命令
#define PRU_CMD_RWDATA              0x17   //PRU读写模块数据命令
#define PRU_CMD_ERR                      0x41  //错误返回

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

#define PRU_CMD_SET7122_LEN             0x09  //PRU控制命令数据长度
#define PRU_CMD_SET8012_LEN             0x19  //PRU控制命令数据长度  25
#define PRU_CMD_SET7313_LEN             0x26  //PRU控制命令数据长度  38
#define PRU_CMD_SET7314_LEN             0x1C  //PRU控制命令数据长度  28

#include<stdint.h>
#include "PRU_ConfigWrite.h"

typedef struct {
    uint8_t     Txbuf[512];
    uint8_t     Rxbuf[512];
    uint16_t   TxLen;
    uint16_t   TxCnt;
    uint16_t   RxLen;
    uint16_t   RxCnt;
    uint32_t   Baud;
    uint8_t     BaudCode;
    int            Rpmsg_fd;
    uint8_t     Rpmsg_Write_EN;
} PRU_ProTocol;

typedef enum  {
    PRU_INIT,
    PRU_Wirte_SetData,
    PRU_Read_SetData,
    PRU_Ctr_SCAN,
    PRU_Ctr_COMM
} PRU_CMD ;

typedef enum {
    Err_None,               //无错误
    Err_CRC,                //CRC校验错误
    Err_Timeout,        //ARM系统超时错误
    Err_Name,           //模块名称错误
    Err_Ver,                //模块版本错误
    Err_Bitnum,      //位号错误
    Err_Data,        //数据错误
}Rpmsg_err;

typedef enum {
     PRU_CMD_INIT               =0x01U,    //仅初始化连接
     PRU_CMD_SET                =0x02U,    //设置模块参数
     PRU_CMD_READ             =0x03U,    //读取模块参数
     PRU_CMD_SCAN_CTR    =0x04U,    //开始扫描模块控制
     PUR_CMD_SCAN_STOP  =0x05U,    //停止扫描模块
     PUR_CMD_REPLY            =0x06U,    //接收数据回复信号
} Rpmsg_CMD;

extern PRU_ProTocol PRU_Rpmsg;

uint8_t  mHD_Pru_Protocol_SetCmd(uint8_t cmd,uint8_t *data);
uint8_t  mHD_Pru_Protocol_SetData(Mod_set  setdata);
uint8_t  mHD_Pru_Protocol_RWeadRegData(void);
uint8_t  mHD_Pru_Protocol_ReadSetData(uint8_t *modnum);         //读取设置参数
void mHD_Debug_Printf(uint8_t *data,uint8_t len,uint8_t ser);
uint8_t  mHD_Pru_Protocol_ReadRunData(void);
int mHD_Pru_Protocol_WriteRunData(uint8_t mnum,uint16_t name);
void  mHD_Pru_Protocol_ReplyInit(uint8_t cmd,uint8_t *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
