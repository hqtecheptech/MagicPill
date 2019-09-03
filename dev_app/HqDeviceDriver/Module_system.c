/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  Module_system.c
 *       Description:  外设IO系统
 *       Version:  1.0.0(08/26/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/26/2019 22:10 PM"
 ****************************************************************/
#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>

#include "Module_system.h"

#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"
#include "../PRUSoft_Ctl/PRU_Fun.h"
#include "../PRUSoft_Ctl/PRU_Protocol.h"

#define PRUDATA_MAX   22

char *m_shmkey ="/home/hqtech/hq_device/shmkey";   //共享内存目录
char *m_semkey ="/home/hqtech/hq_device/semkey";   //消息队列目录
char *m_msgkey ="/home/hqtech/hq_device/msgkey";   //消息队列目录

char mhd_test;

static void mHD_printf_err(void)
{
    printf("Input command error! Please check!\n");
}

//static void mHD_printf_complete(void)
//{
//    printf("complete!!!\n");
//}




//接收信号更新共享内存数据到本地Dev_data
void mHD_sig_handler_FromRpuData(int sig)
{
    if(sig == SIGFRPU)
    {
        mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);  //读共享内存
    }
}
//注册PRU Update 信号函数
void mHD_Register_SigFromRPU(void)
{
    signal(SIGFRPU,mHD_sig_handler_FromRpuData);
}

//初始化模块系统
int mHD_ModuleSoftware_Init(int type,int *prupid)
{   
    int cmd;
    int tmp =0;
    //step1  初始化信号
    mHD_Register_SigFromRPU();  //注册来自模块的信号处理函数

    //step3 获取共享内存，信号，消息队列键值
   Run_data.Shmkey = ftok(m_shmkey,97);  //共享内存键值
   Run_data.Semkey = ftok(m_semkey,98);  //信号键值
   Run_data.Msgkey = ftok(m_msgkey,99);  //消息队列键值
   Run_data.Pid[0] = getpid();

   //清空Msg队列
   do {
        tmp = mHD_Read_Msg_Cmd(type-6, prupid, &cmd);
   } while(tmp !=-1);
   if(HqDev_CmdSys.debug==1)  printf("msg clear! pid = %d\n",*prupid);

   //发送连接到模块系统命令
   mHD_Send_Msg_Cmd(Msg_Link,type);
   //1.3:等待连接成功命令返回
   do {
       tmp = mHD_Read_Msg_Cmd(type-6, prupid, &cmd);
   } while(tmp ==-1);
   if(HqDev_CmdSys.debug==1)  printf("msg link OK! pid = %d\n",*prupid);

   return 0;
}

//键盘输入控制
void mHD_keyboard_Cmd(void)
{
    char tfg[1024] ;
    char analysis[64][128];

   // strcpy(tfg,test);
    if(mHD_Keyboard_Finish== 0)  return;
    mHD_Keyboard_Finish =0;                     //复位键盘接收完成
    strcpy(tfg,mHD_keyboard_InBuf);        //复制键盘输入到tfg 数组
    int mlen = strlen(tfg);                                 //计算输入的字符数量
    //int cnt = mHD_Stringanalysis(tfg, analysis,' ','\n');
    tfg[mlen-1]  =  '\0';
    mHD_CLibanalysis(tfg, analysis, " ");
    //for(i=0;i<cnt;i++)  printf("%s\n",&analysis[i][0]);       //Test
    //printf("analsis num %d\n",cnt);       //Test
    //输入数据结构判断
    if((mstrcmp(&analysis[0][0],"write") !=0)&&(mstrcmp(&analysis[0][0],"read") !=0)) {  mHD_printf_err(); return; }
    else if((mstrcmp(&analysis[3][0],"msg") !=0)&&(mstrcmp(&analysis[3][0],"data") !=0)&&(mstrcmp(&analysis[3][0],"config") !=0))
    {  mHD_printf_err(); return; }
    else if((atoi(&analysis[2][0]) <0) || (atoi(&analysis[2][0]) >31))  {  mHD_printf_err(); return; }

     //keyboard_prudata(analysis);

    //处理PRU数据
//    if(mstrcmp(&analysis[1][0],"pru") ==0)
//    {
//        if((mstrcmp(&analysis[3][0],"msg") ==0) &&(atoi(&analysis[2][0]) ==0)) {  keyboard_prumsg(analysis,msgtype) ;  return;}     //Pru msg
//        else if((mstrcmp(&analysis[3][0],"data") ==0) &&(atoi(&analysis[2][0]) ==0)) { keyboard_prudata(analysis) ;  return;}    //Pru data
//        else {mHD_printf_err(); return;}
//    }
    //处理Module数据
    else if(mstrcmp(&analysis[1][0],"module")==0 )
    {
        if(mstrcmp(&analysis[3][0],"config") ==0) {  keyboard_moduleconfig(analysis) ;  return;}   //module config
        else if(mstrcmp(&analysis[3][0],"data") ==0) { keyboard_moduledata(analysis) ;   return;}  //module data
        else {mHD_printf_err(); return;}
    }
    //可以添加自己的代码

    else {mHD_printf_err(); return;} //无意义
}

/****** 消息队列信息 **********************/
void mHD_Readmsg_Poll(int msgtype )
{
    int pid;
    int cmd;

    int msgstatus = mHD_Read_Msg_Cmd(msgtype-6, &pid, &cmd);
    if(msgstatus == -1)  return;

    Run_data.Pid[1]  = pid;
    mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);

//    if(cmd ==  Msg_SoftInitComplete)    //PRU发来的消息
//    if(cmd == Msg_Updata_Data)
//     if(cmd == Msg_WriteParaComplete)
//     if(cmd == Msg_ReadParaComplete)
//     if(cmd == Msg_ScanStartStatus)
//     if(cmd == Msg_ScanStopStatus)
}

//Dev 数据比较 更新到共享内存 同时发送 数据更新到PRU 处理器信号
void mHD_CompDev_SigToRPU(void)
{
    static Hq_Dev_Data old_Dev_data;
    int nummax;
    int i;
    int cmp = 0;

    if (Dev_data.Pru.RStatus ==1) return;
    nummax = Dev_data.Pru.MEndBit;
    for(i=1;i<= nummax;i++)
    {
//        if(Dev_data.Mconfig[i].Name == 0x7122)
//        {
//            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0)
//            {Dev_data.Pru.MWData[i] =1;cmp =1;}
//        }
//       else  if(Dev_data.Mconfig[i].Name == 0x8012)
//        {
//             if(memcmp(Dev_data.MData[i].AOutData,old_Dev_data.MData[i].AOutData,8) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//        }
//        else if(Dev_data.Mconfig[i].Name == 0x7313)
//        {
//            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//            else if(memcmp(Dev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,6) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//            else if(memcmp(Dev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//        }
//        else if(Dev_data.Mconfig[i].Name == 0x7314)
//        {
//            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//            else if(memcmp(Dev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,3) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//            else if(memcmp(Dev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
//        }
    }
    if(cmp ==1)
    {
        memcpy((void *)&old_Dev_data,&Dev_data,sizeof(Hq_Dev_Data));
        Dev_data.Pru.RStatus =1;  //PRU任务状态,0=无任务处理，1=有未完成的任务。
        Run_data.DataChangeEN = 1;    //有任务处理任务状态
        cmp =0;
    }
}

//Dev 数据比较 更新到共享内存 同时发送 数据更新到PRU 处理器信号
void mHD_CompDev_DataToRPU_Thread(void)
{
    static Hq_Dev_Data old_Dev_data;
     Hq_Dev_Data mDev_data;
    int nummax;
    int i,y;

    while(1)
    {
        memcpy((void *)&mDev_data,&Dev_data,sizeof(Hq_Dev_Data));
        nummax = Dev_data.Pru.MEndBit;
        for(i=1;i<= nummax;i++)
        {
            if(Dev_data.Mconfig[i].Name == 0x7122)
            {
                if(memcmp(mDev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0)
                {
                    m_UpdateModule_SingeData(i,0x7122);
                    for(y=0;y<16;y++) old_Dev_data.MData[i].DOutData[y] = mDev_data.MData[i].DOutData[y]; //赋旧值
                }
            }
            else  if(Dev_data.Mconfig[i].Name == 0x8012)
            {
                if(memcmp(mDev_data.MData[i].AOutData,old_Dev_data.MData[i].AOutData,8) !=0)
                {
                    m_UpdateModule_SingeData(i,0x8012);
                    for(y=0;y<8;y++) old_Dev_data.MData[i].AOutData[y] = mDev_data.MData[i].AOutData[y]; //赋旧值
                }
            }
            else if(Dev_data.Mconfig[i].Name == 0x7313)
            {
                if((memcmp(mDev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) ||
                        (memcmp(mDev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,6) !=0)    ||
                        (memcmp(mDev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0))
                {
                    m_UpdateModule_SingeData(i,0x7313);
                    for(y=0;y<16;y++) old_Dev_data.MData[i].DOutData[y] = mDev_data.MData[i].DOutData[y]; //赋旧值
                    for(y=0;y<6;y++) old_Dev_data.MData[i].EnInClear[y] = mDev_data.MData[i].EnInClear[y]; //赋旧值
                    for(y=0;y<8;y++) old_Dev_data.MData[i].PulseData[y] = mDev_data.MData[i].PulseData[y]; //赋旧值
                }
            }
            else if(Dev_data.Mconfig[i].Name == 0x7314)
            {
                if((memcmp(mDev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) ||
                        (memcmp(mDev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,3) !=0)    ||
                        (memcmp(mDev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0) )
                    m_UpdateModule_SingeData(i,0x7314);
                for(y=0;y<16;y++) old_Dev_data.MData[i].DOutData[y] = mDev_data.MData[i].DOutData[y]; //赋旧值
                for(y=0;y<3;y++) old_Dev_data.MData[i].EnInClear[y] = mDev_data.MData[i].EnInClear[y]; //赋旧值
                for(y=0;y<8;y++) old_Dev_data.MData[i].PulseData[y] = mDev_data.MData[i].PulseData[y]; //赋旧值
            }
        }
    }
}

/**************** 创建线程收发模块数据****************************************
* 名称：            mHD_Module_Send_CreatThread(void)
* 功能：            创建线程发送模块数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
pthread_t Thread_ModuleData_ID;    //接收线程ID
int mHD_Module_Send_CreatThread(void)
{
    int ret;
    ret = pthread_create(&Thread_ModuleData_ID,NULL,(void *) mHD_CompDev_DataToRPU_Thread,NULL);
    if(ret !=0)
    {
        if(HqDev_CmdSys.debug ==1)
        {
            printf("Create Uart_Receive_thread error!\n");
            return -1;
        }
    }
    return 0;
}

//Module 接受数据线程函数
void mHD_ReadModule_Data_Thread(void)
{
    while(1)
    {
        //mHD_Pru_Protocol_ReadRunData();  //接收模块返回数据
        //mHD_CompDev_DataToRPU_Thread();  //发送模块数据线程
    }
}
/**************** 创建线程接收模块数据****************************************
* 名称：            mHD_Module_Send_CreatThread(void)
* 功能：            创建线程发送模块数据
* 入口参数：     无
* 出口参数：    正确返回发送数据的长度，错误返回为-1
*******************************************************************/
pthread_t Thread_ModuleRead_ID;    //接收线程ID
int mHD_Module_Read_CreatThread(void)
{
    int ret;
    ret = pthread_create(&Thread_ModuleRead_ID,NULL,(void *) mHD_ReadModule_Data_Thread,NULL);
    if(ret !=0)
    {
        if(HqDev_CmdSys.debug ==1)
        {
            printf("Create Uart_Receive_thread error!\n");
            return -1;
        }
    }
    return 0;
}












