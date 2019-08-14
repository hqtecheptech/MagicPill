#include <stdio.h>
#include <stdint.h>

#include "local_inc/mapp.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/MPUCtrDev.h"
#include "local_inc/mapp_init.h"
#include "local_inc/DEV_CONF.h"

extern void test_shm_sem_msg(void);
extern void testgobalvalve(void);
int  main(void)
{

//    test_shm_sem_msg();
//    while(1)
//    {
//        usleep(10000);
//    }
     testgobalvalve();


//#if defined HQ_FPJ

//testdefine();

//#elif defined HQ_YXC
// testdefine();

//#elif defined HQ_HLXT
// testdefine();
// sleep(1);

//#endif
//    mHD_MPUGPIO_Test();  //Test MPU GPIO
//    mHD_Software_Init();

//     mHD_Send_Msg_Cmd(Msg_Link,wangzy);
//     sleep(1);
//     mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //读取共享内存区
//     mHD_Module_ConfigData_Init();   //写入数据值
//     mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //写入共享内存区
//     mHD_Send_Msg_Cmd(Msg_WritePara,wangzy);
//     sleep(1);
//     mHD_Send_Msg_Cmd(Msg_ReadPara,wangzy);
//     sleep(1);
//     mHD_Send_Msg_Cmd(Msg_StartScan,wangzy);

//    while(1)
//    {
//        mHD_keyboard_Cmd();
//        mHD_Readmsg_Poll(wangzy);
//        mHD_CompDev_SigToRPU();
//        Dev_data.MData[1].DOutData[6] =1;
//        Dev_data.MData[1].PulseData[6] =10;
//    }
    return 0;
}
