#ifndef __MODULE_SYSTEM_H
#define __MODULE_SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define MSG_CTRSOFT       7
#define MSG_MANSOFT     8
#define MSG_REMSOFT      9
#define MSG_NETSOFT      10
#define MSG_USER1          11
#define MSG_USER2          12

typedef enum  {
     msg_CtrSoft  =  7,    //控制软件
     msg_ManSoft,           //人机界面
     msg_RemSoft,           //远程系统 上位机
     msg_NetSoft,            //网络连接系统
     msg_User1,               //用户1
     msg_User2                //用户2
 } msgname;

int mHD_ModuleSoftware_Init(int type,int *prupid);  //初始化
void mHD_keyboard_Cmd(int msgtype);     //键盘输入控制
void mHD_Readmsg_Poll(int msgtype );  //消息队列读取轮询
void mHD_CompDev_SigToRPU(void);  //Dev 数据比较 更新到共享内存 同时发送 数据更新到PRU 处理器信号
void mHD_Module_ConfigData_Init(void); //模块设置参数初始化信息



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
