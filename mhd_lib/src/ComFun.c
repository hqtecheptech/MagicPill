

#include "../inc/mhd_lib.h"

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
