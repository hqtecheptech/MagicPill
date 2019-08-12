
/********************************************************
 * File name: Share_Memory.c
 * Features:Shared memory and Semaphore
 * Created Time:2019-07-08
 * ******************************************************/

#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

#include<string.h>
#include <errno.h>
#include <sys/sem.h>
#include<signal.h>

#include "../inc/mhd_lib.h"

#define MSGTEXT_MAX  128

union semun {
    int  val;
    struct semid_ds   *buf;
    unsigned short   *array;
    struct   seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

typedef struct
{
    long msg_type;
    char text[MSGTEXT_MAX];
} m_Msg_Gt;

/************************ 信号量 ***********************************/
/***  信号量创建 *******
 * 参数   key_t key   表示所创建或打开信号量集的键。
 *返回： 成功：返回信号量标识符
 *            失败：返回 -1
 * ****************************************************/
int mHD_Create_Semaphore(key_t key)
{
    int semid;
    int status;
    semid = semget(key,1,IPC_CREAT|0666);  //创建失败返回 -1
    if(semid == -1) return -1;

    status =  mHD_Semaphorel_semctl(semid);  //初始化信号量
    if(status == -1) return -1;
    return semid;
}

/***  控制信号量信息 *******
 * 参数   int semid   信号集的标识符，即是信号表的索引。
 *           iint semnum  信号集的索引，用来存取信号集内的某个信号
 *返回： 成功：返回为一个正数
 *            失败：返回 -1
 * ****************************************************/
int mHD_Semaphorel_semctl(int semid)
{
    union semun arg;
    arg.val = 1;
    int status = semctl(semid,0,SETVAL,arg);  //初始化信号量;
    if(status ==-1)  return -1;
    return 0;
}

/***  删除进程信号量 *******
 *  参数：int semid  信号量标识符
 *  返回：  成功：0
 *               失败：-1
 * ****************************************************/
int mHD_Delete_Semaphore(key_t key)
{
    union semun sembuf;
    int semid;

    semid = semget(key,1,IPC_CREAT|0666);  //创建失败返回 -1
    if(semid == -1) return -1;

    if(semctl(semid,0,IPC_RMID,sembuf) == -1) return -1;
    else return 0;
}

/***  信号量p操作，由运行状转化为阻塞状态 *******
 *       锁定
 *  操作：申请一个空资源将信量 减1,成功退出，失败 该进程被阻塞
 *  参数：int semid  信号量标识符
 *  返回：  成功：0
 *               失败：-1
 * ****************************************************/
int mHD_Sem_P_Lock(int semid)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = -1;      //信号量减1
    sbuf.sem_flg = SEM_UNDO;

    if(semop(semid,&sbuf,1) == -1)  return -1;
    else return 0;
}

/***  信号量v操作，把一个被阻塞的进程唤醒,它有一个参数表，存放着等待被唤醒的进程信息 *******
 *     解锁
 *  操作：释放一个被占用的资源（把信号量加1）,如果发现有被阻塞的进程，则选择一个唤醒之
 *  参数：int semid  信号量标识符
 *  返回：  成功：0
 *               失败：-1
 * ****************************************************/
int mHD_Sem_V_Unlock(int semid)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = 1; //信号量加1
    sbuf.sem_flg = SEM_UNDO;
    if(semop(semid,&sbuf,1) == -1) return -1;
    else return 0;
}

/************************ 共享内存 ***********************************/
/***  请求共享内存区 *******
 * 参数 const char * fname  是一个已存在的可访问的路径或者文件 "." "./" ""/home/hqtech/modle/""
 *          int id  子序列号 不为0
 *          int msize 共享内存区大小
 *返回： 成功：共享内存断标识符ShareMomery
 *            失败：-1
 * ****************************************************/
int mHD_Creat_Shm_ShareMemory(key_t key,int msize)
{
   int shmid;
   shmid = shmget(key,msize,IPC_CREAT|0666);     //创建共享内存，返回一个id
   if(shmid == -1) return -1;
   else return shmid;
}

/***  销毁共享内存区 *******
 * 参数 key_t key  共享内存KEY
 *返回： 成功：共享内存段标识符
 *            失败：-1
 * ****************************************************/
int mHD_Delete_Shm_ShareMemory(key_t key)
{
    int shmid  = shmget(key,0,0);     //获取共享内存，返回一个id
    if(shmid == -1)  return -1;

     shmdt((int *)shmid);  //分离共享内存
    if(shmctl(shmid,IPC_RMID,NULL) == -1) return -1;
    else return shmid;
}

/****  写入Hq_Dev_Data到共享内存 *******
 *  说明：Hq_Dev_Data
 *  参数：key_t key  共享内存KEY
 *              Hq_Dev_Datat * data 指向需要写入的数
 * 返回：成功:写入的共享内存段标识符
 *             失败：-1
 * **************************************/
int mHD_Write_Shm_ShareMemory_DevData(key_t shmkey,key_t semkey,Hq_Dev_Data * data)
{
    int shmid;
    int semid;
    void *p;

    shmid  = shmget(shmkey,0,0);     //获取共享内存，返回一个id
    if(shmid == -1)  return -1;

    p  = shmat(shmid,0,0); //映射共享内存，的到虚拟地址
    if(p == (void*)-1)  return  -1;

    semid = semget(semkey,0,0);  //获取信号ID
    if(semid == -1) return -1;

     // 写共享内存数据
    mHD_Sem_P_Lock(semid);
    memcpy(p,(void *)data,sizeof(Hq_Dev_Data));
    mHD_Sem_V_Unlock(semid);

    //解除映射
      if(shmdt(p) == -1)  return -1;
      return shmid;
}

/****  写入数据到共享内存 通用*******
 *  说明：Hq_Dev_Data
 *  参数：key_t key  共享内存KEY
 *              void * data 指向需要写入的数
 *              int 写入字节数
 * 返回：成功:写入的共享内存段标识符
 *             失败：-1
 * **************************************/
int mHD_Write_Shm_ShareMemory(key_t shmkey,key_t semkey,int len,void * data)
{
    int shmid;
    int semid;
    void *p;

    shmid  = shmget(shmkey,0,0);     //获取共享内存，返回一个id
    if(shmid == -1)  return -1;

    p  = shmat(shmid,0,0); //映射共享内存，的到虚拟地址
    if(p == (void*)-1)  return  -1;

    semid = semget(semkey,0,0);  //获取信号ID
    if(semid == -1) return -1;

     // 写共享内存数据
    mHD_Sem_P_Lock(semid);
    memcpy(p,data,len);
    mHD_Sem_V_Unlock(semid);

    //解除映射
      if(shmdt(p) == -1)  return -1;
      return shmid;
}

/****  读取共享内存值到Hq_Dev_Data *******
 *  说明：读取共存内存 Hq_Dev_Data
 *  参数：key_t key  共享内存KEY
 *             Hq_Dev_Datat * data 指向读取缓存区
 * 返回：成功:读取共享内存段标识符
 *             失败：-1
 * **************************************/
int mHD_Read_Shm_ShareMemory_DevData(key_t shmkey,key_t semkey,Hq_Dev_Data * data)
{
    int shmid;
    int semid;
    void  *p;

    shmid  = shmget(shmkey,0,0);     //获取共享内存，返回一个id
    if(shmid == -1) return -1;

    p  = shmat(shmid,0,0); //映射共享内存，的到虚拟地址
    if((void*)-1 ==p)  return  -1;

    semid = semget(semkey,0,0);  //获取信号ID
    if(semid == -1) return -1;

    // 读取共享内存数据
    mHD_Sem_P_Lock(shmid);
    memcpy((void *)data,p,sizeof(Hq_Dev_Data));
    mHD_Sem_V_Unlock(shmid);

    //解除映射
      if(shmdt(p) == -1) return -1;
      return shmid;
}

/****  读取共享内存值 通用 *******
 *  说明：读取共存内存 Hq_Dev_Data
 *  参数：key_t key  共享内存KEY
 *              * data 指向读取缓存区
 *              int len  需要读取的字节数
 * 返回：成功:读取共享内存段标识符
 *             失败：-1
 * **************************************/
int mHD_Read_Shm_ShareMemory(key_t shmkey,key_t semkey,int len,void* data)
{
    int shmid;
    int semid;
    void  *p;

    shmid  = shmget(shmkey,0,0);     //获取共享内存，返回一个id
    if(shmid == -1) return -1;

    p  = shmat(shmid,0,0); //映射共享内存，的到虚拟地址
    if((void*)-1 ==p)  return  -1;

    semid = semget(semkey,0,0);  //获取信号ID
    if(semid == -1) return -1;

    // 读取共享内存数据
    mHD_Sem_P_Lock(shmid);
    memcpy(data,p,len);
    mHD_Sem_V_Unlock(shmid);

    //解除映射
      if(shmdt(p) == -1) return -1;
      return shmid;
}

/************************ 消息队列 ***********************************/
/***  创建消息队列 *******
 *返回： 成功：返回消息队列ID
 *            失败：-1
 * ****************************************************/
int mHD_Creat_Meg_MessageQueue(key_t key)
{
   int msgid;
   msgid = msgget(key,IPC_CREAT|0666);     //创建共享内存，返回一个id
   if(msgid == -1) return -1;
   else return msgid;
}

/***  删除消息队列 *******
 *  参数：int msgid  消息队列标识符
 *  返回：  成功：0
 *               失败：-1
 * ****************************************************/
int mHD_Delete_Meg_MessageQueue(key_t key)
{
    int msgid;
    msgid = mHD_Creat_Meg_MessageQueue(key);
    if(msgid == -1) return -1;
    else if(msgctl(msgid,IPC_RMID,0) == -1) return -1;
    else return 0;
}

/*** 发送消息队列 *******
 *  参数：int msgid  消息队列标识符
 *  返回：  成功：0
 *          失败：-1
 * cmd ==1 发送启动完成；
 * cmd ==2 发送数据更新；
 * ****************************************************/
int mHD_Send_Meg_MessageQueue(key_t key,int who, char *msg)
{
     int msgid;
    m_Msg_Gt msgbuf;

    msgbuf.msg_type =who;
    strcpy(msgbuf.text,msg);
    msgid = mHD_Creat_Meg_MessageQueue(key);
    if(msgid == -1) return -1;
    else if(msgsnd(msgid, &msgbuf, sizeof(msgbuf.text) , 0) < 0) return -1;
    return 0;
}
/*** 读取消息队列 *******
 *  参数：int msgid  消息队列标识符
 *  返回：  成功：返回信号类型
 *               失败：-1
 * cmd ==1 发送启动完成；
 * cmd ==2 发送数据更新；
 * ****************************************************/
int mHD_Rec_Meg_MessageQueue(key_t key,int recvtype,char *out)
{
    m_Msg_Gt msg;
    int size;
    int msgid;

    size = sizeof(msg.text);
    msgid = mHD_Creat_Meg_MessageQueue(key);
    if(msgid == -1) return -1;
    else  if(msgrcv(msgid,(void *)&msg,size,recvtype,IPC_NOWAIT) <0) return -1;
    strcpy(out,msg.text);
    return 0;
}

/*** 发送信号 *******/
void mHD_Send_Signal(int pid,int signal)
{
    kill(pid,signal);
}


//发送消息命令
int mHD_Send_Msg_Cmd(msg_cmd cmd,int type)
{
    int status;
    int pid;
    char m_setdata[64];
    pid = Run_data.Pid[0];

    if(cmd ==Msg_Link)  sprintf(m_setdata,"pid:%d - link\n",pid);
    else if(cmd ==Msg_DisLink)  sprintf(m_setdata,"pid:%d - disconnect\n",pid);
    else if(cmd ==Msg_Updata_Data)  sprintf(m_setdata,"pid:%d - updata data\n",pid);
    else if(cmd ==Msg_WritePara)  sprintf(m_setdata,"pid:%d - write parameter settings\n",pid);
    else if(cmd ==Msg_ReadPara)  sprintf(m_setdata,"pid:%d - read parameter settings\n",pid);
    else if(cmd ==Msg_StartScan)  sprintf(m_setdata,"pid:%d - start scan module\n",pid);
    else if(cmd ==Msg_StopScan)  sprintf(m_setdata,"pid:%d - stop scan module\n",pid);
    else if(cmd ==Msg_SoftInitComplete)  sprintf(m_setdata,"pid:%d - software init completed\n",pid);
    else if(cmd ==Msg_WriteParaComplete)  sprintf(m_setdata,"pid:%d - write parameter setting completed\n",pid);
    else if(cmd ==Msg_ReadParaComplete)  sprintf(m_setdata,"pid:%d - read parameter setting completed\n",pid);
    else if(cmd ==Msg_ScanStartStatus)  sprintf(m_setdata,"pid:%d - module scan start\n",pid);
    else if(cmd ==Msg_ScanStopStatus)  sprintf(m_setdata,"pid:%d - module scan stop\n",pid);
    else return -1;

    status =  mHD_Send_Meg_MessageQueue(Run_data.Msgkey,type, m_setdata);  //发送信号队列 类型type,
    //printf("msgtype = %d,  msg = %s, msgstatus = %d \n",type,m_setdata,status);
    if(status == -1)  return -1;
    return 0;
}

//解析收到的消息
//返回进程PID 值
int  mHD_Message_Analysis(char * inchar, int *pid, int *cmd)
{
    int len,i;
    char outchar[64];

    char * p_pid;      //pid:
    char * p_line;     // -
    char * p;
    const char *c_pid = "pid:";
    const char c_line = '-';
    const char c_sp = ' ';

    const char *link = "link";
    const char *dislink = "disconnect";
    const char *updat ="updata data";
    const char *writepara = "write parameter settings";
    const char *readpara = "read parameter settings";
    const char *startscan = "start scan module";
    const char *stopscan =  "stop scan module";
    const char *softinitcomplete = "software init completed";
    const char *writeparacomplete =  "write parameter setting completed";
    const char *readparacomplete =  "read parameter setting completed";
    const char *scanrunstart = "module scan start";
    const char *scanrunstop =  "module scan stop";

    if(inchar == NULL) return -1;  //输入字符串为空字符串

    p_pid  = strstr(inchar,c_pid);  //返回 pid:在inchar中的首次位置
    p_line = strchr(inchar,c_line);  //返回 '-' 在inchar中首次出现的位置

    if((p_pid ==NULL) ||(p_line ==NULL))  return -1;  //消息格式错误

    if(p_pid != inchar)
    {
        len = p_pid - inchar;
        for(i=0;i<len;i++) { if( *(inchar+i) != c_sp) {return -1;} }   //检测 pid:前面是否有除 ‘  ’ 外的其它字符
    }

    *pid = atoi(inchar+4);  //转化字符串为数字 进程PID

    len = strlen(inchar);
    for(i =1;i<len;i++)  //查找第一个非 ‘  ’ 字符位置
    {
        if( *(p_line+i) != c_sp)
        {
            p = p_line+i;
            break;
        }
    }

    outchar[0] = '\0';
    for(i=0; i<len;i++)  //取PID值
    {
        if(*(p+i) != 0x0A)  strncat(outchar,p+i,1);
        else break;
    }

    if(strcmp(link,outchar) ==0 ) *cmd = Msg_Link;
    else if(strcmp(dislink,outchar) ==0 ) *cmd = Msg_DisLink;
    else if(strcmp(updat,outchar) ==0 ) *cmd = Msg_Updata_Data;
    else if(strcmp(writepara,outchar) ==0 ) *cmd = Msg_WritePara;
    else if(strcmp(readpara,outchar) ==0 ) *cmd = Msg_ReadPara;
    else if(strcmp(startscan,outchar) ==0 ) *cmd = Msg_StartScan;
    else if(strcmp(stopscan,outchar) ==0 ) *cmd = Msg_StopScan;
    else if(strcmp(softinitcomplete,outchar) ==0 ) *cmd = Msg_SoftInitComplete;
    else if(strcmp(writeparacomplete,outchar) ==0 ) *cmd = Msg_WriteParaComplete;
    else if(strcmp(readparacomplete,outchar) ==0 ) *cmd = Msg_ReadParaComplete;
    else if(strcmp(scanrunstart,outchar) ==0 ) *cmd = Msg_ScanStartStatus;
    else if(strcmp(scanrunstop,outchar) ==0 ) *cmd = Msg_ScanStopStatus;

    return 0;
}

//接收消息命令
int mHD_Read_Msg_Cmd(int type, int *pid, int *cmd)
{
    int status;
    char m_read[128];
    status = mHD_Rec_Meg_MessageQueue(Run_data.Msgkey,type,m_read);
    if(status == -1) return -1;
    status =  mHD_Message_Analysis(m_read, pid, cmd);
    if(status ==-1) return -1;
    return 0;
}











