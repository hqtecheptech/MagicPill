#ifndef __SHARE_MEMORY_H
#define __SHARE_MEMORY_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>

#include "Public_DataType.h"

/************* 信号量 *****************/
int mHD_Create_Semaphore(key_t key);     //创建信号量
int mHD_Semaphorel_semctl(int semid);   //控制信号量
int mHD_Delete_Semaphore(key_t key);    //删除信号量
int mHD_Sem_P_Lock(int semid);                //信号量p操作，由运行状转化为阻塞状态
int mHD_Sem_V_Unlock(int semid);            //信号量v操作，把一个被阻塞的进程唤醒

/********** 共享内存 *****************/
int mHD_Creat_Shm_ShareMemory(key_t key,int msize);       //请求共享内存区
int mHD_Delete_Shm_ShareMemory(key_t key);                      //销毁共享内存区
int mHD_Write_Shm_ShareMemory_DevData(key_t shmkey,key_t semkey,Hq_Dev_Data * data); //写入Hq_Dev_Data到共享内存
int mHD_Read_Shm_ShareMemory_DevData(key_t shmkey,key_t semkey,Hq_Dev_Data * data);   //读取共享内存值到Hq_Dev_Data

int mHD_Read_Shm_ShareMemory(key_t shmkey,key_t semkey,int len,void* data); //读取共享内存值 通用
int mHD_Write_Shm_ShareMemory(key_t shmkey,key_t semkey,int len,void * data); //写入数据到共享内存 通用

/************************ 消息队列 ***********************************/
int mHD_Creat_Meg_MessageQueue(key_t key);        //创建消息队列
int mHD_Delete_Meg_MessageQueue(key_t key );      //删除消息队列
int mHD_Send_Meg_MessageQueue(key_t key,int who, char *msg);      //发送消息
int mHD_Rec_Meg_MessageQueue(key_t key,int recvtype,char *out);   //读取消息

/************************ 信号 ***********************************/
void mHD_Send_Signal(int pid,int signal);   //发送信号

/************************ 消息队列 ***********************************/
int mHD_Send_Msg_Cmd(msg_cmd cmd,int type); //发送消息命令
int  mHD_Message_Analysis(char * inchar, int *pid, int *cmd); //解析收到的消息
int mHD_Read_Msg_Cmd(int type, int *pid, int *cmd);  //接收消息命令

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
