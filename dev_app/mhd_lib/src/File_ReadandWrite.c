/*****************************************************************
 *      Copyright:  (C) 2019 HQtech  All rights reserved.
 *       Filename:  File_ReadandWrite.c
 *       Description:  文件读写功能
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
#include <fcntl.h>

#include "../inc/File_ReadandWrite.h"

/************* 创建文件 **********************************************
 * 名称：             mHD_CreateOpen_File
 * 功能：             打开文件,如果文件不存在就创建文件
 * 入口参数：      pathname  文件路径及文件名
 * 出口参数：      串口设备文件描述，错误返回为-1
 * return :             FALSE -1 错误，文件已存在返回0, 文件不存在新创建成功返回 1;
 * ********************************************************************************/
int  mHD_Create_File(char *pathname)
{
    int fd;
    int cfd;

    fd = open(pathname,O_RDWR);
    {
        if(fd == -1)  //打开失败
        {
            //printf("File does not exist to create file\n");
            cfd = open(pathname,O_RDWR | O_CREAT, S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH); //打开文件不存在就创建它,读写权限
            if(cfd == -1)
            {
                //printf("Failed to create file\n");
                return -1;
            } else
            {
                //printf("Create file successfully\n");
                close(cfd); //关闭文件
                return 1;
            }
        } else
        {
            //printf("Open file successfully\n");
            close(fd); //关闭文件
            return 0;
        }
    }
}

/************* 打开文件 **********************************************
 * 名称：             mHD_Open_File
 * 功能：             打开文件,
 * 入口参数：      pathname  文件路径及文件名, modes 文本打开方式"r" "w"
 *                                                                                            二进制打开 "rb
 * 出口参数：      成功返回文件指针，错误返回NULL
 * return :             FALSE -1 错误,否则返回文件描述
 * ********************************************************************************/
FILE *  mHD_Open_File(char *pathname,char * modes)
{
    FILE  *fp;
    fp =fopen(pathname,modes);
    if(fp == NULL)
    {
        //printf("Failed to open file\n");
        return NULL;
    } else return fp;
}

/************* 按行读取文本文件内容 **********************************************
 * 名称：             mHD_Read_File_Row
 * 功能：             按行读取文件
 * 入口参数：      fd  文件描述   buf 读取缓存区 maxnum 最大读取数量
 * 出口参数：      返回buf的指针,或者NULL
 * return :            返回buf的指针,或者NULL
 * ********************************************************************************/
char *  mHD_ReadTXT_File_Row(FILE *fp,char *buf,int maxnum)
{
    return fgets(buf,maxnum-1,fp);

}

/************* 读取指定数量的数据 **********************************************
 * 名称：             mHD_Read_Data
 * 功能：             读取文件
 * 入口参数：      fp  文件流   buf 读取缓存区  datatype 数据大小以字节为单位
 *                          datalen 读取数量已字节为单位 startpos 读取位置 字节为单位
 * 出口参数：      错误返回为-1,否则返回读取到的数量
 * return :            读取到的数据个数,如果不等于datalen 表示出错或到达文件结尾
 * ********************************************************************************/
int  mHD_Read_Data(FILE *fp,void *buf,size_t datatype,size_t datalen,int startpos)
{
    int num;
    fseek(fp,startpos,SEEK_SET);  //定位文件位置

    num = fread(buf,datatype,datalen,fp);
    return num;
}

/************* 写入字符串到文件 **********************************************
 * 名称：             mHD_WriteTXT_File_String
 * 功能：             写入字符串到文件
 * 入口参数：      fd  文件描述   buf 字符串指针
 * 出口参数：      错误返回为-1,否则返回0
 * return :            FALSE -1  错误，正确返回0
 * ********************************************************************************/
int mHD_WriteTXT_File_String(FILE  *fp,char *buf)
{
    int i;
    char c;
    for(i=0;i<(int)strlen(buf);i++)
    {
        c = fputc(buf[i],fp);
        if(c == (char)EOF) return -1;
    }
    return 0;
}

/************* 写入指定数量的数据到文件 **********************************************
 * 名称：             mHD_Read_Data
 * 功能：             写入数据到文件
 * 入口参数：      fp  文件流   buf 写出数据缓存区  datatype 数据大小以字节为单位
 *                          datalen 读取数量已字节为单位 startpos 读取位置 字节为单位
 * 出口参数：      错误返回为-1,否则返回写入到文件的数据个数
 * return :            写入到文件的数据个数,如果不等于datalen 表示出错或到达文件结尾
 * ********************************************************************************/
int  mHD_Write_Data(FILE *fp,void *buf,size_t datatype,size_t datalen,int startpos)
{
    int num;
    fseek(fp,startpos,SEEK_SET);  //定位文件位置

    num = fwrite(buf,datatype,datalen,fp);
    return num;
}

/************* 关闭文件 **********************************************
 * 名称：             mHD_Close_File
 * 功能：             关闭文件
 * 入口参数：      fp  文件流   buf 写出数据缓存区  datatype 数据大小以字节为单位
 *                          datalen 读取数量已字节为单位 startpos 读取位置 字节为单位
 * 出口参数：      错误返回为-1,否则返回写入到文件的数据个数
 * return :            写入到文件的数据个数,如果不等于datalen 表示出错或到达文件结尾
 * ********************************************************************************/
void  mHD_Close_File(FILE *fp)
{
    fclose(fp);
}
