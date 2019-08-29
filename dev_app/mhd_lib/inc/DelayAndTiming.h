#ifndef __DELAYANDTIMING_H
#define __DELAYANDTIMING_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define TONMAX  32             //接通延时定时器最大数量
#define TOFFMAX  32            //断开延时定时器最大数量

 typedef struct
 {
     int tm_usec;  //代表目前毫秒数，正常范围为0-1000，
     int tm_sec;   //代表目前秒数，正常范围为0-59，但允许至61秒
     int tm_min;  //代表目前分数，范围0-59
     int tm_hour; //从午夜算起的时数，范围为0-23
     int tm_mday; //目前月份的日数，范围01-31
     int tm_mon;  //代表目前月份，从一月算起，范围从0-11
     int tm_year;  //从1900 年算起至今的年数
     int tm_wday; //一星期的日数，从星期一算起，范围为0-6
     int tm_yday;  //从今年1月1日算起至今的天数，范围为0-365
     int tm_isdst;  //夏令时
 } mHD_TimeDay;

 typedef enum {
      TOver = -2,               //定时器超限
      TDisEnable = -1,      //定时器未启动
      TEnable = 0,            //定时器定时阶段
      TComplete =1,        //定时器到达设定时间
      TON =1,                   //延时断开输出开
      TOFF =0,                  //延时断开输出关
  } TimerONF;

 typedef struct
 {
     int  num;  //定时器编号
     int  savein;     //上一次输入状态
     volatile int enable;    //定时器状态 0=未启用，1=启用
     volatile unsigned  long  pt;      //设定时间
     volatile unsigned  long  et;      //剩余时间
     int status;    //定时器定时状态  0=未启动，1= 启动
     int re;           //返回值；
 } mHD_TimerON;

/*** 阻塞延时函数 **************************/
void mHD_sleep (unsigned long seconds);   //阻塞延时函数 秒级 s
void mHD_msleep(unsigned long millisecs);      //阻塞延时函数 毫级 ms
void mHD_usleep(unsigned long microsecs);    //阻塞延时函数 微秒级us
void mHD_timer_sleep (unsigned long seconds,unsigned long microsecs);  //阻塞延时函数,分别设置秒及微妙

/*** 系统时间函数 **************************/
void mHD_GetTimeDay(mHD_TimeDay *td); //读取系统时间日期
void mHD_SetTimeDay(int y,int m,int day,int h,int min,int sec); //设置系统时间日期  年,月,日,时,分,秒
unsigned  long long mHD_GetTimeusec(void); //读取系统时间返回1970-01-01 00:00:00到现在经历的微秒数

/*** 定时器函数 **************************/
void mHD_TimeTON_Init(void); //初始化开延时定时器
int mHD_TimeTON(int num,int in,uint32_t pt,uint32_t *et,int reset);  //开延时定时器
int mHD_TimeTOFF(int num,int in,uint32_t pt,uint32_t *et,int reset); //关延时定时器

/****系统定时器 100ms **/
void mHD_Timer_SystemMaster_Init(unsigned int useconds);  //初始化(注册) 100ms变化一次的定时器中断













#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
