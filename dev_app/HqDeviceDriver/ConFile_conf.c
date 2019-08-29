
/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  ConFile_conf.c
 *       Description:  读写配置文件
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/

#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>

#include "ConFile_conf.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

#define PATHNAME  "/home/hqtech/ctrsoft/hqdev_conf"    //配置文件路径及文件名

/************* 读取系统运行配置文本文件  **********************************************
 * 名称：             mHD_Read_Config_Conf(void)
 * 功能：             读取系统运行配置文件如果不存在则创建
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回文件描述符
 * ********************************************************************************/
#define RUNSETMAX   4
int  mHD_Read_Config_Conf(void)
{
    int re;
    char  buf[1024];
    FILE *fp;
    char mtfg[64][128];
    int num;
    int mlen;
    int i;
    char *varname[RUNSETMAX] = {"debug","linkmodule","setmodule","readmodule"};
    int *vardata[RUNSETMAX];

    vardata[0] = &HqDev_CmdSys.debug;
    vardata[1] = &HqDev_CmdSys.linkmodule;
    vardata[2] = &HqDev_CmdSys.setmodule;
    vardata[3] = &HqDev_CmdSys.readmodule;

    re = mHD_Create_File(PATHNAME);
    if(re ==1) //新创建文件
    {
         fp = mHD_Open_File(PATHNAME,M_TXTRWITE);  // 读写模式打开文件
        sprintf(buf,"debug = 1\n");
        mHD_WriteTXT_File_String(fp,buf); //写入字符串到文件
        sprintf(buf,"linkmodule = 1\n");
        mHD_WriteTXT_File_String(fp,buf); //写入字符串到文件
        sprintf(buf,"setmodule = 1\n");
        mHD_WriteTXT_File_String(fp,buf); //写入字符串到文件
        sprintf(buf,"readmodule = 1\n");
        mHD_WriteTXT_File_String(fp,buf); //写入字符串到文件
        mHD_Close_File(fp);  //关闭文件
    }
    if(re ==0) //读取文件
    {
         fp = mHD_Open_File(PATHNAME,M_TXTREAD);  // 只读模式打开文件
         while(mHD_ReadTXT_File_Row(fp,buf,64) !=NULL)
         {
             //printf("%s",buf);
             mlen = strlen(buf);                                 //计算输入的字符数量
             buf[mlen-1]  =  '\0';
             mHD_CLibanalysis(buf, mtfg, " ");

             for(i=0;i<RUNSETMAX;i++)
             {
                 if((strcmp(&mtfg[0][0],varname[i]) ==0) &&(strcmp(&mtfg[1][0],"=")==0))
                 {
                     num = atoi(&mtfg[2][0]);
                     if((num>-1)&&(num<2)) *vardata[i] = num;
                     else *vardata[i]  = 0;
                     break;
                 }
             }
         }
         mHD_Close_File(fp);  //关闭文件
         if(HqDev_CmdSys.debug ==1)
         {
              for(i=0;i<RUNSETMAX;i++) printf("%s data = %d\n",varname[i], *vardata[i]);
         }

    }
    return 0;
}






















