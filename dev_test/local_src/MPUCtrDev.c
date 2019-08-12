#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#include"local_inc/MPUCtrDev.h"
#include "local_inc/mapp.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/CommFun.h"
#include "local_inc/keyboard_input.h"



/******1.0 MPU GPIO Channels Configuration****通道设置********
 * Features: Set input and output functions
 * parameter:
 *      int ch   Input and output value = in(0-6ch) ,out(2-7ch)
 *      char *inout  channels value = in ,out
 *      char *mode channels mode value =  gpio,pulse,encoder
 *      char *pull  Pull up and down value =   up,down
 * return： erro = -1, success = write byte number;
 * *******************************************************/
int mHD_Write_MPUGPIO_Config(int ch,char *inout,char *mode,char *pull)
{
    int fd;
    int wlen;
    char buf[64] = {'\0'};
    char tbuf[64] = {'\0'};
    fd= open("/dev/hq_gpio_set", O_WRONLY);  //打开配置模块
    if(fd ==-1) return -1;  //打开失败返回
    strcat(buf,inout);                //buf = "in or out"
    sprintf(tbuf,"[%d]",ch);      //[1]        注:格式化字符串复制(以字符的格式存在),参数第二位后的格式 同 printf
    strcat(buf,tbuf);                 //buf = "in[1]"
    strcat(buf," ");                    //buf = "in[1] "
    strcat(buf,mode);              //buf = "in[1] gpio"
    strcat(buf," ");                    //buf = "in[1] gpio "
    strcat(buf,pull);                 //buf = "in[1] gpio down"
    strcat(buf,"\n");                 //buf = "in[1] gpio down\n"
    wlen = write(fd,buf, strlen(buf));  //写设置参数到hq_gpio_set
    close(fd);  //关闭模块
    return wlen;
}
/********1.1 MPU GPIO Channels output  ****通道输出***************
 * Features: Set the output value
 * parameter:
 *      int ch  Input and output value = out(0-7ch)
 *      chari* fun GPIO output function value =  "gpio"  "timer" "mmc0" "heartbeat""cpu0"
 *      int value  output value = 0 ,1
 * return： erro = -1, success = write byte number;
 * **********************************************************/
int mHD_Write_MPUGPIO_Value(int ch,char * fun,int value,int delayon,int delayoff)
{
    int fd;
    int wlen;
    char buf[64] = {'\0'};    // 保存的是文件的路径,open打开的是该路径下 的文件
    char vbuf[64] = {'\0'};   // 往文本中 要写入的 值
    if(strcmp(fun,MPU_DO_TIMER) ==0)
    {
        sprintf(buf,"/sys/class/leds/gpio_out%d/trigger",ch);
        fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开GPIO触发
        sprintf(vbuf,"timer");  wlen = write(fd,vbuf,strlen(vbuf));   close(fd); //写入time 功能

        sprintf(buf,"/sys/class/leds/gpio_out%d/delay_off",ch);
        fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开timer delay_off
        sprintf(vbuf,"%d",delayoff);  wlen = write(fd,vbuf,strlen(vbuf));  close(fd);  //写入delay_off 时间

        sprintf(buf,"/sys/class/leds/gpio_out%d/delay_on",ch);
        fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开timer delay_on
        sprintf(vbuf,"%d",delayon);  wlen = write(fd,vbuf,strlen(vbuf));   close(fd); //写入delay_on 时间
    }
    else if((strcmp(fun,MPU_DO_TF) ==0) ||(strcmp(fun,MPU_DO_HEART))||(strcmp(fun,MPU_DO_CPU)))
    {
        sprintf(buf,"/sys/class/leds/gpio_out%d/trigger",ch);
        fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开GPIO触发
        vbuf[0] = '\0'; strcat(vbuf,fun);  wlen = write(fd,vbuf,strlen(vbuf));   close(fd); //写入tf,heart,cpu 功能
    }
    else if(strcmp(fun,MPU_DO_GPIO) ==0)
    {
        sprintf(buf,"/sys/class/leds/gpio_out%d/brightness",ch);  //写入通道的字符串
        fd= open(buf, O_WRONLY);   //打开GPIO通道
        if(fd == -1) return -1;                    //打开失败返回
        sprintf(vbuf,"%d",value);                   //写入值  字符的形式存在
        wlen = write(fd,vbuf,strlen(vbuf));    //写入通道
        close(fd);  //关闭GPIO通道
    }
    return wlen;
}
/******** MPU GPIO Channels input  *******************
 * Features: read GPIO  value
 * parameter:
 *      int ch  Input and output value = in(0-6ch)
 *      int value  output value = 0 ,1
 * return： erro = -1, success = input value
 * *******************************************/
int mHD_Read_MPUGPIO_Value(uint8_t ch)
{

    //int ch ={192,191,103,102,93,92};  //通道GPIO编号
     return  mHD_Read_GPIO(ch);       //读取GPIO 输入状态
}
/******** MPU Pulse Channels SET  ******脉冲通道设置*************
 * Features: Set the output Pulse Funtion
 * parameter:
 *      int ch  Input and output value = out(0-7ch)
 *      int exp     1= install,0=uninstall
 * return： erro = -1, success = write byte number;
 * ***********************************************************/
int mHD_Write_MPUPulse_Config(int ch,int exp)
{
    const int mch[8]= {0,0,6,9,7,3,4,1};
    int fd;

    char buf[64] = {'\0'};
    char vbuf[64] = {'\0'};

    if(ch<2) return -1;
    if(exp ==0)
    {
        sprintf(buf,"/sys/class/pwm/pwmchip%d/unexport",mch[ch]);
        fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开Plus通道
        sprintf(vbuf,"0");
        write(fd,vbuf,strlen(vbuf));    //卸载通道Pulse 功能   往unexport写如0
        close(fd);  //关闭操作通道
    }  else
        {
            sprintf(buf,"/sys/class/pwm/pwmchip%d/export",mch[ch]);
            fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开Plus通道
            sprintf(vbuf,"0");
            write(fd,vbuf,strlen(vbuf));    //申请通道  往export写如0
            close(fd);  //关闭操作通道
        }
    return 0;
}
/******** MPU Pulse Channels output  *******脉冲通道输出************
 * Features: Set the output Pulse Enable
 * parameter:
 *      int ch  Input and output value = out(2-7ch)
 *      int enable  1=open,0=close
 *      int period  (ns)       周期
 *      int duty (%) Duty cycle    占空比
 *      int polarty    = normal(Negative output),     1 正常(负输出)
 *          极性        = inversed(Positive output)    0 颠倒(正输出)
 * return： erro = -1, success = write byte number;
 * *************************************************************/
int mHD_Write_MPUPulse_Value(int ch,int enable,uint32_t period ,uint8_t duty,uint8_t polarty)
{
    int fd;
    const int mch[8]= {0,0,6,9,7,3,4,1};
    char buf[64] = {'\0'};
    char vbuf[64] = {'\0'};
    uint32_t md_duty;

    if(ch<2) return -1;

    sprintf(buf,"/sys/class/pwm/pwmchip%d/pwm0/duty_cycle",mch[ch]);
    fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开占空比通道
    sprintf(vbuf,"0");  write(fd,vbuf,strlen(vbuf));            //复位占空比
    close(fd);  //关闭操作通道

    sprintf(buf,"/sys/class/pwm/pwmchip%d/pwm0/period",mch[ch]);
    fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开周期通道
    sprintf(vbuf,"%d",period);  write(fd,vbuf,strlen(vbuf));            //写入周期
    close(fd);  //关闭操作通道

    md_duty = period * ((int)duty/100.0);   //计算占空比
    sprintf(buf,"/sys/class/pwm/pwmchip%d/pwm0/duty_cycle",mch[ch]);
    fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开占空比通道
    sprintf(vbuf,"%d",md_duty);  write(fd,vbuf,strlen(vbuf));    //写入占空比
    close(fd);  //关闭操作通道

    vbuf[0] = '\0';
    sprintf(buf,"/sys/class/pwm/pwmchip%d/pwm0/polarity",mch[ch]);
    fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开极性设置
    if( polarty ==0) strcat(vbuf,"inversedl");
    else strcat(vbuf,"normal");
    write(fd,vbuf,strlen(vbuf));            //写入极性
    close(fd);  //关闭操作通道

    sprintf(buf,"/sys/class/pwm/pwmchip%d/pwm0/enable",mch[ch]);
    fd= open(buf, O_WRONLY);   if(fd == -1) return -1;   //打开占空比通道
    if(enable ==0) write(fd,"0",1);    //关闭输出
    else write(fd,"1",1);    //打开输出
    close(fd);  //关闭操作通道

    return 0;
}

/******** MPU Pulse Channels output  *******************
 * Features: Set the output Pulse Enable
 * parameter:
 *      int ch  Input and output value = out(0-7ch)
 *      chari* fun GPIO output function value =  "gpio"  "timer" "mmc0" "heartbeat""cpu0"
 *      int value  output value = 0 ,1
 * return： erro = -1, success = write byte number;
 * *******************************************/
//int mHD_Write_MPUPulse_Value(int ch,int enable,int ,int delayoff)
//{

//}





// MPU输入输出功能初始化
int mHD_MPUGPIO_Init(void)
{
    return 0;
}

// MPU输入输出轮询
int mHD_MPUGPIO_Poll(void)
{
    return 0;
}


void mHD_MPUGPIO_Test(void)
{
    mHD_Write_MPUGPIO_Config(2,MPU_GPIO_OUT,MPU_MODE_GPIO,MPU_PULL_DOWN);
    while(1)
    {
//        mHD_Write_MPUGPIO_Value(2,1);
//        usleep(200000);
//        mHD_Write_MPUGPIO_Value(2,0);
//        usleep(200000);
    }
}

