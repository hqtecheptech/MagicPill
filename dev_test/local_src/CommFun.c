#include "local_inc/CommFun.h"
#include  "../mhd_lib/inc/mhd_lib.h"

/*************循环字符串解析 *****************
 * char *tfg    输入字符串
 * char (* analysis)[128]  二维数组参数
 * int len        输入字符串的长度
 * char cut     分割符
 * char end    结束符
 * return   返回分割字符串的数量
 * ****************************************/
int  mHD_Stringanalysis(char *tfg, char (* analysis)[128],char cut,char end)
{
    int i,y =0,j=0,status=0;
    int len = strlen(tfg);

    for(i=0;i<len;i++)
    {
         if(status == 0)
         {
             if(tfg[i] != cut ) { status =1;j=0;}
         }

         if(status ==1 )
         {
              if((tfg[i] != cut ) &&(tfg[i] != end))analysis[y][j++] = tfg[i];
              else
               {
                   analysis[y][j] = '\0';
                   status =0;
                   //printf("%s\n",&analysis[y][0]);       //Test
                   y++;
               }
         }
    }
    //for(i=0;i<y;i++)  printf("%s\n",&analysis[i][0]);       //Test
    return y;
}

/**************** 使用C 的字符串 函数解析输入字符串 **************
 * char *tfg    输入字符串
 * char (* analysis)[128]  二维数组参数
 * char cut     分割符
 * return   返回分割字符串的数量
 * ****************************************/
int  mHD_CLibanalysis(char *tfg, char (* analysis)[128], char* cut)
{
    char tokens[128];
    int cnt =0;
    char * strv = tfg;

    char * token =strsep(&strv,cut);
    while(token !=NULL)
    {
        strcpy(tokens,token);
        if(tokens[0] != '\0')  //去除多于的空格
        {
            strcpy(&analysis[cnt][0],tokens);
            cnt++;
        }
        token = strsep(&strv,cut);
    }
     //for(i=0;i<cnt;i++)  printf("%s\n",&analysis[i][0]);       //Test
    return cnt;
}

/******* 比较字符串函数 ****************
 * char * ina  需要比较的字符1
 * char * inb 需要比较的字符2
 * int cnt  比较字符的个数，可选选项
 * 返回  -1 比较不相等 0  字符相等
 * ***********************************/
int mstrcmp(const char *ina,const char *inb)
{
    //int i;
    /********* 可以比较字符数组 **************/
//    for(i=0;i<cnt;i++)
//    {
//        //if(ina[i] != inb[i]) return -1;             //三选一
//        if(*(ina+i) != * (inb+i)) return -1;
//        if(memcmp(ina,inb,sizeof(char)*cnt) !=0) return -1;
//    }   return 0;
   /***********************************/

    /********* 比较字符串 ***************/
//    //step1 比长度是否相等
//    int inalen = strlen(ina);
//    int inblen = strlen(inb);
//    if(inalen != inblen)  return -1;
//    //step 2 比较字符是否相等
//    for(i=0;i<inalen;i++)
//    {
//        //if(ina[i] != inb[i]) return -1;             //二选一
//        if(*(ina+i) != * (inb+i)) return -1;
//    } return 0;
   /***********************************/

    /********* 调用C 库函数比较字符串 ***************/
   if(strcmp(ina,inb) !=0)  return -1;
   return 0;
   /*********************************************/

//   ********* 调用C库函数进行内存比较*************/
//    if(memcmp(ina,inb,sizeof(char)*cnt) !=0) return -1;
//    return 0;
//   /********************************************
}

