
#ifndef _FILE_READANDWRITE_H
#define _FILE_READANDWRITE_H

#define M_TXTREAD    "rt"       //只读方式打开文本文件
#define M_TXTRWITE   "rt+"    //读写方式打开文本文件
#define M_DATREAD    "rb"       //只读方式打开文本文件
#define M_DATRWITE   "rb+"    //读写方式打开文本文件

#include <stdio.h>

int  mHD_Create_File(char *pathname); //创建文件
FILE *  mHD_Open_File(char *pathname,char * modes);  // 打开文件
char *  mHD_ReadTXT_File_Row(FILE *fp,char *buf,int maxnum) ; //按行读取文本文件内容
int  mHD_Read_Data(FILE *fp,void *buf,size_t datatype,size_t datalen,int startpos); //读取指定数量的数据
int mHD_WriteTXT_File_String(FILE  *fp,char *buf); //写入字符串到文件
int  mHD_Write_Data(FILE *fp,void *buf,size_t datatype,size_t datalen,int startpos); //写入指定数量的数据到文件
void  mHD_Close_File(FILE *fp); //关闭文件


#endif
