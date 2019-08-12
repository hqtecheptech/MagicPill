#ifndef __MAPP_H
#define __MAPP_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

 enum msgname {
     luozx  =  7,
     wangzy,
     fanr,
     jiwm,
     chenyq,
     wanglg
 };

 extern char mHD_keyboard_InBuf[] ;
 extern int mHD_Keyboard_Finish;


 int mHD_Software_Init(void);   //初始化
 void mHD_keyboard_Cmd(void);  //键盘输入控制
 void mHD_Readmsg_Poll(int msgtype );  //消息队列读取轮询
 void mHD_CompDev_SigToRPU(void);  //Dev 数据比较 更新到共享内存 同时发送 数据更新到PRU 处理器信号
void mHD_Module_ConfigData_Init(void); //模块设置参数初始化信息

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
