/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  DelayAndTimming.c
 *       Description:  延时及定时函数
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 16:50 PM"
 ****************************************************************/
//#define __USE_GNU

#include    <linux/input.h>
#include    <string.h>
#include    <stdio.h>           /*标准输入输出定义*/
#include    <stdlib.h>          /*标准函数库定义*/
#include    <time.h>
 #include <sys/timeb.h>
#include    <unistd.h>         /*Unix标准函数定义*/
#include    <sys/types.h>   /**/
#include    <sys/stat.h>      /**/
#include    <errno.h>         /*错误号定义*/
#include     <stdint.h>      /*数据类型宏定义 */
#include<signal.h>

#include "../inc/DelayAndTiming.h"
#include "../inc/Public_DataType.h"
#include "../../SysTime_Callback.h"


volatile unsigned  long timer_ms_cnt = 0;
static mHD_TimerON  mTON[TONMAX];
static mHD_TimerON  mTOFF[TOFFMAX];

/*************阻塞延时函数 秒级**************************************************
 * 名称：            mHD_sleep
 * 说明：            不受中断影响
 * 功能：             阻塞延时函数
 * 入口参数：      unsigned int seconds  秒
 * *********************************************************************/
void mHD_sleep(unsigned long seconds)
{
    struct timeval my_timeout;
    int err;
    my_timeout.tv_sec = seconds;
    my_timeout.tv_usec = 0;
    do
    {
        err = select(0, NULL, NULL, NULL, &my_timeout);
    } while (err<0 && errno==EINTR);
}
/*************阻塞延时函数 毫秒级**************************************************
 * 名称：            mHD_msleep
 * 说明：            不受中断影响
 * 功能：             阻塞延时函数
 * 入口参数：      microsecs 微秒
 * *********************************************************************/
void mHD_msleep(unsigned long millisecs)
{
    struct timeval my_timeout;
    int err;
    my_timeout.tv_sec = millisecs / 1000L;
    my_timeout.tv_usec = (millisecs%1000L)*1000L;
    do
    {
        err = select(0, NULL, NULL, NULL, &my_timeout);
    } while (err<0 && errno==EINTR);
}
/*************阻塞延时函数 微秒级**************************************************
 * 名称：            sleep_cu
 * 说明：            不受中断影响
 * 功能：             阻塞延时函数
 * 入口参数：      microsecs 微秒
 * *********************************************************************/
void mHD_usleep (unsigned long microsecs)
{
    struct timeval my_timeout;
    int err;
    my_timeout.tv_sec = microsecs / 1000000L;
    my_timeout.tv_usec = microsecs %1000000L;
    do
    {
        err = select(0, NULL, NULL, NULL, &my_timeout);
    } while (err<0 && errno==EINTR);
}
/*************阻塞延时函数 秒and微秒级**************************************************
 * 名称：            mHD_timer_sleep
 * 说明：            不受中断影响
 * 功能：             阻塞延时函数
 * 入口参数：      microsecs 微秒
 * *********************************************************************/
void mHD_timer_sleep (unsigned long seconds,unsigned long microsecs)
{
    struct timeval my_timeout;
    int err;
    my_timeout.tv_sec = seconds+ microsecs / 1000000L;
    my_timeout.tv_usec = microsecs%1000000L;
    do
    {
        err = select(0, NULL, NULL, NULL, &my_timeout);
    } while (err<0 && errno==EINTR);
}

//系统中断定时器
static void mHD_Sig_Timeout_Handle(int tmp)
{
    int i;
    if(tmp == SIGALRM)
    {
        //mHD_Systme_Handle();  //定时应用入口函数
        for(i=0;i<TONMAX;i++)
        {
            if((mTON[i].enable ==1)&&(mTON[i].et !=0))
            {
                mTON[i].et--;
                if(mTON[i].et == 0) mTON[i].enable =0;
            }
        }
        for(i=0;i<TOFFMAX;i++)
        {
            if((mTOFF[i].enable ==1)&&(mTOFF[i].et !=0))
            {
                mTOFF[i].et--;
                if(mTOFF[i].et == 0) mTOFF[i].enable =0;
            }
        }

    }
}
//注:liunx系统SIGALRM信号由alarm和setitimer产生,alarm为单次定时器,setitimer为循环定时器,
//seconds 秒 useconds 毫秒
//返回 -1 错误，返回0 正确
//工作机制:it_value倒计时--->it_value为0触发SIGALRM信号 ----->重置it_value为it_interval
//                            ^--------------------------------------------<-------------------------------------------^
static int m_TimerInitSigaction(unsigned int seconds,unsigned int useconds)
{
    struct itimerval tick;
    signal(SIGALRM,mHD_Sig_Timeout_Handle);  //SIGALRM Alarm clock (POSIX) 注册当前进程SIGALRM处理代码
    //设置系统定时器的时间
    tick.it_value.tv_sec = seconds + useconds/1000000;
    tick.it_value.tv_usec = useconds%1000000;   //设置定时时间
    tick.it_interval = tick.it_value;  //设置循环定时时间
    if(setitimer(ITIMER_REAL, &tick, NULL)<0) return -1;
    else return 0;
}
/*************初始化(注册)一秒变化一次的定时器中断**************************************************
 * 名称：            mHD_TimerSecondChange_Init
 * 说明：            不受中断影响
 * 功能：             阻塞延时函数
 * 入口参数：      unsigned int useconds 微秒
 * return : 返回剩余的时间
 * *********************************************************************/
void mHD_Timer_SystemMaster_Init(unsigned int useconds)
{
    m_TimerInitSigaction(0,useconds);  //设置循环定时器为1000ms
}

/*************读取系统时间日期**************************************************
 * 名称：            mHD_GetTimeDay
 * 说明：            读取系统时间
 * 功能：            读取系统时间
 * 入口参数：     *td  系统时间结构体
 * *********************************************************************/
void mHD_GetTimeDay(mHD_TimeDay *td)
{
   time_t timep;
   struct tm *p;
   struct  timeb   mt;
   ftime(&mt);
   time(&timep);
   p=gmtime(&timep);

   td->tm_usec = mt.millitm;              //赋值毫秒
   td->tm_year = 1900+p->tm_year;   //年赋值
   td->tm_mon =1+p->tm_mon;         //月赋值
   td->tm_mday = p->tm_mday;        //日赋值
   td->tm_wday = p->tm_wday;         //周星期赋值
   td->tm_hour =p->tm_hour;           //小时赋值
   td->tm_min =p->tm_min;              //分赋值
   td->tm_sec = p->tm_sec;               //秒赋值
   td->tm_yday = p->tm_yday;          //天数赋值
   td->tm_isdst = p->tm_isdst;          //下令时赋值

   /* 打印时间信息到控制台 */
    if(HqDev_CmdSys.debug ==1)
    {
        printf("%04d/%02d/%02d ",td->tm_year, td->tm_mon,td->tm_mday);
        printf("%d - %02d:%02d:%02d:%04d\n", td->tm_wday, td->tm_hour, td->tm_min, td->tm_sec,td->tm_usec);
        printf("tm_yday =  %d\n", td->tm_yday);
        printf("tm_isdst =  %d\n", td->tm_isdst);
    }
}
/*************读取系统时间 微秒**************************************************
 * 名称：            mHD_GetTimeusec
 * 说明：            读取系统时间
 * 功能：            读取系统时间
 * 入口参数：     无
 * 出口参数 :      返回 1970-01-01 00:00:00到现在经历的微秒数。
 * *********************************************************************/
unsigned  long long mHD_GetTimeusec(void)
{
    unsigned  long long time_use;
    struct  timeval  tv;
    gettimeofday(&tv,NULL);
    time_use = tv.tv_sec *1000000+tv.tv_usec;
    return time_use;
}

/*************设置系统时间日期**************************************************
 * 名称：            mHD_SetTimeDay
 * 说明：            设置系统时间
 * 功能：            设置系统时间
 * 入口参数：     年,月,日  时,分,秒
 * *********************************************************************/
void mHD_SetTimeDay(int y,int m,int day,int h,int min,int sec)
{
    time_t t;
    struct tm nowtime;
    struct timeval tv;

    nowtime.tm_sec = sec;
    nowtime.tm_min = min;
    nowtime.tm_hour = h;
    nowtime.tm_mday = day;
    nowtime.tm_mon = m-1;       //月份（从一月开始，0代表一月） - 取值区间为[0,11] ，所以这里要减1*/
    nowtime.tm_year = y-1900;  //年份，其值等于实际年份减去1900 */
    nowtime.tm_isdst = 0;           //夏令时, 不启用

    t = mktime(&nowtime);  //写入时间到系统
    tv.tv_sec = t;
    tv.tv_usec = 0;
    settimeofday (&tv, (struct timezone *) 0);
    system("hwclock -w");   //强制写入时间到RTC
}
/*************开延时定时器*******************************************
 * 名称：            mHD_TimeTON
 * 说明：            接通延时定时器,延时精度最大+-90ms
 * 功能：            接通延时定时器
 * 入口参数：     num定时器编号(最大值可设置),
 *                         in  上升延激活,未到达定时值下降定时器复位
 *                         pt  定时设定时间
 *                         et  剩余时间
 *                         reset 复位，为1复位
 * 出口参数 :      返回-1 定时器复位或未启动，返回0 定时器启动但未到达设定时间，
 *                         返回 1 定时器到达设定时间,  返回-2 定时器超限
 * 工作机制：当in出现上升延定时器开始延时,返回值为0,到达设定时间后返回值变为1,
 *                    未到达设定时间 in非1,定时器复位，返回值为-1;
 *                    任何时刻reset为1 定时器都将停止工作并复位
 * *********************************************************************/
int mHD_TimeTON(int num,int in,uint32_t pt,uint32_t *et,int reset)
{
    if((num<0)&&(num>TONMAX-1))  return mTON[num].re = TOver;   //定时器超限 -2
    if((reset ==1) ||(in !=1)) //复位定时器
    {
        mTON[num].enable = 0;   //定时器未启用
        mTON[num].status =0;  //定时状态未启动
        mTON[num].et = mTON[num].pt = pt*10;    //剩余时间 = 设定时间
        *et = pt;
        return  mTON[num].re = TDisEnable;          //定时器未启动
    }
    if((mTON[num].savein != in) &&(in ==1))  //判断in 由非1变为1 启动定时器
    {
        mTON[num].savein = in;   //保存in状态
        mTON[num].enable = 1;   //定时器启用
        mTON[num].status =1;    //启用定时器开始计时
        mTON[num].et = mTON[num].pt = pt*10;    //剩余时间 = 设定时间
        *et = pt;
    }
    if((mTON[num].et ==0) &&(mTON[num].status ==1))
    {
        *et = mTON[num].et/10;
        return mTON[num].re = TComplete;
    } else if((mTON[num].et !=0) &&(mTON[num].status ==1))
    {
        *et = mTON[num].et/10;
        return mTON[num].re = TEnable;
    } else
    {
        *et = pt;
        return mTON[num].re = TDisEnable;          //定时器未启动
    }
}
/*************关延时定时器*******************************************
 * 名称：            mHD_TimeTON
 * 说明：            接通延时定时器,延时精度最大+-90ms
 * 功能：            接通延时定时器
 * 入口参数：     num定时器编号(最大值可设置),
 *                         in  下降延激活,未到达定时值上升延定时器复位
 *                         pt  定时设定时间
 *                         et  剩余时间
 *                         reset 复位，为1复位
 * 出口参数 :      返回-1 定时器复位或未启动，返回1 定时器启动但未到达设定时间，
 *                         返回 0 定时器到达设定时间, 返回-2 定时器超限
 * 工作机制：当in出现下降延定时器开始延时,返回值为1,到达设定时间后返回值变为0,
 *                    未到达设定时间 in非0,定时器复位，返回值为1;
 *                    任何时刻reset为1 定时器都将停止工作并复位
 * *********************************************************************/
int mHD_TimeTOFF(int num,int in,uint32_t pt,uint32_t *et,int reset)
{
    if((num<0)&&(num>TONMAX-1))  return mTOFF[num].re = TOver;   //定时器超限 -2
    if((reset ==1) ||(in != 0)) //复位定时器
    {
        mTOFF[num].enable = 0;   //定时器未启用
        mTOFF[num].status =0;  //定时状态未启动
        mTOFF[num].et = mTOFF[num].pt = pt*10;    //剩余时间 = 设定时间
        *et = pt;
        return  mTOFF[num].re = TON;          //定时器未启动
    }
    if((mTOFF[num].savein != in) &&(in ==0))  //判断in 由非0变为0 启动定时器
    {
        mTOFF[num].savein = in;   //保存in状态
        mTOFF[num].enable = 1;   //定时器启用
        mTOFF[num].status =1;    //启用定时器开始计时
        mTOFF[num].et = mTOFF[num].pt = pt*10;    //剩余时间 = 设定时间
        *et = pt;
    }
    if((mTOFF[num].et ==0) &&(mTOFF[num].status ==1))
    {
        *et = mTOFF[num].et/10;
        return mTOFF[num].re = TOFF;
    } else if((mTOFF[num].et !=0) &&(mTOFF[num].status ==1))
    {
        *et = mTOFF[num].et/10;
         return mTOFF[num].re = TON;
    } else
    {
        *et = pt;
        return mTOFF[num].re = TON;          //定时器未启动
    }
}

/*************初始化开延时定时器*******************************************
 * 名称：            mHD_TimeTON_Init
 * 说明：            初始化开延时定时器状态
 * 功能：            接通延时定时器初始化
 * 入口参数：    无
 * 出口参数 :      无
 * *********************************************************************/
void mHD_TimeTON_Init(void)
{
    int i;
    for(i=0;i<TONMAX;i++) {
        mTON[i].enable =0;  mTON[i].status =0;  mTON[i].num =0;
        mTON[i].et = mTON[i].pt =0;  mTON[i].re = TDisEnable;
        mTON[i].status =0;
        mTON[i].savein =0;
    }
    for(i=0;i<TOFFMAX;i++) {
        mTOFF[i].enable =0;  mTOFF[i].status =0;  mTOFF[i].num =0;
        mTOFF[i].et = mTON[i].pt =0;   mTOFF[i].re = TDisEnable;
        mTOFF[i].status =0;
        mTOFF[i].savein =1;
    }
}




