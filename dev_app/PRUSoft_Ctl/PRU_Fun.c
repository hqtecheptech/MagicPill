
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>


#include "PRU_Fun.h"
#include "PRU_Protocol.h"
#include "PRU_ConfigWrite.h"
#include "../Local_app/Module_GlobalData.h"


#define PIDLINK_NUMMAX   12

//启动PRU处理器
void mHD_PRU_Load(void)
{
    char *StartPru ="echo start > /sys/class/remoteproc/remoteproc3/state";
    system(StartPru);      //启动PRU处理器
}
//停止PRU处理器
void mHD_PRU_Uninstall(void)
{
    char *StopPru ="echo stop  > /sys/class/remoteproc/remoteproc3/state";
    system(StopPru);       //停止PRU处理器
}

/************* 控制命令  **********************************************
 * 名称：             mHD_RPU_Start
 * 功能：             启动PRU处理器
 * 入口参数：      enset  1=初始化运行文件波特率，0=初始化默认波特率
 * return :            错误代码02超时错误，0无错误
 * *********************************************************************************/
int mHD_RPU_Start(int enset)
{
    int status;
    uint8_t  pru_send_buf[10];

    char *StartPru ="echo start > /sys/class/remoteproc/remoteproc3/state";
    char *StopPru ="echo stop  > /sys/class/remoteproc/remoteproc3/state";

    setvbuf(stdout, (char *)NULL, _IONBF, 0);//disable stdio out buffer;

    Run_data.Pid[0] = getpid();
    Dev_data.Pru.RPid[0] = Run_data.Pid[0];                 //进程PID写入Dev_data.

    //Step1: 启动PRU处理器
    system(StopPru);       //停止PRU处理器
    usleep(300000);        //延迟100ms
    system(StartPru);      //启动PRU处理器
    usleep(300000);        //延迟100ms

    PRU_Rpmsg.Rpmsg_fd= open("/dev/rpmsg_pru30", O_RDWR);
    if(PRU_Rpmsg.Rpmsg_fd<=0)
    {
       printf("open %s device error!\n", "/dev/rpmsg_pru30");
        return -1;
   }

    //Step2:建立与PRU处理器的连接 
    pru_send_buf[0] = PRU_CMD_INIT;  //建立与PRU处理器的连接
    pru_send_buf[1] = 0x00;  //保留
    if(enset ==1)
    {
        pru_send_buf[2] = 3;
        pru_send_buf[3] = 3;
    } else
    {
        pru_send_buf[2] = 0x03;
        pru_send_buf[3] = 0x03;
    }
    do
    {
        status = mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf);
    } while(status !=0);
    usleep(500000);        //延迟100ms
    return 0;
}


/****** 应答命令 无回复******/
void  mHD_DevData_ReplyAndInit(void)
{
   uint8_t  pru_send_buf[10];
   pru_send_buf[0] = PUR_CMD_REPLY;  //建立与PRU处理器的连接
   pru_send_buf[1] = 0x00;  //保留
   pru_send_buf[2] = 0x00;        //PRU处理器波特率
   pru_send_buf[3] = 0x00;       //Module 波特率
   mHD_Pru_Protocol_ReplyInit(PRU_CMD_CTR,pru_send_buf);
}

//Dev_Data 参数设置
//返回 -1 设置命令错误, 0设置成功，>0 错误码设置失败
static int mHD_DevData_WritePara(void)
{
    uint8_t  status;
    int modnum;
    Mod_set modpara;
    int i,j;
    uint8_t  pru_send_buf[10];

    if(Dev_data.Pru.MEndBit <32) modnum = Dev_data.Pru.MEndBit;
    else return -1;
    pru_send_buf[0] = PRU_CMD_SET;  //建立与PRU处理器的连接
    pru_send_buf[1] = 0x00;  //保留
    pru_send_buf[2] = Dev_data.Pru.RBaud;        //PRU处理器波特率
    pru_send_buf[3] = Dev_data.Pru.MBaud;       //Module 波特率
    if(mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf) ==0)
    {
        usleep(500000);  //延时300ms等待PRU完成参数设置命令

        for(i=1;i<modnum+1;i++)   //设置各模块参数
        {
            modpara.bitnum = Dev_data.Mconfig[i].BitNum;       //bitnumber
            M_SetData[i].bitnum =  Dev_data.Mconfig[i].BitNum;       //bitnumberMsg_Updata_Data
            M_SetData[i].name = Dev_data.Mconfig[i].Name;            //module name
            modpara.ver = Dev_data.Mconfig[i].Ver;                     //module ver
            modpara.name = Dev_data.Mconfig[i].Name;            //module name
            M_SetSysData.Modbaud = Dev_data.Pru.MBaud;     //module rate
            if(modpara.name == 0x7122)
            {
                for(j=0;j<4;j++) modpara.setdata[j] = Dev_data.Mconfig[i].DInFilterTime[j] ; //普通DI输入通道滤波时间设置
            }
            else if(modpara.name == 0x8012)
            {
                for(j=0;j<4;j++)
                {
                    modpara.setdata[2*j] = Dev_data.Mconfig[i].AOutVReg[j]; //模拟量输出电压范围设置
                    modpara.setdata[2*j+1] = Dev_data.Mconfig[i].AOutAReg[j]; //模拟量输出电流范围设置
                }
                for(j=0;j<12;j++)  modpara.setdata[8+j] = Dev_data.Mconfig[i].AInReg[j]; //模拟量输入范围设置
            }
            else if(modpara.name == 0x7313)
            {
                for(j=0;j<8;j++) modpara.setdata[j] = Dev_data.Mconfig[i].DOutFunSet[j];               //DO输出功能设置
                for(j=0;j<12;j++) modpara.setdata[8+j] = Dev_data.Mconfig[i].DInFunSet[j];      //DI输入功能设置
                modpara.setdata[20] = Dev_data.Mconfig[i].DInFilterTime[0];                               //普通DI输入通道滤波时间设置
                for(j=0;j<6;j++) modpara.esturn[j] = Dev_data.Mconfig[i].EnSCPulseSet[j];
            }
            else if(modpara.name == 0x7314)
            {
                for(j=0;j<8;j++) modpara.setdata[j] = Dev_data.Mconfig[i].DOutFunSet[j];       //DO输出功能设置
                for(j=0;j<6;j++) modpara.setdata[8+j] = Dev_data.Mconfig[i].DInFunSet[j];      //DI输入功能设置
                for(j=0;j<2;j++) modpara.setdata[14+j] = Dev_data.Mconfig[i].SSIFunSet[j];     //SSI输入功能设置
                modpara.setdata[16] = Dev_data.Mconfig[i].DInFilterTime[0];                               //普通DI输入通道滤波时间设置
                for(j=0;j<3;j++) modpara.esturn[j] = Dev_data.Mconfig[i].EnSCPulseSet[j];
            }
            status = mHD_Pru_Protocol_SetData(modpara);
            Dev_data.Mconfig[i].ErrCode = status;
            usleep(500000);  //延时300ms等待PRU完成参数设置命令
        }
     }else return -1;
    return 0;
}

//配置文件参数设置
//返回 -1 设置错误，返回其他写入数量
int mHD_ConfigFile_WritePara(void)
{
    uint8_t modnum =0;
    uint8_t i;
    int status;
    uint8_t  pru_send_buf[10];

    modnum =mHD_MPU_Read_SetFile();     //读取设置文件
    if(modnum ==0) return -1;             //读取文件错误

    pru_send_buf[0] = PRU_CMD_SET ;  //设置模块为写入参赛设在模式
    pru_send_buf[1] = 0x00;  //保留
    pru_send_buf[2] = M_SetSysData.Devbaud;        //PRU处理器波特率
    pru_send_buf[3] = M_SetSysData.Modbaud;       //Module 波特率

    if(mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf) ==0)
    {
        usleep(50000);  //延时300ms等待PRU完成参数设置命令
        for(i=1;i<modnum+1;i++)
        {
            status = mHD_Pru_Protocol_SetData(M_SetData[i]);
            if(status !=0) return -1;
            usleep(500000);  //延时300ms
        }
        return modnum;
    } else return -1;
}

//Dev 配置信息设置模块参数
static int m_Dev_Setpara_cmd(void)
{
    int status;
    status = mHD_DevData_WritePara(); //Dev参数设置
    if(status ==0)
    {
        Dev_data.Pru.MWSetStatus =1; //参数设置成功
        Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态

        HqRpuRun_data.setmodule =0;  //不进行参数设置 暂未使用
        HqRpuRun_data.setstatus = 1;    //已完成参赛设置

        HqRpuRun_data.Devbaud = Dev_data.Pru.RBaud;   //PRU 波特率
        HqRpuRun_data.Modbaud =Dev_data.Pru.MBaud; //Module 波特率
        HqRpuRun_data.MStartBit =1;   //开始读取模块的位号
        HqRpuRun_data.MEndBit = Dev_data.Pru.MEndBit ;  //结束读取模块的位号
        HqRpuRun_data.back = 8;     //暂未使用
        HqRpuRun_data.user1 = 31;     //最大连接用户 暂未使用
        mHD_Write_Run_Data(); //写入系统运行配置二进制文
    }
    return 0;

}

//File 配置文件设置模块参数
static int m_File_Setpara_cmd(void)
{
    int num;
    num = mHD_ConfigFile_WritePara();  //Configuration file 参数设置
    if(num >0)
    {
        Run_data.ModuleNum = num;  //模块数量
        Dev_data.Pru.MEndBit = num;  //模块数量
        Dev_data.Pru.MRSetStatus =1; //参数设置成功成功
        Dev_data.Pru.RBaud = M_SetSysData.Devbaud;   //PRU处理器波特率
        Dev_data.Pru.MBaud = M_SetSysData.Modbaud;       //Module 波特率
        Dev_data.Pru.MSetSource =0x0;   //配置文件设置参赛
        Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态

        HqRpuRun_data.setmodule =0;  //不进行参数设置 暂未使用
        HqRpuRun_data.setstatus = 1;    //已完成参赛设置
        HqRpuRun_data.Devbaud =M_SetSysData.Devbaud;   //PRU 波特率
        HqRpuRun_data.Modbaud =M_SetSysData.Modbaud; //Module 波特率
        HqRpuRun_data.MStartBit =1;   //开始读取模块的位号
        HqRpuRun_data.MEndBit = num ;  //结束读取模块的位号
        HqRpuRun_data.back = 8;     //暂未使用
        HqRpuRun_data.user1 = 31;     //最大连接用户 暂未使用
        mHD_Write_Run_Data();           //写入系统运行配置二进制文件
    }
    return num;
}

//读取模块设置参数
int m_Readpara_cmd(void)
{
    uint8_t  pru_send_buf[10];
    int men;
    //int status;
    //uint8_t bitnum;

    //发送读取设置参赛到PRU处理器
    pru_send_buf[0] = PRU_CMD_READ;  //发送读取模块命令
    pru_send_buf[1] = 0x00;  //保留
    pru_send_buf[2] = 0x01;          //从BitNum 1 开始读取参数
    pru_send_buf[3] = Run_data.ModuleNum;  //读取结束模块
    sleep(1);
    //读取设置参数
    if(mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf) ==0)
    {
        men = 1;
        do{
                //status =   mHD_Pru_Protocol_ReadSetData(&bitnum);
               // if(Run_data.ModuleNum == bitnum)  men =0;
        } while (men);

//        if(status ==0)
//        {
//            Dev_data.Pru.MRSetStatus = 1;
//       }
    }
    return 0;
}
//开始扫描模块
int m_StartModule_Scan(void)
{
    uint8_t  pru_send_buf[10];

    pru_send_buf[0] = PRU_CMD_SCAN_CTR;  //发送读取模块命令
    pru_send_buf[1] = 0x00;  //保留
    pru_send_buf[2] = 0x01;          //从BitNum 1 开始读取参数
    pru_send_buf[3] = Run_data.ModuleNum;  //读取结束模块

    if(mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf) ==0)
    {
        Dev_data.Pru.MScanStatus = 1;
        Run_data.PollStatus =1;
    }
    return 0;
}

//停止扫描模块
int m_StopModule_Scan(void)
{
    uint8_t  pru_send_buf[10];
    if(Dev_data.Pru.MScanStatus == 0) return 0;
    pru_send_buf[0] = PUR_CMD_SCAN_STOP ;  //发送读取模块命令
    pru_send_buf[1] = 0x00;  //保留
    pru_send_buf[2] = 0x00;
    pru_send_buf[3] = 0x00;
    Run_data.PollStatus =0;
    if(mHD_Pru_Protocol_SetCmd(PRU_CMD_CTR,pru_send_buf) ==0)
    {
        Dev_data.Pru.MScanStatus = 0;
        Run_data.PollStatus =0;
    }
    return 0;
}

//发送运行数据到PRU处理器
//返回0 无错误,返回-1 有错误
static int m_UpdateModule_Data(void)
{
//   uint8_t i;
//   //int status;
//   uint8_t len;
//   //uint16_t name;
//   int mreturn = 0;

//   len = Dev_data.Pru.MEndBit;

//   for(i=1;i<len+1;i++)
//   {
////       if( Dev_data.Pru.MWData[i] ==1)
////       {
////           Dev_data.Pru.MWData[i]  =0;
//           //name =Dev_data.Mconfig[i].Name;
//           //status = mHD_Pru_Protocol_WriteRunData(i,name);
//           //if(status !=0) mreturn = -1;
//       }
//       usleep(100);
//   }
//   Dev_data.Pru.RStatus =0;
   return 0;
}
//发送运行数据到PRU处理器 但个数据
//返回0 无错误,返回-1 有错误
int m_UpdateModule_SingeData(int num,int name)
{
   //if(mHD_Pru_Protocol_WriteRunData(num,name) !=0)  return -1;
    int i;
    i= num+name;
    return i;
}

//等待消息命令 执行任务
//MSG写入设置参数
// 读取消息--->处理消息内容
int mHD_ProcessMsg_CMD(void)
{
    //uint8_t msg_status;
    int i;
    int mupdatestatus;
    int msgtype;
    msgtype = -1;

    for(i=7;i<PIDLINK_NUMMAX+1;i++)
    {
        //msg_status = mHD_Read_MessageQueue(i,&msgtype);    //读取消息
        //if(msg_status != 1)  continue;


        switch (Run_data.Msg_Update_CMD)
        {
        case MsgUpdate_DateChange: //仅数据改变
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
            Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
            if((Run_data.Pid[i-6] !=0)&&(Run_data.PidLink[i-6] ==1))
            {
                 mHD_Send_Signal(Run_data.Pid[i-6],SIGFRPU);
             }
            mHD_Send_Msg_Cmd(Msg_Updata_Data,msgtype);   //返回数据更新消息给发送消息的应用程序
           //mHD_Send_MsgCmd_App(Msg_Updata_Data);                     //发送PRU数据更新消息到APP

#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_DateChange! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;
       case MsgUpdate_WritePara: //写入参数设置
                m_StopModule_Scan(); //停止扫描
                Run_data.ModuleNum = Dev_data.Pru.MEndBit;
                Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
                switch (Dev_data.Pru.MSetSource)
                {
                case 1:
                    m_Dev_Setpara_cmd();                   
                    m_Readpara_cmd();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_WritePara Form Dev_data! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
                    break;
                case 0:
                    m_File_Setpara_cmd();
                    m_Readpara_cmd();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_WritePara Form File! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
                 break;
                }
            break;
        case MsgUpdate_ReadPara:
                m_StopModule_Scan(); //停止扫描
                Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
                Run_data.ModuleNum = Dev_data.Pru.MEndBit;
                m_Readpara_cmd();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_ReadPara! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;
        case MsgUpdate_StartScan:  //轮询读写模块数据
                Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
                Run_data.ModuleNum = Dev_data.Pru.MEndBit;
                m_StartModule_Scan();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_StartScan! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;
        case MsgUpdate_StopScan:
                Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
                m_StopModule_Scan();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_StopScan! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;

        case MsgUpdate_UpdataData:
           Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
           mupdatestatus =  m_UpdateModule_Data();
#ifdef  mHD_Debug
             printf("Process msg: MsgUpdate_UpdataData! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;
        case MsgUpdate_None:
            Run_data.Msg_Update_CMD =MsgUpdate_None; //复位消息状态
#ifdef  mHD_Debug
             printf("Process msg: msg Error! type = %d\n",msgtype); //Debug  发送接收到的消息
#endif
            break;
        }
    }
    return mupdatestatus;
}

int mHD_SigUpdate_Send(void)
{
    return m_UpdateModule_Data();
}

//RunSet1 ==0  设置参数
//0 成功，-1失败
int mHD_WritePare_RunSet(void)  //配置文件写入参赛
{
   uint8_t readstatus =1;
   int modnum =0;

   modnum = m_File_Setpara_cmd();  //按配置文件设置模块参赛
   if(modnum <=0)  return -1;
   usleep(500000);
   readstatus = m_Readpara_cmd();   //读取设置参数
   if(readstatus <=0)  return -1;

    //轮询读写模块数据
    //m_StartModule_Scan();
    return 0;
}

/************* 初始化RPU处理器任务  **********************************************
 * 名称：             mHD_RPU_Init
 * 功能：             初始化PRU处理器
 * 入口参数：     无
 * return :            错误代码02超时错误，0无错误
 * *********************************************************************************/
int mHD_RPU_Init(void)
{
    int rdataen = -1;
    int re = -1;
    //radrun =  mHD_Read_Run_Data();       //读取二进制配置文件
//    if(radrun ==0)  //读参数成功 赋值变量
//    {
//        Dev_data.Pru.MEndBit = HqRpuRun_data.MEndBit;  //模块数量
//        Dev_data.Pru.MRSetStatus =1; //参数设置成功成功
//        Dev_data.Pru.RBaud = HqRpuRun_data.Devbaud;         //PRU处理器波特率
//        Dev_data.Pru.MBaud = HqRpuRun_data.Modbaud;     //Module 波特率
//        Run_data.ModuleNum = HqRpuRun_data.MEndBit;  //读取结束模块
//        rdataen =1 ; //读取运行配置成功
//    }


    Run_data.ModuleNum = Dev_data.Pru.MEndBit;  //读取结束模块
    mHD_RPU_Start(rdataen);    //启动PRU处理器,等待直到启动完成，建立通信
    //RunSet_1 ON 位置 设置参数 ---> 读取设置参数 ---> 设置为模块扫描模式
    if(mHD_Read_GPIO(BUTRUN_RPU_1) ==0)  //RUNSET_1 开关 配置文件设置参数
//    {
//         if(m_File_Setpara_cmd() >0)  //按配置文件设置模块参赛
//         {
//             re =0;
//             usleep(500000);
//             m_Readpara_cmd();   //读取设置参数
//             usleep(500000);
//             m_StartModule_Scan();  //启动模块自动扫描
//              usleep(500000);
//         }
//    } else if( mHD_Read_GPIO(BUTRUN_MPU_2) ==0)  //RUNSET_2 开个 参数设置参数
//    {
//        mHD_Module_ConfigData_Init();
//        if(m_Dev_Setpara_cmd() >0)  //配置文件设置参数
//        {
//            re =0;
//            usleep(500000);
//            m_Readpara_cmd();   //读取设置参数
//             usleep(500000);
//            m_StartModule_Scan();  //启动模块自动扫描
//            usleep(500000);
//        }
//    } else if (rdataen == 1) //参数已配置完成,
//    {
//        re =0;
//        usleep(500000);
//        m_Readpara_cmd();   //读取设置参数
//        usleep(500000);
//        m_StartModule_Scan();  //启动模块自动扫描
//        usleep(500000);
//    }
    m_Readpara_cmd();   //读取设置参数
    usleep(500000);
    m_StartModule_Scan();  //启动模块自动扫描
    usleep(500000);
    return re;
}















