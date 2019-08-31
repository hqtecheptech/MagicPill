
#include    <linux/input.h>
#include    <string.h>
#include    <stdio.h>           /*标准输入输出定义*/
#include    <stdlib.h>          /*标准函数库定义*/
#include    <unistd.h>         /*Unix标准函数定义*/
#include    <sys/types.h>   /**/
#include    <sys/stat.h>      /**/
#include    <fcntl.h>           /*文件控制定义*/
#include    <errno.h>         /*错误号定义*/
#include     <termios.h>    /*PPSIX终端控制定义*/
#include     <stdint.h>      /*数据类型宏定义 */

#include "../inc/Uart_Port.h"
#include "../inc/Public_DataType.h"


/*************打开串口 **************************************************
 * 名称：             mHD_Uart__Open
 * 功能：             打开串口并返回串口设备文件描述
 * 入口参数：      char * port      串口号(ttyS0,ttyS1,ttyS3,ttyS4,ttyS5,ttyS6,ttyS7)
 * 出口参数：串口设备文件描述，错误返回为-1
 * return : FALSE -1  错误，正确返回文件描述符
 * 打开流程  开始->调用open()函数打开串口->获取串口状态->(未阻塞)测试是否为串口终端->结束
 *                                                                           |                                     |
 *                                                                           ->阻塞(返回错误)           ->非终端(返回错误)
 * *********************************************************************/
int  mHD_Uart_Open(char *port)
{
    int fd;
    fd = open(port,O_RDWR|O_NOCTTY|O_NDELAY) ;
    //fd = open(port,O_RDWR|O_NOCTTY|O_NONBLOCK) ;
    if (fd<0)
    {
        perror("Can't Open Serial Port");
        return(FALSE);
    }
//    //判断串口的状态是否为阻塞状态
//    if(fcntl(fd, F_SETFL, 0) < 0) return(FALSE);
//    //测试是否为终端设备
//    if(0 == isatty(STDIN_FILENO)) return(FALSE);
   return fd;
}

/*************关闭串口 *****************
 * 名称：mHD_Uart__Close
 * 功能：关闭串口
 * 入口参数：int fd 文件描述符
 * 打开参数：
 * 出口参数：void
 * ****************************************/
void mHD_Uart_Close(int fd)
{
    close(fd);
}

/**************串口参数设置**************************************
*名称：             mHD_Uart_Set
*功能：             设置串口波特率 数据位，停止位和效验位
*入口参数：      fd          串口文件描述符
*                         speed       串口速度
*                         flow_ctrl   数据流控制 取值 0 无流控制,1 硬件流控制, 2软件流控制
*                         databits    数据位   取值为 7 或者8
*                         stopbits    停止位   取值为 1 或者2
*                         parity      效验类型 取值为N,E,O,,S
*出口参数：正确返回为0，错误返回为-1
* 设置流程：  开始 -> 设置波特率 -> 设置数据流控制 -> 设置帧的格式(即数据位个数，停止位，校验位)
*******************************************************************/
static int mHD_Uart_Set (int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    uint8_t i;
    int   speed_arr[] = { B115200,B57600, B38400,B19200, B9600, B4800, B2400, B1200};
    int   name_arr[] = {115200,57600,38400,19200,9600,4800,2400,1200};
    struct termios options;
    /*  tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，
            该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.  */
    if( tcgetattr( fd,&options)  !=  0)  return(FALSE);
    for (i= 0;i < sizeof(speed_arr) /sizeof(int);  i++)    //设置串口输入波特率和输出波特率
    {
        if  (speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);    //设置输入波特率
            cfsetospeed(&options, speed_arr[i]);   //设置输出波特率
        }
    }
    options.c_cflag |= CLOCAL; //忽略信号线路状态
    options.c_cflag |= CREAD;      //接收使能
    switch(flow_ctrl)    //设置数据流控制
    {
    case 0:     options.c_cflag &= ~CRTSCTS;  break;                           //不使用流控制
    case 1:     options.c_cflag |= CRTSCTS; break;                              //使用硬件流控制
    case 2:     options.c_cflag |= IXON | IXOFF | IXANY; break;     //使用软件流控制
    default:    options.c_cflag &= ~CRTSCTS;  break;                        //不使用流控制
    }
    options.c_cflag &= ~CSIZE;  //屏蔽其他标志位
    switch (databits)    //设置数据位
    {
    case 5:     options.c_cflag |= CS5;   break;   //5位数据位
    case 6:     options.c_cflag |= CS6;   break;   //6位数据位
    case 7:     options.c_cflag |= CS7;   break;   //7位数据位
    case 8:     options.c_cflag |= CS8;   break;   //8位数据位
    default:    options.c_cflag |= CS8;   break;   //8位数据位
    }
    switch (parity)     //设置校验位
    {
    case 'n':
    case 'N': //无奇偶校验位。
        options.c_cflag &= ~PARENB;    //PARENB = 进行奇偶校验
        options.c_iflag &= ~INPCK;        //INPCK =打开输入奇偶校验
        break;
    case 'o':
    case 'O'://设置为奇校验   PARODD = 奇校验否则为偶校验
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E'://设置为偶校验
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 's':
    case 'S': //设置为空格
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    }
    switch (stopbits)    // 设置停止位   CSTOPB 2位停止位否则为1位停止位
    {
    case 1:  options.c_cflag &= ~CSTOPB; break;
    case 2:  options.c_cflag |= CSTOPB; break;
    default:  options.c_cflag &= ~CSTOPB; break;
    }
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //关闭规范输入,回送，可见搽除字符,终端产生的信号
    options.c_oflag &= ~OPOST;  //修改输出模式，原始数据输出
    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 0; // 读取一个字符等待1*(1/10)s
    options.c_cc[VMIN] = 1; // 读取字符的最少个数为1
    tcflush(fd,TCIFLUSH);   //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)   return (FALSE);
    return (TRUE);
}

/******************初始化串口**************************************
*名称：            mHD_Uart_Init()
*功能：           串口初始化
*入口参数：    fd         文件描述符
*                      speed      串口速度
*                      flow_ctrl  数据流控制 取值 0 无流控制,1 硬件流控制, 2软件流控制
*                      databits   数据位   取值为 7 或者8
*                      stopbits   停止位   取值为 1 或者2
*                      parity     效验类型 取值为N,E,O,,S
*出口参数：FALSE -1  错误，正确返回0
*******************************************************************/
int mHD_Uart_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    if (mHD_Uart_Set(fd,speed,flow_ctrl,databits,stopbits, parity) == FALSE)  //设置串口数据帧格式
    {
        return FALSE;
    }
    else
    {
        return  TRUE;
    }
}
/****************串口接收数据************************************
* 名称：            mHD_Uart_Recv
* 功能：            接收串口数据
* 入口参数：        fd         文件描述符
*                            rcv_buf    接收串口中数据存入rcv_buf缓冲区中
*                            data_len   一帧数据的长度
* 出口参数：        正确返回为接收到数据数量，错误返回为0
*******************************************************************/
int mHD_Uart_Recv(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;
    struct timeval time;

    FD_ZERO(&fs_read);      //指定的文件描述符集清空
    FD_SET(fd,&fs_read);    //用于在文件描述符集合中增加一个新的文件描述符

    time.tv_sec = 0;
    time.tv_usec = 120;

    //使用select实现串口的多路通信
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel)
    {
        len = read(fd,rcv_buf,data_len);
        return len;
    }
    else
    {
        return FALSE;
    }
}

/****************串口发送数据****************************************
* 名称：            mHD_Uart_Send
* 功能：            发送数据
* 入口参数：        fd           文件描述符
*                            send_buf     存放串口发送数据
*                            data_len     一帧数据的个数
* 出口参数：        正确返回为发送数据长度，错误返回为-1
*******************************************************************/
int mHD_Uart_Send(int fd, char *send_buf,int data_len)
{
    int len = 0;

    len = write(fd,send_buf,data_len);
    if (len == data_len )    return len;
    else
    {
        tcflush(fd,TCOFLUSH);   //输出缓冲器清空
        return FALSE;
    }
}
/****************测试串口收发数据****************************************
* 名称：            mHD_Uart_RSTest
* 功能：            测试串口收发数据
* 入口参数：        fd           文件描述符
*                            send_buf     存放串口发送数据
*                            data_len     一帧数据的个数
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
void mHD_Uart_RSTest(char *port)
{
    int fd = -1;           //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
    int err;               //返回调用函数的状态
    int len;
    char rcv_buf[256];
    char send_buf[256] =  "Please enter data";;

    fd = mHD_Uart_Open(port);  //打开串口
    if(fd<0 )
    {
         if(HqDev_CmdSys.debug ==1) printf("Can't Open Serial Port");
         return;
    }

    err = mHD_Uart_Init(fd,115200,0,8,1,'N'); //初始化串口
    if(err <0) {printf("set serial parameter error!\n"); return;}

   //发送 输入提示符号
    len =  mHD_Uart_Send(fd,send_buf,strlen(send_buf));
    if(len > 0)
    {
         if(HqDev_CmdSys.debug ==1) printf("send %d data successful!\n",len);
    }
    else
    {
         if(HqDev_CmdSys.debug ==1) printf("send data failed!\n");
    }
    sleep(1);

    while(1)
    {
        len = mHD_Uart_Recv(fd, rcv_buf,sizeof(rcv_buf));
        if(len > 0)
        {
            rcv_buf[len] = '\0';
             if(HqDev_CmdSys.debug ==1) printf("num = %d receive data is %s\n",len ,rcv_buf);
            usleep(150000);
            len =  mHD_Uart_Send(fd,rcv_buf,len);
             if(HqDev_CmdSys.debug ==1) printf("num = %d txsend data is %s\n",len,rcv_buf);
        }
    }
}



