/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  main.c
 *       Description:  控制系统软件入口函数
 *       Version:  1.0.0(08/24/2019)
 *       Author:   luozx  <luozxin@163.com>
 *       ChangeLog:  1, Release initial version on "08/24/2019 11:12 AM"
 ****************************************************************/


#include <stdio.h>
#include <stdint.h>

#include "dev_app_conf.h"  //配置文件

#include "SysInitRun/Sys_Init.h"
#include "SysInitRun/Sys_Run.h"


int  main(void)
{
    /*** 初始化系统 ***/
    mHD_Sys_Dev_Init();  //系统设备初始化
    /*** 添加应用初始化 ***/


    /*** 系统功能轮询 ***/
    while(1)
    {
        /*** 系统轮询任务  ***/
        mHD_Sys_Dev_Run(); //系统轮询任务
        /*** 添加应用轮询任务 ***/

    }

    return 0;
}

