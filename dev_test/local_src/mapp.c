#include "local_inc/mapp.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/CommFun.h"
#include "local_inc/keyboard_input.h"

#include <pthread.h>


#define PRUDATA_MAX   22

char *m_shmkey ="/home/hqtech/hq_device/shmkey";   //共享内存目录
char *m_semkey ="/home/hqtech/hq_device/semkey";   //消息队列目录
char *m_msgkey ="/home/hqtech/hq_device/msgkey";   //消息队列目录




char mHD_keyboard_InBuf[1024] ;
int mHD_Keyboard_Finish = 0;
char mhd_test;

static void mHD_printf_err(void)
{
    printf("Input command error! Please check!\n");
}

static void mHD_printf_complete(void)
{
    printf("complete!!!\n");
}


//获取键盘输入
void * mHD_keyboard_Scan(void * arg)
{
    while(1)
    {
        fgets(mHD_keyboard_InBuf,1024,stdin);  //等待键盘输入/n结束
        mHD_Keyboard_Finish =1;
        *(int*)arg =1;
        usleep(10000);
    }
}

//接收信号更新共享内存数据到本地Dev_data
static void mHD_sig_handler_FromRpuData(int sig)
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

//初始化
int mHD_Software_Init(void)
{
    pthread_t  a_thread;
    int arg;
    //step1  初始化信号
    mHD_Register_SigFromRPU();
    //step2  开一个线程  用来获取键盘输入
    int res = pthread_create(&a_thread,NULL,mHD_keyboard_Scan,(void*)&arg);
    if(res !=0) { ; }  //线程创建失败处理

    //step3 获取共享内存，信号，消息队列键值
   Run_data.Shmkey = ftok(m_shmkey,97);  //共享内存键值
   Run_data.Semkey = ftok(m_semkey,98);  //信号键值
   Run_data.Msgkey = ftok(m_msgkey,99);  //消息队列键值
   Run_data.Pid[0] = getpid();

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
    if(mstrcmp(&analysis[1][0],"pru") ==0)
    {
        if((mstrcmp(&analysis[3][0],"msg") ==0) &&(atoi(&analysis[2][0]) ==0)) {  keyboard_prumsg(analysis,luozx) ;  return;}     //Pru msg
        else if((mstrcmp(&analysis[3][0],"data") ==0) &&(atoi(&analysis[2][0]) ==0)) { keyboard_prudata(analysis) ;  return;}    //Pru data
        else {mHD_printf_err(); return;}
    }
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

    if(Run_data.Pid[1]  == 0)  return;

    nummax = Dev_data.Pru.MEndBit;
    for(i=1;i<= nummax;i++)
    {
        if(Dev_data.Mconfig[i].Name == 0x7122)
        {
            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
        }
       else  if(Dev_data.Mconfig[i].Name == 0x8012)
        {
             if(memcmp(Dev_data.MData[i].AOutData,old_Dev_data.MData[i].AOutData,8) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
        }
        else if(Dev_data.Mconfig[i].Name == 0x7313)
        {
            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
            else if(memcmp(Dev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,6) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
            else if(memcmp(Dev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
        }
        else if(Dev_data.Mconfig[i].Name == 0x7314)
        {
            if(memcmp(Dev_data.MData[i].DOutData,old_Dev_data.MData[i].DOutData,16) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
            else if(memcmp(Dev_data.MData[i].EnInClear,old_Dev_data.MData[i].EnInClear,3) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
            else if(memcmp(Dev_data.MData[i].PulseData,old_Dev_data.MData[i].PulseData,32) !=0) {Dev_data.Pru.MWData[i] =1;cmp =1;}
        }
    }
    if(cmp ==1)
    {
        //mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,&Dev_data);
        if(Run_data.Pid[0] !=0)
        {
            memcpy((void *)&old_Dev_data,&Dev_data,sizeof(Hq_Dev_Data));
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);  //写入共享内存值
            mHD_Send_Signal(Run_data.Pid[1],SIGTRPU);  //发送信号到PRU 处理器
        }
        cmp =0;
    }
}







