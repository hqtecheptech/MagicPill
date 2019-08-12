
#ifndef _COMFUN_H
#define _COMFUN_H

#include <stdio.h>
#include <stdint.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>


int mHD_CHARtoHex(char cdata,uint8_t *hex);    //字符转化为 16#数值
uint8_t mHD_HexToChar(uint8_t hex,char *cdata) ;    //16#数值转化为ACSII码
uint8_t mHD_StrToHex(char *indata,uint32_t *outdata) ; //字符串转化为 16#数值

#endif





