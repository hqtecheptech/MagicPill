
#include "../inc/mhd_lib.h"

#define FILE_READ_MAXLEN      128-1

#define SETSYS_LEN       6
#define MODSET_LEN    6




static const char  *File_dir = "/home/hqtech/hq_device/Modulectr_conf";      //设置文件路径
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

    fp = fopen(File_dir,"rb");
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
           // set_es =0;
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

