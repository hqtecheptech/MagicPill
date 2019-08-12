#include "local_inc/mapp_init.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/CommFun.h"
#include "local_inc/keyboard_input.h"
#include"local_inc/MPUCtrDev.h"

//模块设置参数初始化信息
void mHD_Module_ConfigData_Init(void)
{
    int i;
    // 初始化PRU处理器
    Dev_data.Pru.RBaud = 0x08;
    Dev_data.Pru.MEndBit = 0x04;
    Dev_data.Pru.MBaud = 0x08;
    Dev_data.Pru.MSetSource =0x01;  //参数设置源

    //Module 7313 Bit1 设置参数
    Dev_data.Mconfig[1].Name = 0x7313;
    Dev_data.Mconfig[1].BitNum = 0x01;
    Dev_data.Mconfig[1].Ver= 0x0A;
    Dev_data.Mconfig[1].Baud = 0x08;

    for(i=0;i<6;i++) Dev_data.Mconfig[1].DOutFunSet[i] = 0x01;       //0-5 DO输出功能
    for(i=0;i<2;i++) Dev_data.Mconfig[1].DOutFunSet[6+i] = 0x02;   //6-7 PWM功能

    for(i=0;i<10;i++) Dev_data.Mconfig[1].DInFunSet[i] = 0x01;       //0-9通道DI 输入功能
    for(i=0;i<2;i++) Dev_data.Mconfig[1].DInFunSet[10+i] = 0x02;       //10-11通道Encoder功能
    Dev_data.Mconfig[1].DInFilterTime[0] = 0x01;         //单功能DI输入滤波时间设置

    for(i=0;i<6;i++) Dev_data.Mconfig[1].EnSCPulseSet[i] = 1024;  //旋转编码器脉冲值设置

    //Module 7314 Bit1 设置参数
    Dev_data.Mconfig[2].Name = 0x7314;
    Dev_data.Mconfig[2].BitNum = 0x02;
    Dev_data.Mconfig[2].Ver= 0x0A;
    Dev_data.Mconfig[2].Baud = 0x08;

    for(i=0;i<6;i++) Dev_data.Mconfig[2].DOutFunSet[i] = 0x01;       //0-5 DO输出功能
    for(i=0;i<2;i++) Dev_data.Mconfig[2].DOutFunSet[6+i] = 0x02;   //6-7 PWM功能

    for(i=0;i<5;i++) Dev_data.Mconfig[2].DInFunSet[i] = 0x01;       //0-9通道DI 输入功能
    for(i=0;i<1;i++) Dev_data.Mconfig[2].DInFunSet[5+i] = 0x02;       //10-11通道Encoder功能
    for(i=0;i<2;i++) Dev_data.Mconfig[2].SSIFunSet[i] = 0x01;       //SSI功能为绝对值编码器
    Dev_data.Mconfig[2].DInFilterTime[0] = 0x01;         //单功能DI输入滤波时间设置

    for(i=0;i<3;i++) Dev_data.Mconfig[2].EnSCPulseSet[i] = 1024;  //旋转编码器脉冲值设置

    //Module 7122 Bit1 设置参数
    Dev_data.Mconfig[3].Name = 0x7122;
    Dev_data.Mconfig[3].BitNum = 0x03;
    Dev_data.Mconfig[3].Ver= 0x0A;
    Dev_data.Mconfig[3].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[3].DInFilterTime[i] = 0x01;

    //Module 8012 Bit1 设置参数
    Dev_data.Mconfig[4].Name = 0x8012;
    Dev_data.Mconfig[4].BitNum = 0x04;
    Dev_data.Mconfig[4].Ver= 0x0A;
    Dev_data.Mconfig[4].Baud = 0x08;
    for(i=0;i<4;i++) Dev_data.Mconfig[4].AOutAReg[i] = 0x0B;
    for(i=0;i<4;i++) Dev_data.Mconfig[4].AOutVReg[i] = 0x01;
    for(i=0;i<12;i++) Dev_data.Mconfig[4].AInReg[i] = 0x01;
}

//#define MPU_EIN_MAX              3
//#define MPU_DIN_MAX             6
//#define MPU_DOUT_MAX         8
//MPUIO设置参数初始化信息
void mHD_MPU_ConfigData_Init(void)
{
    char m_mode[16] = {'\0'};
    char m_pull[16] = {'\0'};
    int i;

    //设置值修改代码
    for(i=0;i<MPU_DOUT_MAX;i++)  {Mpu_data.config.DOutFunSet[i] = 1; Mpu_data.config.DOutPullSet[i] =0;}//1=DO,2=PWM 0=downpull,1=pupull
    for(i=0;i<MPU_DIN_MAX;i++)  {Mpu_data.config.DInFunSet[i] = 1; Mpu_data.config.DInPullSet[i] = 0;} //1=DI,2=Encoder 0=downpull,1=pupull
    for(i=0;i<MPU_EIN_MAX;i++)  Mpu_data.config.EnSCPulseSet[i] = 1024;

    //写入设置值代码
    for(i=0;i<MPU_DOUT_MAX;i++)
    {
        m_mode[0] = '\0';   m_pull[0] = '\0';
        if(Mpu_data.config.DOutFunSet[i] ==1)  strcat(m_mode,MPU_MODE_GPIO);
        else strcat(m_mode,MPU_MODE_PULSE);
        if(Mpu_data.config.DOutPullSet[i] ==1)  strcat(m_pull,MPU_PULL_UP);
        else strcat(m_pull,MPU_PULL_DOWN);
        mHD_Write_MPUGPIO_Config(i,MPU_GPIO_OUT,m_mode,m_pull);
        usleep(50000);
    }

    for(i=0;i<MPU_DIN_MAX;i++)
    {
        m_mode[0] = '\0';   m_pull[0] = '\0';
        if(Mpu_data.config.DInFunSet[i] ==1)  strcat(m_mode,MPU_MODE_GPIO);
        else strcat(m_mode,MPU_MODE_ENCODER);
        if(Mpu_data.config.DInPullSet[i] ==1)  strcat(m_pull,MPU_PULL_UP);
        else strcat(m_pull,MPU_PULL_DOWN);
        mHD_Write_MPUGPIO_Config(i,MPU_GPIO_IN,m_mode,m_pull);
        usleep(50000);
    }
}

#define TESTLEN   64

typedef struct {
    int m_shm[TESTLEN];
} testshm_data;



#define LCMAX  3
#define PDMAX  4

typedef struct {
    uint8_t * lxST;
    uint8_t * lxstatus;
    uint16_t * lxspeedstatus;
    uint16_t * lxsetspeed;
} LC_data;

typedef struct{
    int m_[TESTLEN];
} PD_data;

typedef struct {
    int m_[TESTLEN];
} HL_data;

typedef struct {
    LC_data lc[LCMAX];
    PD_data pd[PDMAX];
    HL_data hlj;
} HLXT_data;


HLXT_data hlxt;

void testgobalvalve(void)
{
     hlxt.lc[0].lxST = &Dev_data.MData[1].DInData[1];
    *hlxt.lc[0].lxST =52;

    printf("hlxt.lc[0].ixst *p =  %d\n",(int)hlxt.lc[0].lxST);

    printf("Dev_data.MData[1].DInData[1] *p =  %d\n",(int)&Dev_data.MData[1].DInData[1]);

    printf("hlxt.lc[0].ixst value =  %d\n",*hlxt.lc[0].lxST);

    printf("Dev_data.MData[1].DInData[1]  =  %d\n",Dev_data.MData[1].DInData[1]);

}
















void test_shm_sem_msg(void)
{
    key_t  shmkey,semkey,msgkey;
    testshm_data mshm;
    int i;
    char outp[64][64];

    char *ctr_shmkey ="/home/hqtech/ctrsoft/shmkey";   //共享内存目录
    char *ctr_semkey ="/home/hqtech/ctrsoft/semkey";   //消息队列目录
    char *ctr_msgkey ="/home/hqtech/ctrsoft/msgkey";   //消息队列目录

    shmkey = ftok(ctr_shmkey,97);  //共享内存键值
    semkey = ftok(ctr_semkey,98);  //信号键值
    msgkey = ftok(ctr_msgkey,99);  //消息队列键值


    int shmstatus = mHD_Creat_Shm_ShareMemory(shmkey,sizeof(testshm_data));   //创建共享内存
    int semstatus = mHD_Create_Semaphore(semkey);     //创建信号量
    int msgstatus =  mHD_Creat_Meg_MessageQueue(msgkey);        //创建消息队列

    for(i=0;i<10;i++) mHD_Send_Meg_MessageQueue(msgkey,1, "m_setdata 123");
    sleep (1);
    for(i=0;i<10;i++) mHD_Rec_Meg_MessageQueue(msgkey,1, outp[i]);
    sleep (1);
}


















