#ifndef __PRU_FUN_H
#define __PRU_FUN_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "../mhd_lib/inc/mhd_lib.h"

#define BUTRUN_RPU_1   193
#define BUTRUN_MPU_2  194

 void mHD_PRU_Load(void);                //加载PRU处理器软件
 void mHD_PRU_Uninstall(void);          //卸载PRU处理器软件
 int mHD_RPU_Start(int enset);            //启动PRU处理器
 int mHD_RPU_Init(void);                      //初始化RPU处理器任务
 int m_UpdateModule_SingeData(int num,int name); //发送运行单个数据到PRU处理器 但个数据

void mHD_Waiting_Msg_Cmd(void) ; //等待消息命令 执行任务
void  mHD_Send_MsgCmd_App(msg_cmd cmd); //发送消息到已连接的 APP
void mHD_Process_Msg_Cmd(void);

int m_Readpara_cmd(void); //读取模块设置参数
int m_StartModule_Scan(void);//开始扫描模块
int m_StopModule_Scan(void); //停止扫描模块

void mHD_Module_PRU_Start(void);            //启动PRU处理器
void mHD_Module_PRU_Stop(void);             //停止PRU处理器
int mHD_WritePare_RunSet(void);                 //配置文件参数设置
int mHD_ProcessMsg_CMD(void);                  //等待消息命令 执行任务
int mHD_SigUpdate_Send(void);                    //Sig更新数据
//void  mHD_DevData_ReplyAndInit(void);      //应答命令 无回复





#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
