#include <linux/input.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>   //define O_WRONLY and O_RDONLY

#include "../inc/mhd_lib.h"

//申请GPOIO
//返回 0成功 -1 失败
static int mHD_Request_GPIO(int gpio)
{
    FILE * fp;

    fp =fopen("/sys/class/gpio/export","w");
    if (fp == NULL)   return -1;
    else  fprintf(fp,"%d",gpio);
    fclose(fp);
    return 0;
}

//设置GPIO方向
//direction gpio "in"  "out"
//返回 0成功 -1 失败
static int mHD_SetGPIO_Direction(int gpio,char *direction)
{
    char path[100] = {0};
    FILE * fp;

   sprintf(path,"/sys/class/gpio/gpio%d/direction",gpio);
   fp =fopen(path,"w");
   if (fp == NULL)  return  -1;
   else
   {
       fprintf(fp,"%s",direction);
       fclose(fp);
   }
   return 0;
}

//读取GPIO状态
//返回  -1 失败  其他GPIO 状态
static int mHD_GetGPIO_Value(int gpio)
{
    char path[64];
    char value_str[3];
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(path, O_RDONLY);
    if (fd < 0)  return -1;

    if (read(fd, value_str, 3) < 0) return -1;
    close(fd);
    return (atoi(value_str));
}

//设置GPIO输出
//返回  -1 失败  0设置成功
int mHD_SetGPIO_Value(int gpio,int value)
{
    char path[64] ={0};
    FILE *fd=NULL;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    fd =fopen(path,"w");
    if (fd ==NULL)  return  -1;
    else
    {
        fprintf(fd,"%d",value);
        fclose(fd);
    }
    return 0;
}
//读取GPIO 输入状态
//返回 -1 错误，
int mHD_Read_GPIO(int gpio)
{
    int status;
    mHD_Request_GPIO(gpio);                         //申请GPIO
    mHD_SetGPIO_Direction(gpio,"in");           //设置GPIO方向为输入
    status =  mHD_GetGPIO_Value(gpio);        //读取GIPO状态
    return status;
}


