/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  PRU_Protocol.c
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

#include "PRU_ConfigWrite.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "../HqDeviceDriver/HqDeviceDriver.h"

#define FILE_READ_MAXLEN      128-1

#define SETSYS_LEN       6
#define MODSET_LEN    6

#define PRUCONFIGFILE      "/home/hqtech/ctrsoft/Modulectr_conf"      //设置文件路径
#define DEBUGNAME           "/home/hqtech/ctrsoft/Mdebug_conf"            //配置文件路径及文件名
#define RPURUNDATA          "/home/hqtech/ctrsoft/MdRunData"                //配置文件路径及文件名

#define M_TXTREAD    "rt"       //只读方式打开文本文件
#define M_TXTRWITE   "rt+"    //读写方式打开文本文件
#define M_DATREAD    "rb"       //只读方式打开二进制文件
#define M_DATRWITE   "rb+"    //读写方式打开二进制文件

static const char *SetSys = "system";
static const char *ModSet ="module";
static const char CtrEnd  = '}';
static const char CtrFirst = '{';
static const char CtrEqu = '=';
static const char CtrSP = ' ';
static const char CtrSeg = ',';
static const char CtrNote = '#';
//static const char CtrNone = '\0';

Mod_system M_SetSysData;
Mod_set M_SetData[128];

/**************************************
 * 读取设置参数,填充到SetData 数组
 * 返回 读取到的MODSET 数量, 返回0读取错误
**************************************/
uint8_t mHD_MPU_Read_SetFile(void)
{
    FILE *fp;
    uint32_t mdata =0;
    uint8_t m_end =0;
    uint16_t m_moddatalen =0;
    uint16_t m_setdatalen =0;

    uint8_t set_sysen =0;
    uint8_t set_data =0;
    //uint8_t set_es = 0;

    uint16_t SetData[512];
    char buf[512];
    char segbuf[512];
    char * p_set;
    char * p_mod;
    char * p_first;
    char * p_end;
    char * p_equ;
    char * p;
    uint16_t len,i;

    fp = fopen(PRUCONFIGFILE,"rb");
    if(fp ==NULL)  return 0;    //打开文件失败。

    while( fgets(buf,FILE_READ_MAXLEN,fp)  !=NULL )  //按行读取
    {
        for(i=0;i<strlen(buf);i++) {
            if((*(buf+i) !=CtrSP) &&(*(buf+i) !=CtrNote)) break;  //如果不是空格或者# 退出for语句执行后面内容。
            else if (*(buf+i) ==CtrNote){
                m_end =1;
                break;
            }
        }
        if(m_end ==1) {
            m_end =0;
            continue;
        }
        p_first =strchr(buf, CtrFirst);              //首先查找 ‘{’
        p_end =strchr(buf, CtrEnd);               //首先查找 ‘}’
        p_equ= strchr(buf,CtrEqu);                //查找 CtrEqu  ==
        p_set = strstr(buf,SetSys);                  //查找 SetSys  system
        p_mod= strstr(buf,ModSet);              //查找 ModSet module

        if((p_first==NULL)||(p_end ==NULL)||(p_equ ==NULL))  continue;
        if((p_set ==NULL)&&(p_mod==NULL))  continue;

        if(p_set !=NULL)  len = p_equ - p_set - SETSYS_LEN;          //判断 ‘=’  前面是否有除 ‘  ’ 外的其它符号
        else if(p_mod !=NULL)  len = p_equ - p_mod - SETSYS_LEN;          //判断 ‘=’  前面是否有除 ‘  ’ 外的其它符号
        else continue;

        for(i=0;i<len;i++) { if(*(buf+SETSYS_LEN+i) !=CtrSP ) { return 0;} }

        len = p_first - p_equ - 1;       //判断 ‘{’  前面是否有除 ‘  ’ 外的其它符号
        for(i=0;i<len;i++) { if(*(p_equ+1+i) !=CtrSP ) { return 0;} }

        if(p_set !=NULL)    //SetSystem
        {
            if(set_sysen !=0)  return 0;    //只能扫描一次
            if(p_set != buf)  {   //命令符前面有除空格外的其它字符判断
                len =p_set -buf;
                for(i=0;i<len;i++) {  if(*(buf+i) != CtrSP) { return 0;}  }
            }
            //分解字符串
            len = p_end-p_first;
            segbuf[0] = '\0';
            p= p_first+1;
            m_setdatalen =0;
            for(i=0;i<len;i++)
            {
                if((*(p+i) != CtrSP)&&(*(p+i) != CtrSeg)&&(*(p+i) !=CtrEnd))
                {
                    strncat(segbuf,p+i,1);
                }else if((*(p+i) == CtrSeg)||(*(p+i) == CtrEnd)){
                    if(mHD_StrToHex(segbuf,&mdata) ==0) {
                        SetData[m_setdatalen] = mdata;
                        printf("位号:0 序号:%d 值:%x\n",m_setdatalen,SetData[m_setdatalen]);
                        m_setdatalen++;
                        segbuf[0] = '\0';
                        mdata =0;
                    }  else return 0;
                  }
                }
            set_sysen =1;
             M_SetSysData.Devbaud = SetData[0] ;
             M_SetSysData.Modbaud = SetData[1] ;
             printf("*********Set_System************\n");
        } else {
            //分解字符串
            if(p_mod != buf)  {   //命令符前面有除空格外的其它字符判断
                len =p_mod -buf;
                for(i=0;i<len;i++) {  if(*(buf+i) != CtrSP) { return 0;}  }
            }
            len = p_end-p_first;
            segbuf[0] = '\0';
            p= p_first+1;
            m_moddatalen =0;
            for(i=0;i<len;i++)
            {
                if((*(p+i) != CtrSP)&&(*(p+i) != CtrSeg)&&(*(p+i) != CtrEnd))
                {
                    strncat(segbuf,p+i,1);
                }else if((*(p+i) == CtrSeg)||(*(p+i) == CtrEnd)) {
                    if(mHD_StrToHex(segbuf,&mdata) ==0) {
                        SetData[m_moddatalen] = mdata;
                        printf("位号:0 序号:%d 值:%x\n", m_moddatalen,SetData[m_moddatalen]);
                        m_moddatalen++;
                        segbuf[0] = '\0';
                        mdata =0;
                    }  else return 0;
                }
            }
            //set_es =0;
            set_data++;
            //if(SetData[0]  != set_data)  return 0;  //位号排序错误
            M_SetData[set_data].bitnum =  SetData[0];
            M_SetData[set_data].name = SetData[1];
            M_SetData[set_data].ver = SetData[2];
            if(SetData[1] == 0x7122) {
                for(i=0;i<4;i++) M_SetData[set_data].setdata[i] = SetData[i+3];
            }
            else if(SetData[1] == 0x8012)  {
                for(i=0;i<20;i++) M_SetData[set_data].setdata[i] = SetData[i+3];
            }
            else if(SetData[1] == 0x7313)  {
                for(i=3;i<21;i++)  M_SetData[set_data].setdata[i] = SetData[i+3];
                for(i=0;i<6;i++) M_SetData[set_data].esturn[i] = SetData[24+i];
            }
           else if(SetData[1] == 0x7314)  {
               for(i=0;i<17;i++)   M_SetData[set_data].setdata[i] = SetData[i+3];
               for(i=0;i<3;i++) M_SetData[set_data].esturn[i] = SetData[20+i] ;
                }
            printf("*****Modsett_%d***************\n\n",set_data);
        }
    }
        fclose(fp);
        return  set_data;
}


/************* 读取系统运行配置文本文件  **********************************************
 * 名称：             mHD_Read_Config_Conf(void)
 * 功能：             读取系统运行配置文件如果不存在则创建
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，正确返回0
 * ********************************************************************************/
#define RUNSETMAX   4
#define RUNSETCNT   1
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

    re = mHD_Create_File(DEBUGNAME);
    if(re ==1) //新创建文件
    {
         fp = mHD_Open_File(DEBUGNAME,M_TXTRWITE);  // 读写模式打开文件
        sprintf(buf,"debug = 0\n");
        mHD_WriteTXT_File_String(fp,buf); //写入字符串到文件
        mHD_Close_File(fp);  //关闭文件
    }
    if(re ==0) //读取文件
    {
         fp = mHD_Open_File(DEBUGNAME,M_TXTREAD);  // 只读模式打开文件
         while(mHD_ReadTXT_File_Row(fp,buf,64) !=NULL)
         {
             //printf("%s",buf);
             mlen = strlen(buf);                                 //计算输入的字符数量
             buf[mlen-1]  =  '\0';
             mHD_CLibanalysis(buf, mtfg, " ");

             for(i=0;i<RUNSETCNT;i++)
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
            for(i=0;i<RUNSETCNT;i++) printf("%s data = %d\n",varname[i], *vardata[i]);
         }
    }
    return 0;
}

/************* 读取系统运行配置二进制文件  **********************************************
 * 名称：             mHD_Read_Run_Data(void)
 * 功能：             读取运行配置文件如果不存在，跳过，
 *                          如果存在则表示模块已设置成功,
 *                          仅需要按照配置文件内容 1、初始化命令带串口信息 2、读取模块参赛
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，0读取成功，参赛已设置
 * ********************************************************************************/
int  mHD_Read_Run_Data(void)
{
    FILE *fp;
    int readlen;
    int datalen;
    int pos;
    uint8_t readbuf[32];
    size_t datatype;
    int i;

    datatype = sizeof(uint8_t); //计算数据的字节单位
    datalen = datatype * 8;   //读取8个数
    pos = 0;   //偏置0,从头开始读取
    fp = mHD_Open_File(RPURUNDATA,M_DATREAD);  // 只读模式二进制文件
    if(fp == NULL)  //文件打开失败或者文件不存在
    {
        printf("read fp = %d\n",(int)fp);
        return -1;
    }
    //读取二进制文件
    readlen = mHD_Read_Data(fp,readbuf,datatype,datalen,pos); //读取指定数量的数据
    printf("read readlen = %d\n",readlen);
    printf("read datalen = %d\n",datalen);
    mHD_Close_File(fp);         //关闭文件
    if(readlen == datalen)
    {
        HqRpuRun_data.setmodule = readbuf[0];
        HqRpuRun_data.setstatus = readbuf[1];
        HqRpuRun_data.Devbaud = readbuf[2];
        HqRpuRun_data.Modbaud = readbuf[3];
        HqRpuRun_data.MStartBit = readbuf[4];
        HqRpuRun_data.MEndBit = readbuf[5];
        HqRpuRun_data.back = readbuf[6];
        HqRpuRun_data.user1 = readbuf[7];
        printf("read data[] = ");
        for(i=0;i<8;i++)
        {
            if(i!=7) printf("%d,",readbuf[i]);
            else printf("%d\n",readbuf[i]);
        }
        return 0;
    } else return -1;
}

/************* 写入系统运行配置二进制文件  **********************************************
 * 名称：             mHD_Read_Run_Data(void)
 * 功能：             读取运行配置文件如果不存在，跳过，
 *                          如果存在则表示模块已设置成功,
 *                          仅需要按照配置文件内容 1、初始化命令带串口信息 2、读取模块参赛
 * 入口参数：      无
 * 出口参数：      无
 * return :             FALSE -1  错误，0读取成功，参赛已设置
 * ********************************************************************************/
int  mHD_Write_Run_Data(void)
{
    FILE *fp;
    int writelen;
    int datalen;
    int pos;
    uint8_t writebuf[32];
    size_t datatype;

    datatype = sizeof(uint8_t); //计算数据的字节单位
    datalen = datatype * 8;   //读取8个数
    pos = 0;   //偏置0,从头开始读取

    mHD_Remove_File(RPURUNDATA);  //首先删除文件

    fp = mHD_Open_File(RPURUNDATA,"wb+");  // 打开二进制文件不存在就创建,存在就截断为0
    if(fp == NULL)  //文件打开失败或者文件不存在
    {
        printf("write fp = %d\n",(int)fp);
        return -1;
    }
    //写入二进制文件
    writebuf[0] = HqRpuRun_data.setmodule;
    writebuf[1] = HqRpuRun_data.setstatus;
    writebuf[2] = HqRpuRun_data.Devbaud;
    writebuf[3] = HqRpuRun_data.Modbaud;
    writebuf[4] = HqRpuRun_data.MStartBit;
    writebuf[5] = HqRpuRun_data.MEndBit ;
    writebuf[6] = HqRpuRun_data.back = 9;
    writebuf[7] = HqRpuRun_data.user1 = 3;

    //写入二进制文件
    writelen = mHD_Write_Data(fp,writebuf,datatype,datalen,pos); //读取指定数量的数据
    printf("write readlen = %d\n",writelen);
    printf("write datalen = %d\n",datalen);
    mHD_Close_File(fp); //关闭文件
    if(writelen == datalen) return 0;
    else return -1;
}















