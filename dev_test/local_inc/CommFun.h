#ifndef __COMMFUN_H
#define __COMMFUN_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


 int  mHD_Stringanalysis(char *tfg, char (* analysis)[128],char cut,char end); //循环字符串解析
 int  mHD_CLibanalysis(char *tfg, char (* analysis)[128],char* cut); //使用C 的字符串 函数解析输入字符串
 int mstrcmp(const char *ina,const char *inb);  //比较字符串函数



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
