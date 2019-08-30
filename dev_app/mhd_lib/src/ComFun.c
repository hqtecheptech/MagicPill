

#include "../inc/mhd_lib.h"

#include <stdio.h>
#include <stdint.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

/**** ASCII To Hex ***************
 * char cdata   需要转换的字符
 * int *Hex 转换后的16进制数值
 *********************************/
int mHD_CHARtoHex(char cdata,uint8_t *hex)
{
  if( ( cdata >= '0' ) && ( cdata <= '9' ) )
  {
    *hex = cdata - '0';
    return 0;
  }
  else if( ( cdata >= 'A' ) && ( cdata <= 'F' ) )
  {
    *hex = cdata - 'A' + 0x0A;
    return 0;
  }
  else
  {
            return -1;
  }
}

/**** Hex To ASCII ******************
 * uint8_t hex   需要转换的16#数值
 * char *cdata   转换后的char字符 A -F 大写
 */
uint8_t mHD_HexToChar(uint8_t hex,char *cdata) {
    uint8_t hex_L,hex_H;

    hex_L = hex & 0x0F;
    hex_H = (hex>>4) & 0x0F;

    if(hex_H<=9)
    {
        *cdata = hex_H + '0';
    }else if((hex_H >=0x0A) && (hex_H<=0x0F))
    {
        *cdata = hex_H + 'A'-0x0A;
    }else
    {
        return 1;
    }

    if(hex_L <=9)
    {
      *(cdata+1) = hex_L + '0';
    }else if((hex_L >=0x0A) && (hex_L<=0x0F))
     {
       *(cdata+1) = hex_L + 'A'-0x0A;
     }else
      {
         return 1;
      }

    return 0;
}

/**** String To Hex ******************
 * char *indata          需要转换的字符串
 * uint32_t outdata   转换后的Hex数
 * 返回0 转换完成，返回1转换失败
 */
uint8_t mHD_StrToHex(char *indata,uint32_t *outdata) {
    int len;
    int i;
    uint8_t mdata =0;
    char m_indata;
    len = strlen(indata);
    if(len>7) return 1;
    if(len ==0) {
        *outdata =0;
        return 0;
    } else {
        *outdata =0;
        for(i=0;i<len;i++) {
            m_indata = *(indata+i);
            if(mHD_CHARtoHex(m_indata,&mdata) !=1)   *outdata = (*outdata<<4)|mdata;
            else return 1;
            }
        return 0;
        }
}

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
/*测试变量某一位是否为‘1’，是返回1，否返回0
 * num为需要修改的数，bit为位数，其值从0到7 */
uint8_t  mHD_Bit8Test(uint8_t  num,uint8_t bit)
{
    if(((num>>bit)&0x01)==1) return 1;
    else return 0;
}
/*设置位值 num为需要修改的数，bit为位数，其值从0到7 */
uint8_t  mHD_Bit8Set(uint8_t  num,uint8_t bit,uint8_t value)
{
    uint8_t  bit_value[]={1,2,4,8,16,32,64,128};
    if(value==0) return num&~bit_value[bit];
    else return num|bit_value[bit];
}
/*取反某一位 num为需要修改的数，bit为位数，其值从0到7*/
uint8_t mHD_Bit8Cpl(uint8_t  num,uint8_t  bit)
{
    uint8_t  bit_value[]={1,2,4,8,16,32,64,128};
    if(((num>>bit)&0x01)==1) return num&~bit_value[bit];
    else return num|bit_value[bit];
}

