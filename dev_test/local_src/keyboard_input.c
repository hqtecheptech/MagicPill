
#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>


#include "local_inc/keyboard_input.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/CommFun.h"

#define PRUMSG_MAX   9

//首地址  prumsg[0], 首值  *(prumsg[0]+0) == m *(prumsg[0]+4) ==l
//首地址  prumsg[3], 首值  *(prumsg[3]+0) == m *(prumsg[3]+4) ==w
const char *prumsg[32] = {  "msg_link",            "msg_dislink",        "msg_update",       "msg_writepara",  "msg_readpara",
                                                 "msg_startscan",  "msg_stopscan",    "msg_init",            "msg_all"};
const char *keycmd_Set0[2] ={"write","read"};
const msg_cmd  prucmdmsg[8] ={Msg_Link,Msg_DisLink, Msg_Updata_Data, Msg_WritePara,  Msg_ReadPara, Msg_StartScan, Msg_StopScan,Msg_SoftInitComplete};

static void mHD_printf_err(void)
{
    printf("Input command error! Please check!\n");
}

static void mHD_printf_complete(void)
{
    printf("complete!!!\n");
}

//用来解析msg read 信息
static void m_readmsg(int ch,int msgtype)
{
    mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //把共享内存中的数据更新到本地变量Dev_data
    switch (ch) {
    case 0:  //PRU LINK
        if((Dev_data.Pru.RPid[msgtype-6] !=0) &&(Dev_data.Pru.RPidLink[msgtype-6] ==1))  {printf("PRU Connection Status.\n"); return;}
        else { printf("PRU Disconnected state!\n"); return;}
        break;
    case 3: //Module 参数设置状态
        if(Dev_data.Pru.MWSetStatus ==1) printf("Module Parameter setting complete\n");
        else printf("Module Parameter setting is not completed!\n");
        break;
    case 4: //Module 参数读取状态
        if(Dev_data.Pru.MRSetStatus ==1 ) printf("Module Parameter reading  complete\n");
        else printf("Module Parameter reading  is not completed!\n");
        break;
    case 5: //Module 扫描状态
        if(Dev_data.Pru.MScanStatus ==1 ) printf("Module Scanning has started\n");
        else printf("Module Scanning has not started!\n");
        break;
    case 6:
        if(Dev_data.Pru.MScanStatus ==1 ) printf("Module Scanning has started\n");
        else printf("Module Scanning has not started!\n");
        break;
    case 7:
        if(Dev_data.Pru.MStatus ==1 )  printf("PRU Software is ready\n");
        else printf("PRU Software not ready!\n");
        break;
     default: //对1,2 >7的处理
        mHD_printf_err();
        break;
    }
}

//用来解析msg read all 信息
static void m_readmsgall(int msgtype)
{
    mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //把共享内存中的数据更新到本地变量Dev_data
    if((Dev_data.Pru.RPid[msgtype-6] !=0) &&(Dev_data.Pru.RPidLink[msgtype-6] ==1))  {printf("PRU Connection Status.\n"); }
    else { printf("PRU Disconnected state!\n"); }
    if(Dev_data.Pru.MWSetStatus ==1) printf("Module Parameter setting complete\n");
    else printf("Module Parameter setting is not completed!\n");
    if(Dev_data.Pru.MRSetStatus ==1 ) printf("Module Parameter reading  complete\n");
    else printf("Module Parameter reading  is not completed!\n");
    if(Dev_data.Pru.MScanStatus ==1 ) printf("Module Scanning has started\n");
    else printf("Module Scanning has not started!\n");
    if(Dev_data.Pru.MStatus ==1 )  printf("PRU Software is ready\n");
    else printf("PRU Software not ready!\n");
}

extern  void mHD_Module_ConfigData_Init(void); //模块设置参数初始化信息

//处理键盘输入的pru msg 信息
void keyboard_prumsg(char (*mtf)[128],int msgtype)
{
    int i;

  if((mstrcmp(mtf[4],prumsg[0]) ==0)&&(mstrcmp(mtf[0],keycmd_Set0[0])==0))  //msg_link 命令
   {
       mHD_Send_Msg_Cmd(Msg_Link,msgtype);
       mHD_printf_complete();
       return;
   }

   if(((Dev_data.Pru.RPid[msgtype-6]) ==0) ||(Dev_data.Pru.RPidLink[msgtype-6] ==0))
   {
        printf("Not connected to RPU,Please connect to the RPU first!!!\n");
        return;
   }

    for(i=0;i<PRUMSG_MAX;i++)
    {
        if(mstrcmp(&mtf[4][0],prumsg[i]) ==0)
        {
            if(mstrcmp((const char*)&mtf[0][0],keycmd_Set0[0] ) ==0)  //if 是 wirte
            {
                if(i<7)
                {
                    if((i==3)||(i==4)||(i==5))
                    {
                        mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //读取共享内存区
                        mHD_Module_ConfigData_Init();   //写入数据值
                        mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);  //写入共享内存区
                    }
                    mHD_Send_Msg_Cmd(prucmdmsg[i],msgtype);
                    mHD_printf_complete();
                } else  mHD_printf_err();
                return;
            }
            else // 是read
            {
               if(i<8) m_readmsg(i,msgtype);
               else m_readmsgall(msgtype);
            }
        }
    }
}


//键盘输入 pru msg 命令
void keyboard_prumsg_poll(char mtfg[20][50],int msgtype)
{
    if(strcmp(&mtfg[4][0],"msg_link") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0) { mHD_Send_Msg_Cmd(Msg_Link,msgtype); mHD_printf_complete();return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey , Run_data.Semkey ,&Dev_data);
              if((Dev_data.Pru.RPid[1] !=0)&&(Dev_data.Pru.RPidLink[1] ==1)) {printf("PRU Connection Status.\n"); return;}
              else { printf("PRU Disconnected state!\n"); return;}
          }
    }
   else if(strcmp(&mtfg[4][0],"msg_dislink") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0) { mHD_Send_Msg_Cmd(Msg_DisLink,msgtype); mHD_printf_complete();return;}
        else {mHD_printf_err(); return;}
    }
   else if(strcmp(&mtfg[4][0],"msg_update") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
            mHD_Send_Msg_Cmd(Msg_Updata_Data, msgtype); mHD_printf_complete();return;
        }
        else {mHD_printf_err(); return;}
    }
    else if(strcmp(&mtfg[4][0],"msg_writepara") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
            mHD_Send_Msg_Cmd(Msg_WritePara, msgtype); mHD_printf_complete();return;
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
             mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
             if(Dev_data.Pru.MWSetStatus ==1 ) printf("Module Parameter setting complete\n");
             else printf("Module Parameter setting is not completed!\n");
        }
    }
    else if(strcmp(&mtfg[4][0],"msg_readpara") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
            mHD_Send_Msg_Cmd(Msg_ReadPara,msgtype); mHD_printf_complete();return;
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
             mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
             if(Dev_data.Pru.MRSetStatus ==1 ) printf("Module Parameter reading  complete\n");
             else printf("Module Parameter reading  is not completed!\n");
        }
    }
    else if(strcmp(&mtfg[4][0],"msg_startscan") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            mHD_Write_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
            mHD_Send_Msg_Cmd(Msg_StartScan, msgtype); mHD_printf_complete();return;
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
             mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
             if(Dev_data.Pru.MScanStatus ==1 ) printf("Module Scanning has started\n");
             else printf("Module Scanning has not started!\n");
        }
    }
    else if(strcmp(&mtfg[4][0],"msg_stopscan") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0) {mHD_Send_Msg_Cmd(Msg_StopScan, msgtype); mHD_printf_complete();return;}
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
             mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);
             if(Dev_data.Pru.MScanStatus ==1 ) printf("Module Scanning has started\n");
             else printf("Module Scanning has not started!\n");
        }
    }
    else if(strcmp(&mtfg[4][0],"msg_init") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0) {mHD_printf_err();return;}
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
             mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey ,&Dev_data);
             if(Dev_data.Pru.MStatus ==1 )  printf("PRU Software is ready\n");
             else printf("PRU Software not ready!\n");
        }
    }
    else {mHD_printf_err();return;}
}

//键盘输入 pru msg 命令
void keyboard_prudata(char (*mtfg)[128])
{
    int i;
    int tempdata;
    int tempdata1;
    if(strcmp(&mtfg[4][0],"rshmid") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RShmid = %d.\n", Dev_data.Pru.RShmid); return;}
    }
    else if(strcmp(&mtfg[4][0],"rsemid") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
        else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RSemid = %d.\n", Dev_data.Pru.RSemid); return;}
   }
    else if(strcmp(&mtfg[4][0],"rmsgid") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RMsgid = %d.\n", Dev_data.Pru.RMsgid); return;}
    }
    else if(strcmp(&mtfg[4][0],"rshmkey") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RShmkey = %d.\n", Dev_data.Pru.RShmkey); return;}
    }
    else if(strcmp(&mtfg[4][0],"rsemkey") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RSemkey = %d.\n", Dev_data.Pru.RSemkey); return;}
    }
    else if(strcmp(&mtfg[4][0],"rmsgkey") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RMsgkey = %d.\n", Dev_data.Pru.RMsgkey); return;}
    }
    else if(strcmp(&mtfg[4][0],"rpid") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Data Rpid = { ");
             for(i=0;i<6;i++)
             {
                 if(i==5 ) printf("%d}\n", Dev_data.Pru.RPid[i]);
                 else printf("%d, ", Dev_data.Pru.RPid[i]);
             }
             return;
         }
    }
    else if(strcmp(&mtfg[4][0],"rpidlink") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Data RpidLink = {");
             for(i=0;i<6;i++)
             {
                 if(i==5) printf("%d}\n", Dev_data.Pru.RPidLink[i]);
                 else printf("%d, ", Dev_data.Pru.RPidLink[i]);
             }
             return;
         }
    }
    else if(strcmp(&mtfg[4][0],"rstatus") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             if((strcmp(&mtfg[5][0],"1") !=0) &&(strcmp(&mtfg[5][0],"0") !=0) )  {mHD_printf_err(); return;}
             else {Dev_data.Pru.RStatus = atoi(&mtfg[5][0]); printf("PRU Data RStatus = %d\n",Dev_data.Pru.RStatus); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RStatus = %d\n",Dev_data.Pru.RStatus); return;}
    }
    else if(strcmp(&mtfg[4][0],"rbaud") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tempdata = atoi(&mtfg[5][0]);
             if((tempdata>=0) &&(tempdata <=13 ) )  {Dev_data.Pru.RBaud = tempdata;printf("PRU Data RBaud = %d\n",Dev_data.Pru.RBaud); return;}
             else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data RBaud = %d\n",Dev_data.Pru.RBaud); return;}
    }
    else if(strcmp(&mtfg[4][0],"mstatus") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MStatus = %d.\n", Dev_data.Pru.MStatus); return;}
    }
    else if(strcmp(&mtfg[4][0],"mwsetstatus") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MWSetStatus = %d.\n", Dev_data.Pru.MWSetStatus); return;}
    }
    else if(strcmp(&mtfg[4][0],"mrsetstatus") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MRSetStatus = %d.\n", Dev_data.Pru.MRSetStatus); return;}
    }
    else if(strcmp(&mtfg[4][0],"mscanstatus") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MScanStatus = %d.\n", Dev_data.Pru.MScanStatus); return;}
    }
    else if(strcmp(&mtfg[4][0],"mstartbit") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            tempdata = atoi(&mtfg[5][0]);
            if((tempdata>=1) &&(tempdata <=31 ) )  {Dev_data.Pru.MStartBit = tempdata;printf("PRU Data MStartBit= %d\n",Dev_data.Pru.MStartBit); return;}
            else {mHD_printf_err(); return;}
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MStartBit = %d\n",Dev_data.Pru.MStartBit); return;}
    }
    else if(strcmp(&mtfg[4][0],"mendbit") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            tempdata = atoi(&mtfg[5][0]);
            if((tempdata>=1) &&(tempdata <=31 ) )  {Dev_data.Pru.MEndBit = tempdata;printf("PRU Data MEndBit= %d\n",Dev_data.Pru.MEndBit); return;}
            else {mHD_printf_err(); return;}
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MEndBit = %d\n",Dev_data.Pru.MEndBit); return;}
    }
    else if(strcmp(&mtfg[4][0],"mbaud") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            tempdata = atoi(&mtfg[5][0]);
            if((tempdata>=0) &&(tempdata <=13 ) )  {Dev_data.Pru.MBaud = tempdata;printf("PRU Data MBaud= %d\n",Dev_data.Pru.MBaud); return;}
            else {mHD_printf_err(); return;}
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MBaud = %d\n",Dev_data.Pru.MBaud); return;}
    }
    else if(strcmp(&mtfg[4][0],"msetsource") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            tempdata = atoi(&mtfg[5][0]);
            if((tempdata>=0) &&(tempdata <=1 ) )  {Dev_data.Pru.MSetSource = tempdata;printf("PRU Data MSetSource= %d\n",Dev_data.Pru.MSetSource); return;}
            else {mHD_printf_err(); return;}
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Data MSetSource = %d\n",Dev_data.Pru.MSetSource); return;}
    }
    else if(strcmp(&mtfg[4][0],"merrcode") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Data MErrCode[] = {");
             for(i=0;i<=Dev_data.Pru.MEndBit ;i++)
             {
                 if(i== Dev_data.Pru.MEndBit)  printf("%d}\n", Dev_data.Pru.MErrCode[i]);
                 else printf("%d, ", Dev_data.Pru.MErrCode[i]);
             }
             return;
         }
    }
    else if(strcmp(&mtfg[4][0],"mrdata") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Data MRData = {");
             for(i=0;i<=Dev_data.Pru.MEndBit ;i++)
             {
                  if(i== Dev_data.Pru.MEndBit)  printf("%d}\n", Dev_data.Pru.MRData[i]);
                  else printf("%d, ", Dev_data.Pru.MRData[i]);
             }
             return;
         }
    }
    else if(strcmp(&mtfg[4][0],"mwdata") ==0)
    {
        if(strcmp(&mtfg[0][0],"write") ==0)
        {
            tempdata = atoi(&mtfg[5][0]);
            tempdata1 = atoi(&mtfg[6][0]);
            if((tempdata>=1) &&(tempdata <=31 ) &&(tempdata1 >=0)&&(tempdata1<=1))
            {
                Dev_data.Pru.MWData[tempdata] = tempdata1;
                printf("PRU Data MWData[%d]= %d\n",tempdata ,tempdata1); return;
            } else {mHD_printf_err(); return;}
        }
        else if(strcmp(&mtfg[0][0],"read") ==0)
        {
            printf("PRU Data MWData[] = {");
            for(i=0;i<=Dev_data.Pru.MEndBit;i++)
            {
                if(i== Dev_data.Pru.MEndBit)  printf("%d}\n",Dev_data.Pru.MWData[i]);
                else printf("%d, ",Dev_data.Pru.MWData[i]);
            }
            return;
        }
    }
    else if(strcmp(&mtfg[4][0],"all") ==0)  //读取模块全部参数
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("PRU Data RShmid = %d.\n", Dev_data.Pru.RShmid);
              printf("PRU Data RSemid = %d.\n", Dev_data.Pru.RSemid);
              printf("PRU Data RMsgid = %d.\n", Dev_data.Pru.RMsgid);
              printf("PRU Data RShmkey = %d.\n", Dev_data.Pru.RShmkey);
              printf("PRU Data RSemkey = %d.\n", Dev_data.Pru.RSemkey);
              printf("PRU Data RMsgkey = %d.\n", Dev_data.Pru.RMsgkey);
              printf("PRU Data Rpid = { ");
              for(i=0;i<6;i++)
              {
                  if(i==5 ) printf("%d}\n", Dev_data.Pru.RPid[i]);
                  else printf("%d, ", Dev_data.Pru.RPid[i]);
              }
              printf("PRU Data RpidLink = { ");
              for(i=0;i<6;i++)
              {
                  if(i==5 ) printf("%d}\n", Dev_data.Pru.RPidLink[i]);
                  else printf("%d, ", Dev_data.Pru.RPidLink[i]);
              }
              printf("PRU Data RStatus = %d\n",Dev_data.Pru.RStatus);
              printf("PRU Data RBaud = %d\n",Dev_data.Pru.RBaud);
              printf("PRU Data MStatus = %d\n",Dev_data.Pru.MStatus);
              printf("PRU Data MWSetStatus = %d\n",Dev_data.Pru.MWSetStatus);
              printf("PRU Data MRSetStatus = %d\n",Dev_data.Pru.MRSetStatus);
              printf("PRU Data MScanStatus = %d\n",Dev_data.Pru.MScanStatus);
              printf("PRU Data MStartBit = %d\n",Dev_data.Pru.MStartBit);
              printf("PRU Data MEndBit = %d\n",Dev_data.Pru.MEndBit);
              printf("PRU Data MBaud = %d\n",Dev_data.Pru.MBaud);
              printf("PRU Data MSetSource = %d\n",Dev_data.Pru.MSetSource);
              printf("PRU Data MErrCode[] = {");
              for(i=0;i<=Dev_data.Pru.MEndBit ;i++)
              {
                  if(i== Dev_data.Pru.MEndBit)  printf("%d}\n", Dev_data.Pru.MErrCode[i]);
                  else printf("%d, ", Dev_data.Pru.MErrCode[i]);
              }
              printf("PRU Data MWData[] = {");
              for(i=0;i<=Dev_data.Pru.MEndBit;i++)
              {
                  if(i== Dev_data.Pru.MEndBit)  printf("%d}\n",Dev_data.Pru.MWData[i]);
                  else printf("%d, ",Dev_data.Pru.MWData[i]);
              }
              printf("PRU Data MRData[] = {");
              for(i=0;i<=Dev_data.Pru.MEndBit ;i++)
              {
                   if(i== Dev_data.Pru.MEndBit)  printf("%d}\n", Dev_data.Pru.MRData[i]);
                   else printf("%d, ", Dev_data.Pru.MRData[i]);
              }
          }
    }
    else  {mHD_printf_err(); return;}
}

//键盘输入 module config 命令
void  keyboard_moduleconfig(char (*mtfg)[128])
{
    int i;
    int num;
    uint32_t tdata;
    int tvalue;

    num= atoi(&mtfg[2][0]);
    if((num<1)&&(num>31)) {mHD_printf_err(); return;}

    if(strcmp(&mtfg[4][0],"status") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module MConfig[%d] Status = %d\n",num, Dev_data.Mconfig[num].SetStatus ); return;}
    }
    else if(strcmp(&mtfg[4][0],"errcode") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module Mconfig[%d] ErrCode  = 0x%X\n",num,Dev_data.Mconfig[num].ErrCode ); return;}
    }
    else if(strcmp(&mtfg[4][0],"bitnum") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             if((tdata>=1) &&(tdata <=31 ) )  {Dev_data.Mconfig[num].BitNum = tdata;printf("PRU Mconfig[%d] BitNum = 0x%X\n",num,tdata); return;}
             else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Mconfig[%d] BitNum = 0x%X\n",num,Dev_data.Mconfig[num].BitNum); return;}
    }
    else if(strcmp(&mtfg[4][0],"name") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             mHD_StrToHex(&mtfg[5][0],&tdata);
             if((tdata>=1000) &&(tdata <=40000) )  {Dev_data.Mconfig[num].Name = tdata;printf("PRU Mconfig[%d] Name = 0x%X\n",num,tdata); return;}
             else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Mconfig[%d] Name = 0x%X\n",num,Dev_data.Mconfig[num].Name); return;}
    }
    else if(strcmp(&mtfg[4][0],"ver") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             mHD_StrToHex(&mtfg[5][0],&tdata);
             if((tdata>=10) &&(tdata <=15 ) )  {Dev_data.Mconfig[num].Ver = tdata;printf("PRU Mconfig[%d] Ver = 0x%X\n",num,tdata); return;}
             else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Mconfig[%d] Ver = 0x%X\n",num,Dev_data.Mconfig[num].Ver); return;}
    }
    else if(strcmp(&mtfg[4][0],"baud") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             mHD_StrToHex(&mtfg[5][0],&tdata);
             if(tdata <=13  )  {Dev_data.Mconfig[num].Baud = tdata;printf("PRU Mconfig[%d] Baud = 0x%X\n",num,tdata); return;}
             else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("PRU Mconfig[%d] Baud = 0x%X\n",num,Dev_data.Mconfig[num].Baud); return;}
    }
    else if(strcmp(&mtfg[4][0],"dinfiltertime") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <=4 ) &&(tvalue>=0)&&(tvalue<=8))
             {
                 Dev_data.Mconfig[num].DInFilterTime[tdata] = tvalue;
                 printf("PRU Mconfig[%d] DInFilterTime[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] DInFilterTime = {",num);
             for(i=0;i<4;i++)
             {
                 if(i==3) printf("%d}\n",Dev_data.Mconfig[num].DInFilterTime[i]);
                 else printf("%d,",Dev_data.Mconfig[num].DInFilterTime[i]);
             }   return;
         }
    }
    else if(strcmp(&mtfg[4][0],"dinfunset") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 11) &&(tvalue>=0)&&(tvalue<=3))
             {
                 Dev_data.Mconfig[num].DInFunSet[tdata] = tvalue;
                 printf("PRU Mconfig[%d] DInFunSet[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] DInFunSet = {",num);
             for(i=0;i<12;i++)
             {
                 if(i==11) printf("%d}\n",Dev_data.Mconfig[num].DInFunSet[i]);
                 else printf("%d, ",Dev_data.Mconfig[num].DInFunSet[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"doutfunset") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 11) &&(tvalue>=0)&&(tvalue<=3))
             {
                 Dev_data.Mconfig[num].DOutFunSet[tdata] = tvalue;
                 printf("PRU Mconfig[%d] DOutFunSet[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] DOutFunSet = {",num);
             for(i=0;i<8;i++)
             {
                 if(i==7) printf("%d}\n",Dev_data.Mconfig[num].DOutFunSet[i]);
                 else printf("%d, ",Dev_data.Mconfig[num].DOutFunSet[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"enscpulseset") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 5) &&(tvalue>=0)&&(tvalue<=65535))
             {
                 Dev_data.Mconfig[num].EnSCPulseSet[tdata] = tvalue;
                 printf("PRU Mconfig[%d] EnSCPulseSet[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] EnSCPulseSet = {",num);
             for(i=0;i<6;i++)
             {
                 if(i==5) printf("%d}\n",Dev_data.Mconfig[num].EnSCPulseSet[i]);
                 else printf("%d,",Dev_data.Mconfig[num].EnSCPulseSet[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"ssifunset") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 1) &&(tvalue>=0)&&(tvalue<=2))
             {
                 Dev_data.Mconfig[num].SSIFunSet[tdata] = tvalue;
                 printf("PRU Mconfig[%d] SSIFunSet[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] SSIFunSet = {",num);
             for(i=0;i<2;i++)
             {
                 if(i==1)  printf("%d}\n",Dev_data.Mconfig[num].SSIFunSet[i]);
                 else printf("%d,",Dev_data.Mconfig[num].SSIFunSet[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"aoutareg") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 3) &&(((tvalue>=10)&&(tvalue<=11))||(tvalue ==0)))
             {
                 Dev_data.Mconfig[num].AOutAReg[tdata] = tvalue;
                 printf("PRU Mconfig[%d] AOutAReg[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] AOutAReg = {",num);
             for(i=0;i<4;i++)
             {
                 if(i==3) printf("%d}\n",Dev_data.Mconfig[num].AOutAReg[i]);
                 else printf("%d,",Dev_data.Mconfig[num].AOutAReg[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"aoutvreg") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 3) &&(tvalue>=0)&&(tvalue<=4))
             {
                 Dev_data.Mconfig[num].AOutVReg[tdata] = tvalue;
                 printf("PRU Mconfig[%d] AOutVReg[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] AOutVReg = {",num);
             for(i=0;i<4;i++)
             {
                 if(i==3) printf("%d}\n",Dev_data.Mconfig[num].AOutVReg[i]);
                 else printf("%d,",Dev_data.Mconfig[num].AOutVReg[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"ainreg") ==0)
    {
         if(strcmp(&mtfg[0][0],"write") ==0)
         {
             tdata = atoi(&mtfg[5][0]);
             tvalue = atoi(&mtfg[6][0]);
             if((tdata <= 11) &&(((tvalue>=0)&&(tvalue<=5)) ||((tvalue>=10)&&(tvalue<=12))))
             {
                 Dev_data.Mconfig[num].AInReg[tdata] = tvalue;
                 printf("PRU Mconfig[%d] AInReg[%d] = %d\n",num,tdata,tvalue); return;
             } else {mHD_printf_err(); return;}
         }
         else if(strcmp(&mtfg[0][0],"read") ==0)
         {
             printf("PRU Mconfig[%d] AInReg = {",num);
             for(i=0;i<12;i++)
             {
                 if(i ==11 ) printf("%d}\n",Dev_data.Mconfig[num].AInReg[i]);
                 else printf("%d,",Dev_data.Mconfig[num].AInReg[i]);
             } return;
         }
    }
    else if(strcmp(&mtfg[4][0],"all") ==0)  //读取模块全部参数
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] ALL Config:\n",num);
              printf("SetStatus = %d\n",Dev_data.Mconfig[num].SetStatus );
              printf("ErrCode = %d\n",Dev_data.Mconfig[num].ErrCode );
              printf("BitNum = %d\n",Dev_data.Mconfig[num].BitNum );
              printf("Name = 0x%X\n",Dev_data.Mconfig[num].Name );
              printf("Ver = 0x%X\n",Dev_data.Mconfig[num].Ver );
              printf("Baud = %d\n",Dev_data.Mconfig[num].Baud );
              if(Dev_data.Mconfig[num].Name == 0x7122)
              {
                  printf("DinFilterTime ={");
                  for(i=0;i<4;i++)
                  {
                      if(i==3 ) printf("%d}\n",Dev_data.Mconfig[num].DInFilterTime[i] );
                      else printf("%d,",Dev_data.Mconfig[num].DInFilterTime[i] );
                  }
                  return;
              }
              else if(Dev_data.Mconfig[num].Name == 0x8012)
              {
                  printf("AOutAReg ={");
                  for(i=0;i<4;i++)
                  {
                      if(i==3 ) printf("%d}\n",Dev_data.Mconfig[num].AOutAReg[i] );
                      else printf("%d,",Dev_data.Mconfig[num].AOutAReg[i] );
                  }
                  printf("AOutVReg ={");
                  for(i=0;i<4;i++)
                  {
                      if(i==3 ) printf("%d}\n",Dev_data.Mconfig[num].AOutVReg[i] );
                      else printf("%d,",Dev_data.Mconfig[num].AOutVReg[i] );
                  }
                  printf("AInReg ={");
                  for(i=0;i<12;i++)
                  {
                      if(i==11 ) printf("%d}\n",Dev_data.Mconfig[num].AInReg[i] );
                      else printf("%d,",Dev_data.Mconfig[num].AInReg[i] );
                  }
                  return;
              }
              if(Dev_data.Mconfig[num].Name == 0x7313)
              {
                  printf("DOutFunSet ={");
                  for(i=0;i<8;i++)
                  {
                      if(i==7 ) printf("%d}\n",Dev_data.Mconfig[num].DOutFunSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].DOutFunSet[i] );
                  }
                  printf("DInFunSet ={");
                  for(i=0;i<12;i++)
                  {
                      if(i==11 ) printf("%d}\n",Dev_data.Mconfig[num].DInFunSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].DInFunSet[i] );
                  }
                  printf("DinFilterTime = %d\n",Dev_data.Mconfig[num].DInFilterTime[0]);
                  printf("EnSCPulseSet ={");
                  for(i=0;i<6;i++)
                  {
                      if(i==5 ) printf("%d}\n",Dev_data.Mconfig[num].EnSCPulseSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].EnSCPulseSet[i] );
                  }
                  return;
              }
              if(Dev_data.Mconfig[num].Name == 0x7314)
              {
                  printf("DOutFunSet ={");
                  for(i=0;i<8;i++)
                  {
                      if(i==7 ) printf("%d}\n",Dev_data.Mconfig[num].DOutFunSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].DOutFunSet[i] );
                  }
                  printf("DInFunSet ={");
                  for(i=0;i<6;i++)
                  {
                      if(i==5 ) printf("%d}\n",Dev_data.Mconfig[num].DInFunSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].DInFunSet[i] );
                  }
                  printf("SSIFunSet ={");
                  for(i=0;i<2;i++)
                  {
                      if(i==1 ) printf("%d}\n",Dev_data.Mconfig[num].SSIFunSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].SSIFunSet[i] );
                  }
                  printf("DinFilterTime = %d\n",Dev_data.Mconfig[num].DInFilterTime[0]);
                  printf("EnSCPulseSet ={");
                  for(i=0;i<3;i++)
                  {
                      if(i==2 ) printf("%d}\n",Dev_data.Mconfig[num].EnSCPulseSet[i] );
                      else printf("%d,",Dev_data.Mconfig[num].EnSCPulseSet[i] );
                  }
                  return;
              }
          }
    }
    else {mHD_printf_err(); return;}
}

//键盘输入 module Data 命令
void keyboard_moduledata(char (*mtfg)[128])
{
    int i;
    int num;
    int tdata;
    int tvalue;

    num= atoi(&mtfg[2][0]);
    if((num<1)&&(num>31)) {mHD_printf_err(); return;}

    if(strcmp(&mtfg[4][0],"errcode") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module MData[%d] ErrCode Value = %d\n",num, Dev_data.MData[num].ErrCode); return;}
    }
    else if(strcmp(&mtfg[4][0],"bitnum") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module MData[%d] BitNum Value = %d\n",num,Dev_data.MData[num].BitNum); return;}
    }
    else if(strcmp(&mtfg[4][0],"name") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module MData[%d] Name Value = 0x%X\n",num,Dev_data.MData[num].Name); return;}
    }
    else if(strcmp(&mtfg[4][0],"status") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)  {printf("Module MData[%d] Status Value = %d\n",num,Dev_data.MData[num].Status); return;}
    }
    else if(strcmp(&mtfg[4][0],"dindata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] DInData Value = {",num);
              for(i=0;i<32;i++)
              {
                  if((i==7)||(i==15)||(i==23)) printf("%d,  ",Dev_data.MData[num].DInData[i] );
                  else if(i==31) printf("%d}\n",Dev_data.MData[num].DInData[i] );
                  else printf("%d,",Dev_data.MData[num].DInData[i] );
              }
              return;
          }
    }
    else if(strcmp(&mtfg[4][0],"doutdata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)
          {
              tdata = atoi(&mtfg[5][0]);
              tvalue = atoi(&mtfg[6][0]);
              if((tdata>=0) &&(tdata <= 15) &&(tvalue>=0)&&(tvalue<=1))
              {
                  Dev_data.MData[num].DOutData[tdata] = tvalue;
                  printf("PRU MData[%d] DOutData[%d] = %d\n",num,tdata,tvalue); return;
              } else {mHD_printf_err(); return;}
          }
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] DOutData Value = {",num);
              for(i=0;i<16;i++)
              {
                  if(i==7)printf("%d,  ",Dev_data.MData[num].DOutData[i] );
                  else if(i==15) printf("%d}\n",Dev_data.MData[num].DOutData[i] );
                  else printf("%d,",Dev_data.MData[num].DOutData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"eninclear") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)
          {
              tdata = atoi(&mtfg[5][0]);
              tvalue = atoi(&mtfg[6][0]);
              if((tdata>=0) &&(tdata <= 5) &&(tvalue>=0)&&(tvalue<=1))
              {
                  Dev_data.MData[num].EnInClear[tdata] = tvalue;
                  printf("PRU MData[%d] EnInClear[%d] = %d\n",num,tdata,tvalue); return;
              } else {mHD_printf_err(); return;}
          }
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] EnInClear Value = {",num);
              for(i=0;i<6;i++)
              {
                  if(i==5) printf("%d}\n",Dev_data.MData[num].EnInClear[i] );
                  else printf("%d,",Dev_data.MData[num].EnInClear[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"enindir") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] EnInDir Value = {",num);
              for(i=0;i<6;i++)
              {
                  if(i==5) printf("%d}\n",Dev_data.MData[num].EnInDir[i] );
                  else printf("%d,",Dev_data.MData[num].EnInDir[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"ssiindir") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] SSIInDir Value = {",num);
              for(i=0;i<2;i++)
              {
                  if(i==1) printf("%d}\n",Dev_data.MData[num].SSIInDir[i] );
                  else printf("%d,",Dev_data.MData[num].SSIInDir[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"ssierr") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] SSIErr Value = {",num);
              for(i=0;i<2;i++)
              {
                  if(i==1) printf("%d}\n",Dev_data.MData[num].SSIErr[i] );
                  else printf("%d,",Dev_data.MData[num].SSIErr[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"pulsedata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)
          {
              tdata = atoi(&mtfg[5][0]);
              tvalue = atoi(&mtfg[6][0]);
              if((tdata>=0) &&(tdata <= 7) )
              {
                  Dev_data.MData[num].PulseData[tdata] = tvalue;
                  printf("Module MData[%d] PulseData[%d] = %d\n",num,tdata,tvalue); return;
              } else {mHD_printf_err(); return;}
          }
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] PulseData Value = {",num);
              for(i=0;i<8;i++)
              {
                  if(i==7) printf("%d}\n",Dev_data.MData[num].PulseData[i] );
                  else printf("%d,",Dev_data.MData[num].PulseData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"enpulsedata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] EnPulseData Value = {",num);
              for(i=0;i<6;i++)
              {
                  if(i==5) printf("%d}\n",Dev_data.MData[num].EnPulseData[i] );
                  else printf("%d,",Dev_data.MData[num].EnPulseData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"enspeeddata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] EnSpeedData Value = {",num);
              for(i=0;i<6;i++)
              {
                  if(i==5) printf("%0.1f}\n",(float)Dev_data.MData[num].EnSpeedData[i]/10 );
                  else printf("%0.1f, ",(float)Dev_data.MData[num].EnSpeedData[i]/10);
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"ssipulsedata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] SSIPulseData Value = {",num);
              for(i=0;i<2;i++)
              {
                  if(i==1) printf("%d}\n",Dev_data.MData[num].SSIPulseData[i] );
                  else printf("%d,",Dev_data.MData[num].SSIPulseData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"ssispeeddata") ==0)
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] SSISpeedData Value = {",num);
              for(i=0;i<2;i++)
              {
                  if(i==1) printf("%0.1f}\n",(float)Dev_data.MData[num].SSISpeedData[i]/10 );
                  else printf("%0.1f,",(float)Dev_data.MData[num].SSISpeedData[i]/10);
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"aindata") ==0)  //完善输入值转化为电流电压值
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] AInData Value = {",num);
              for(i=0;i<12;i++)
              {
                  if(i==11) printf("%d}\n",Dev_data.MData[num].AInData[i] );
                  else printf("%d,",Dev_data.MData[num].AInData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"aoutdata") ==0) //完善输入值转化为电流电压值
    {
          if(strcmp(&mtfg[0][0],"write") ==0)
          {
              tdata = atoi(&mtfg[5][0]);
              tvalue = atoi(&mtfg[6][0]);
              if((tdata>=0) &&(tdata <= 3) )
              {
                  Dev_data.MData[num].AOutData[tdata] = tvalue;
                  printf("PRU MData[%d] AOutData[%d] = %d\n",num,tdata,tvalue); return;
              } else {mHD_printf_err(); return;}
          }
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d] AOutData Value = {",num);
              for(i=0;i<4;i++)
              {
                  if(i==3) printf("%d}\n",Dev_data.MData[num].AOutData[i] );
                  else printf("%d,",Dev_data.MData[num].AOutData[i] );
              } return;
          }
    }
    else if(strcmp(&mtfg[4][0],"all") ==0)  //读取模块全部参数
    {
          if(strcmp(&mtfg[0][0],"write") ==0)  {mHD_printf_err(); return;}
          else if(strcmp(&mtfg[0][0],"read") ==0)
          {
              printf("Module MData[%d]-%X: ALL Value :\n",num,Dev_data.Mconfig[num].Name);
              if(Dev_data.Mconfig[num].Name == 0x7122)
              {
                  printf("DInData = {");
                  for(i=0;i<32;i++)
                  {
                      if((i==7)||(i==15)||(i==23)) printf("%d,  ",Dev_data.MData[num].DInData[i] );
                      else if(i==31) printf("%d}\n",Dev_data.MData[num].DInData[i] );
                      else printf("%d,",Dev_data.MData[num].DInData[i] );
                  }
                  printf("DOutData = {");
                  for(i=0;i<16;i++)
                  {
                      if(i==7)printf("%d,  ",Dev_data.MData[num].DOutData[i] );
                      else if(i==15) printf("%d}\n",Dev_data.MData[num].DOutData[i] );
                      else printf("%d,",Dev_data.MData[num].DOutData[i] );
                  } return;
              }
              else if(Dev_data.Mconfig[num].Name == 0x8012)
              {
                  printf("AInData = {");
                  for(i=0;i<12;i++)
                  {
                      if(i==11) printf("%d}\n",Dev_data.MData[num].AInData[i] );
                      else printf("%d,",Dev_data.MData[num].AInData[i] );
                  }
                  printf("AOutData = {");
                  for(i=0;i<4;i++)
                  {
                      if(i==3) printf("%d}\n",Dev_data.MData[num].AOutData[i] );
                      else printf("%d,",Dev_data.MData[num].AOutData[i] );
                  } return;
              }
              else if(Dev_data.Mconfig[num].Name == 0x7313)
              {
                  printf("DInData = {");
                  for(i=0;i<20;i++)
                  {
                      if(i==12) printf("%d,  ",Dev_data.MData[num].DInData[i] );
                      else if(i==19) printf("%d}\n",Dev_data.MData[num].DInData[i] );
                      else printf("%d,",Dev_data.MData[num].DInData[i] );
                  }
                  printf("DOutData = {");
                  for(i=0;i<16;i++)
                  {
                      if(i==7)printf("%d,  ",Dev_data.MData[num].DOutData[i] );
                      else if(i==15) printf("%d}\n",Dev_data.MData[num].DOutData[i] );
                      else printf("%d,",Dev_data.MData[num].DOutData[i] );
                  }
                  printf("EnInDir = {");
                  for(i=0;i<6;i++)
                  {
                      if(i==5) printf("%d}\n",Dev_data.MData[num].EnInDir[i] );
                      else printf("%d,",Dev_data.MData[num].EnInDir[i] );
                  }
                  printf("EnPulseData = {");
                  for(i=0;i<6;i++)
                  {
                      if(i==5) printf("%d}\n",Dev_data.MData[num].EnPulseData[i] );
                      else printf("%d,",Dev_data.MData[num].EnPulseData[i] );
                  }
                  printf("EnSpeedData = {");
                  for(i=0;i<6;i++)
                  {
                      if(i==5) printf("%0.1f}\n",(float)Dev_data.MData[num].EnSpeedData[i]/10 );
                      else printf("%0.1f,",(float)Dev_data.MData[num].EnSpeedData[i]/10);
                  }
                  printf("PulseData = {");
                  for(i=0;i<8;i++)
                  {
                      if(i==7) printf("%d}\n",Dev_data.MData[num].PulseData[i] );
                      else printf("%d,",Dev_data.MData[num].PulseData[i] );
                  } return;
              }
              else if(Dev_data.Mconfig[num].Name == 0x7314)
              {
                  printf("DInData = {");
                  for(i=0;i<14;i++)
                  {
                      if(i==6) printf("%d,  ",Dev_data.MData[num].DInData[i] );
                      if(i==13) printf("%d}\n",Dev_data.MData[num].DInData[i] );
                      else printf("%d,",Dev_data.MData[num].DInData[i] );
                  }
                  printf("DOutData = {");
                  for(i=0;i<16;i++)
                  {
                      if(i==7)printf("%d,  ",Dev_data.MData[num].DOutData[i] );
                      else if(i==15) printf("%d}\n",Dev_data.MData[num].DOutData[i] );
                      else printf("%d,",Dev_data.MData[num].DOutData[i] );
                  }
                  printf("EnInDir = {");
                  for(i=0;i<3;i++)
                  {
                      if(i==2) printf("%d}\n",Dev_data.MData[num].EnInDir[i] );
                      else printf("%d,",Dev_data.MData[num].EnInDir[i] );
                  }
                  printf("SSIInDir = {");
                  for(i=0;i<2;i++)
                  {
                      if(i==1) printf("%d}\n",Dev_data.MData[num].SSIInDir[i] );
                      else printf("%d, ",Dev_data.MData[num].SSIInDir[i] );
                  }
                  printf("SSIErr = {");
                  for(i=0;i<2;i++)
                  {
                      if(i==1) printf("%d}\n",Dev_data.MData[num].SSIErr[i] );
                      else printf("%d,",Dev_data.MData[num].SSIErr[i] );
                  }
                  printf("EnPulseData = {");
                  for(i=0;i<3;i++)
                  {
                      if(i==2) printf("%d}\n",Dev_data.MData[num].EnPulseData[i] );
                      else printf("%d,",Dev_data.MData[num].EnPulseData[i] );
                  }
                  printf("SSIPulseData = {");
                  {
                      for(i=0;i<2;i++)
                      {
                          if(i==1) printf("%d}\n",Dev_data.MData[num].SSIPulseData[i] );
                          else printf("%d,",Dev_data.MData[num].SSIPulseData[i] );
                      }
                  }
                  printf("EnSpeedData = {");
                  for(i=0;i<3;i++)
                  {
                      if(i==2) printf("%0.1f}\n",(float)Dev_data.MData[num].EnSpeedData[i]/10 );
                      else printf("%0.1f,",(float)Dev_data.MData[num].EnSpeedData[i]/10);
                  }
                  printf("SSISpeedData = {");
                  {
                      for(i=0;i<2;i++)
                      {
                          if(i==1) printf("%0.1f}\n",(float)Dev_data.MData[num].SSISpeedData[i]/10 );
                          else printf("%0.1f,",(float)Dev_data.MData[num].SSISpeedData[i]/10);
                      }
                  }
                  printf("PulseData = {");
                  for(i=0;i<8;i++)
                  {
                     if(i==7) printf("%d}\n",Dev_data.MData[num].PulseData[i] );
                      else printf("%d,",Dev_data.MData[num].PulseData[i] );
                  } return;
              }
          }
    }
    else {mHD_printf_err(); return;}
}























