#ifndef __PUBLIC_DATATYPE_H
#define __PUBLIC_DATATYPE_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
 #include<signal.h>

 #include<sys/ipc.h>

#define mHD_Debug

#define MODULE_NUM_MAX                32
#define MODULE_DIN_MAX                  32
#define MODULE_DIN_PORT_MAX       2
#define MODULE_DIN_FUN_MAX        12
#define MODULE_DOUT_MAX             16
#define MODULE_DOUT_FUN_MAX    8
#define MODULE_AIN_MAX                 12
#define MODULE_AOUT_MAX              4
#define MODULE_POUT_MAX              8
#define MODULE_EIN_MAX                 6
#define MODULE_SSIN_MAX               2
#define MODULE_LINK_MAX              32
#define MODULE_DIOCH_MAX          16

 //MPU
 #define MPU_DIN_FUN_MAX              6
 #define MPU_DOUT_FUN_MAX           8
 #define MPU_EIN_MAX                        3
 #define MPU_DIN_MAX                       6
 #define MPU_DOUT_MAX                    8
 #define MPU_POUT_MAX                   8

 /* Signals.  */
//SIGRTMIN = 34
 //SIGRTMAX = 64
  //SIGINT
#define SIGFRPU     SIGRTMIN+1   //来自PRU的数据更新信号
#define SIGTRPU     SIGRTMIN+2   //发送给PRU的数据更新信号

/* DB datas length */
#define DB_FLOAT_LEN 500
#define DB_UINT16_LEN 500
#define DB_UINT8_LEN 500
#define DB_INT_LEN 500
#define DB_BOOL_LEN 500

struct Data8_sBit{
     uint8_t B0:1; uint8_t B1:1; uint8_t B2:1; uint8_t B3:1;
     uint8_t B4:1; uint8_t B5:1; uint8_t B6:1; uint8_t B7:1;
 };

struct SSI_sBit{
     uint8_t SSI1:4;
     uint8_t SSI2:4;
 };

struct Data16_sBit{
     uint16_t B0:1; uint16_t B1:1;  uint16_t B2:1;  uint16_t B3:1;
     uint16_t B4:1; uint16_t B5:1;  uint16_t B6:1;  uint16_t B7:1;
     uint16_t B8:1; uint16_t B9:1;  uint16_t B10:1;uint16_t B11:1;
     uint16_t B12:1; uint16_t B13:1; uint16_t B14:1; uint16_t B15:1;
 };

struct Data32_sBit{
     uint32_t B0:1; uint32_t B1:1;  uint32_t B2:1;  uint32_t B3:1;
     uint32_t B4:1; uint32_t B5:1;  uint32_t B6:1;  uint32_t B7:1;
     uint32_t B8:1; uint32_t B9:1;  uint32_t B10:1;uint32_t B11:1;
     uint32_t B12:1; uint32_t B13:1; uint32_t B14:1; uint32_t B15:1;
     uint32_t B16:1; uint32_t B17:1; uint32_t B18:1; uint32_t B19:1;
     uint32_t B20:1; uint32_t B21:1; uint32_t B22:1; uint32_t B23:1;
     uint32_t B24:1; uint32_t B25:1; uint32_t B26:1; uint32_t B27:1;
     uint32_t B28:1; uint32_t B29:1; uint32_t B30:1; uint32_t B31:1;
 };

struct Data64_sBit{
     uint64_t B0:1; uint64_t B1:1;  uint64_t B2:1;  uint64_t B3:1;
     uint64_t B4:1; uint64_t B5:1;  uint64_t B6:1;  uint64_t B7:1;
     uint64_t B8:1; uint64_t B9:1;  uint64_t B10:1;uint64_t B11:1;
     uint64_t B12:1; uint64_t B13:1; uint64_t B14:1; uint64_t B15:1;
     uint64_t B16:1; uint64_t B17:1; uint64_t B18:1; uint64_t B19:1;
     uint64_t B20:1; uint64_t B21:1; uint64_t B22:1; uint64_t B23:1;
     uint64_t B24:1; uint64_t B25:1; uint64_t B26:1; uint64_t B27:1;
     uint64_t B28:1; uint64_t B29:1; uint64_t B30:1; uint64_t B31:1;
     uint64_t B32:1; uint64_t B33:1;  uint64_t B34:1;  uint64_t B35:1;
     uint64_t B36:1; uint64_t B37:1;  uint64_t B38:1;  uint64_t B39:1;
     uint64_t B40:1; uint64_t B41:1;  uint64_t B42:1;uint64_t B43:1;
     uint64_t B44:1; uint64_t B45:1; uint64_t B46:1; uint64_t B47:1;
     uint64_t B48:1; uint64_t B49:1; uint64_t B50:1; uint64_t B51:1;
     uint64_t B52:1; uint64_t B53:1; uint64_t B54:1; uint64_t B55:1;
     uint64_t B56:1; uint64_t B57:1; uint64_t B58:1; uint64_t B59:1;
     uint64_t B60:1; uint64_t B61:1; uint64_t B62:1; uint64_t B63:1;
 };

union Data8_Val{
     uint8_t	sVal;
     struct Data8_sBit sBit;
 };

union SSI_Val{
     uint8_t	SSI_Err;
     struct SSI_sBit SSI;
 };

union Data16_Val{
     uint16_t	sVal;
     struct Data16_sBit sbit;
 };

union Data32_Val{
     uint32_t	sVal;
     struct Data32_sBit sbit;
 };

union Data64_Val{
     uint64_t	sVal;
     struct Data64_sBit sbit;
 };

typedef enum {
     Msg_None,                                //无消息
     Msg_Link,                                  //仅连接
     Msg_DisLink,                            //断开连接
     Msg_Updata_Data,                   //数据更新
     Msg_WritePara,                        //写设置参数
     Msg_ReadPara,                         //读设置参数
     Msg_StartScan,                         //开始模块扫描
     Msg_StopScan,                         //结束模块扫描
     Msg_SoftInitComplete,            //软件初始化完成
     Msg_WriteParaComplete,        //写入参数设置完成
     Msg_ReadParaComplete,        //读取参数设置完成
     Msg_ScanStartStatus,               //扫描运行开始状态
     Msg_ScanStopStatus,                //扫描运行停止状态
 } msg_cmd;

typedef enum {
    MsgUpdate_None,
    MsgUpdate_WritePara,              //消息设置参数使能 执行后复位
    MsgUpdate_ReadPara,              //消息读取参数使能 执行后复位
    MsgUpdate_UpdataData,          //消息数据更新使能 执行后复位，模块写入数据更新
    MsgUpdate_StartScan,             //消息开始扫描模块使能 执行后复位
    MsgUpdate_StopScan,           //消息停止扫描模块使能 执行后复位
    MsgUpdate_DateChange,      //消息数据修改使能 执行后复位
} msg_update;

typedef enum {
    MsgType_None,
    MsgType_Dev,              //驱动程序PID   消息类型 1
    MsgType_Ctr,              //控制系统PID    消息类型2
    MsgType_Man,             //人机界面PID    消息类型3
    MsgType_Rem,             //远程系统PID    消息类型4
    MsgType_User1,           //用户1PID         消息类型5
    MsgType_User2,           //用户2PID         消息类型6
} msg_type;

 typedef struct {
     int             RShmid;               //信号量ID。
     int             RSemid;               //共享内存ID。
     int             RMsgid;               //消息队列ID。
     int             RShmkey;            //信号量ID
     int             RSemkey;            //共享内存ID。
     int             RMsgkey;            //消息队列ID。
     int             RPid[MODULE_LINK_MAX];   //[0] =驱动系统PID,[1] =控制系统PID, [2] =人机界面PID,[3] =远程系统PID
                                                                                         //[4]=用户1PID,[5]=用户2PID,[6]=用户3PID,
     int             RPidLink[MODULE_LINK_MAX];     //PID消息连接状态,定义同Pid 1=已连接，0 =未连接
     uint8_t      RStatus;              //PRU任务状态,0=无任务处理，1=有未完成的任务。
     uint8_t      RBaud;                //PRU处理器传输速率代码
                                                                      //0=115.2K ,1=9.6K,2=19.2K,3=115.2K, 4=150K,5=200K,6=250K,7=300K
                                                                      //8= 500K(默认),9=1M,10= 2M,11=3M,12=4M,13=6M.

     uint8_t       MStatus;             //PRU软件状态，0=未就绪，1=就绪
     uint8_t       MWSetStatus;     //写入参数设置状态，0=未设置，1=设置完成，
     uint8_t       MRSetStatus;      //读取参赛设置状态，0=未读取，1=读取完成，
     uint8_t       MScanStatus;      //模块扫描状态，0=未开始扫描，1=开始扫描。

     uint8_t       MStartBit;          //开始扫描模块位号。
     uint8_t       MEndBit;            //结束扫描的模块位号。
     uint8_t       MBaud;                //模块PRU连接波特率代码 同RBaud.
     uint8_t       MSetSource;        //模块参赛设置源, 0 = 配置文件，1= 参数设置，
     uint8_t       MErrCode[MODULE_NUM_MAX];      //运行错误代码 下标0=PRU处理器错误代码,下标1-x模块错误代码。
     uint8_t       MWData[MODULE_NUM_MAX];      //写入更新数据标识,下标0=PRU,下标1-x模块数据更新  1=有数据更新，0 =  无数据写入。
     uint8_t       MRData[MODULE_NUM_MAX];       //读取更新数据标识,下标0=PRU,下标1-x模块数据更新  1=有数据更新，0 =  无数据写入。
 } PRU_set;

 typedef struct {
     uint8_t     SetStatus;                   //模块参数设置完成
     uint8_t     ReadStatus;                 //模块读取参数设置完成
     uint8_t     ErrCode;                    //设置错误代码 详见错误代码表。
     uint8_t     BitNum;                    //模块位号
     uint16_t   Name;                       //模块名词
     uint8_t     Ver;                            //模块版本号
     uint8_t    Baud;                         //通讯波特率
     uint8_t     DInFilterTime[MODULE_DIN_PORT_MAX*2];   //数字量输入滤波时间设置代码 8通道1组设置,通道对应数组下标,。
                                                                                                   // 0=50us(默认),1=100us,2= 400us,3=800us 4=1.6ms,5=3.2ms,6=10ms,7=20ms,8=None。
     uint8_t     DInFunSet[MODULE_DIN_FUN_MAX];              //数字量输入通道功能设置,仅7313,7314有效 0=Close,1=DI,2=Encoder,3=Timer。
     uint8_t     DOutFunSet[MODULE_DOUT_FUN_MAX];        //数字量输出通道功能设置,仅7313,7314有效 0=Close,1=DO,2=PWM_50%。
     uint16_t   EnSCPulseSet[MODULE_EIN_MAX];                  //增量旋转编码器单圈脉冲数量,最大值65535 仅7313,7314有效
     uint8_t     SSIFunSet[MODULE_SSIN_MAX];                      //SSI 借口功能设置 0=不启用，1=启用SSI编码器模式，2=启用串口模式 仅7314有效
     uint8_t     AOutAReg[MODULE_AOUT_MAX];                   //模拟量输出电流输出范围代码
     uint8_t     AOutVReg[MODULE_AOUT_MAX];                   //模拟量输出电压输出范围代码
     uint8_t     AInReg[MODULE_AIN_MAX];                     //模拟量输入范围代码
 } Mod_config;

 typedef struct {
     uint8_t      BitNum;                                                 //位号
     uint16_t    Name;                                                     //模块名词
     uint8_t      Status;                                                    //设置任务状态,0=已完成写入，1=未完成写入。
     uint8_t      ErrCode;                                                 //设置错误代码 详见错误代码表。
     uint8_t      DInData[MODULE_DIN_MAX ];   //开关量输入状态,  只读
     uint8_t      DOutData[MODULE_DOUT_MAX ];                        //开关量输出状态,  读写
     uint8_t      EnInClear[MODULE_EIN_MAX];                             //旋转编码器清零。按位操作 读写
     uint8_t      EnInDir[MODULE_EIN_MAX];                                //增量旋转编码器旋转方向。按位操作 只读 0=正转，1=反转
     uint8_t      SSIInDir[MODULE_SSIN_MAX];                             //绝对值编码器旋转方向。按位操作 只读 0=正转，1=反转
     uint8_t      SSIErr[MODULE_SSIN_MAX];                                 //SSI旋转编码器错误码 B0,B3=SYNC B1,B4=RSI B2,B5=SYNC
     uint32_t    PulseData[MODULE_POUT_MAX];       //脉冲输出频率 读写
     int32_t      EnPulseData[MODULE_EIN_MAX];       //增量旋转编码器脉冲值 只读
     uint16_t    EnSpeedData[MODULE_EIN_MAX];      //增量旋转编码器速度 只读 单位 转/分
     uint32_t    SSIPulseData[MODULE_SSIN_MAX];    //SSI绝对值旋转编码器脉冲值 只读
     uint16_t    SSISpeedData[MODULE_SSIN_MAX];   //SSI绝对值旋转编码器速度 只读 单位 转/分
     uint16_t    AInData[MODULE_AIN_MAX];              //模拟量输入值,   只读
     uint16_t    AOutData[MODULE_AOUT_MAX ];       //模拟量输出值,   读写
 } Mod_data;

//MPU 通道配置
typedef struct {
    uint8_t     DInFunSet[MPU_DIN_FUN_MAX];              //数字量输入通道功能设置, 0=Close,1=DI,2=Encoder,3=Timer。
    uint8_t     DInPullSet[MPU_DIN_FUN_MAX];              //数字量输入通道上下拉设置, 0=下拉,1=上拉,
    uint8_t     DOutFunSet[MPU_DOUT_FUN_MAX];        //数字量输出通道功能设置, 0=Close,1=DO,2=PWM
    uint8_t     DOutPullSet[MPU_DOUT_FUN_MAX];        //数字量输出通道上下拉设置, 0=下拉,1=上拉,
    uint16_t   EnSCPulseSet[MPU_EIN_MAX];                  //增量旋转编码器单圈脉冲数量,最大值65535
} Cpu_config;
//MPU Data
typedef struct {
    uint8_t      DInErrCode[MPU_DIN_MAX];                  //输入通道错误代码 -1错误。
    uint8_t      DOutErrCode[MPU_DOUT_MAX];            //输出通道错误代码 -1错误。
    uint8_t      DInData[MPU_DIN_MAX ];                       //开关量输入状态,  只读
    uint8_t      DOutData[MPU_DOUT_MAX ];                 //开关量输出状态,  读写
    uint32_t   DoutDelayOff[MPU_DOUT_MAX];             //开关量输出Timer 功能关闭时间 ms
    uint32_t   DoutDelaynOn[MPU_DOUT_MAX];           //开关量输出Timer 功能打开时间 ms
    uint8_t      EnInClear[MPU_EIN_MAX];                      //增量旋转编码器清零,   读写
    uint8_t      EnInDir[MPU_EIN_MAX];                          //增量旋转编码器旋转方向。按位操作 只读 0=正转，1=反转
    uint32_t    PulseData[MPU_POUT_MAX];                  //脉冲输出频率 读写
    int32_t      EnPulseData[MPU_EIN_MAX];                 //增量旋转编码器脉冲值 只读
    uint16_t    EnSpeedData[MPU_EIN_MAX];                //增量旋转编码器速度 只读 单位 转/分
} Cpu_data;

//模块数据类型
typedef struct {
    PRU_set         Pru;                                                     //PRU处理器
    Mod_config   Mconfig[MODULE_NUM_MAX];         //模块设置参数
    Mod_data      MData[MODULE_NUM_MAX];           //模块读写数据
}Hq_Dev_Data;

//MPU数据类型
typedef struct {
    Cpu_config   config;         //MPU设置参数
    Cpu_data      Data;           //MPU读写数据
}Hq_Mpu_Data;

typedef struct {
    uint8_t           Step;                                                    //运行步骤
    key_t              Semkey;                                              //信号键值
    key_t              Shmkey;                                              //共享内存键值
    key_t              Msgkey;                                              //消息队列键值
    int                  Semid;                                                //信号ID
    int                  Shmid;                                                //共享内存ID
    int                  Msgid;                                                 //消息队列ID
    int                  Pid[MODULE_LINK_MAX];                 //进程PID [0] =驱动系统PID,[1] =控制系统PID, [2] =人机界面PID,[3] =远程系统PID
                                                                                       // //[4]=用户1PID,[5]=用户2PID,[6]=用户3PID,
    int                 PidLink[MODULE_LINK_MAX];           //PID 消息系统连接状态，定义同PID 0=未连接，1=连接
    uint8_t           ConfigTxtReadStatus;                        //配置文件读取状态 0 =未读取，1 = 已读取
    uint8_t           ParaSetStatus;                                   //参数设置状态 0 = 未设置完成，1 = 设置完成
    uint8_t           PareReadStatus;                                //参赛读取状态 0 = 未完成读取，1=  读取完成
    uint8_t           PollStatus;                                          //模块轮询状态 0= 未开始， 1= 开始轮询
    //msg_cmd      MsgCmd[MODULE_LINK_MAX];        //接收到的消息命令 参数同PID
    msg_cmd       MsgCmdStatus;                                    //消息状态，1有未处理的消息，0无待处理消息
    int                  MsgStatus[MODULE_LINK_MAX];     //消息处理状态
    msg_update  Msg_Update_CMD;                              //消息处理命令
    uint8_t           ModuleNum;                                       //模块数量
    int                  SigEn;                                                    //有需要处理的更新数据
}Hq_Run_Data;


//
typedef struct {
    float f_data[DB_FLOAT_LEN];
    uint16_t dw_data[DB_UINT16_LEN];
    uint8_t w_data[DB_UINT8_LEN];
    int i_data[DB_INT_LEN];
    int b_data[DB_BOOL_LEN];
 }Hq_Plc_Db;


extern Hq_Dev_Data Dev_data;             //模块 IO 数据定义
extern Hq_Mpu_Data Mpu_data;          //MPU IO 数据定义
extern Hq_Run_Data Run_data;            //本地运行 数据定义


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
