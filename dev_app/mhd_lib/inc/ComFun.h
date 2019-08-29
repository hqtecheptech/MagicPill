
#ifndef _COMFUN_H
#define _COMFUN_H

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;

int mHD_CHARtoHex(char cdata,uint8_t *hex);    //字符转化为 16#数值
uint8_t mHD_HexToChar(uint8_t hex,char *cdata) ;    //16#数值转化为ACSII码
uint8_t mHD_StrToHex(char *indata,uint32_t *outdata) ; //字符串转化为 16#数值
int  mHD_Stringanalysis(char *tfg, char (* analysis)[128],char cut,char end); //循环字符串解析
int  mHD_CLibanalysis(char *tfg, char (* analysis)[128], char* cut); //使用C 的字符串 函数解析输入字符串
int mstrcmp(const char *ina,const char *inb); //比较字符串函数

#endif





