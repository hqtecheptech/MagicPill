#ifndef __UART_PORT_H
#define __UART_PORT_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define FALSE  -1
#define TRUE   0

 int  mHD_Uart_Open(char *port);  //打开串口  char * port  串口号(ttyS0,ttyS1,ttyS3,ttyS4,ttyS5,ttyS6,ttyS7) return : FALSE -1  错误，正确返回文件描述符
 void mHD_Uart_Close(int fd);  //关闭串口 int fd 文件描述符
 /************mHD_Uart_Init***********************
  *fd 文件描述符 speed 串口速度, flow_ctrl  数据流控制 取值 0 无流控制,1 硬件流控制, 2软件流控制
  *databits   数据位   取值为 7 或者8, stopbits   停止位   取值为 1 或者2,stopbits   停止位   取值为 1 或者2
  *返回 FALSE -1  错误，正确返回0
********************************************************/
int mHD_Uart_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity,uint8_t wtime,uint8_t wlen);  //初始化串口
 /****************mHD_Uart_Recv************************************
 * fd 文件描述符,  rcv_buf 接收串口数据缓冲区,data_len 接收长度
 * 返回：正确返回为接收到数据数量，错误返回为0
 *******************************************************************/
 int mHD_Uart_Recv(int fd, char *rcv_buf,int data_len);  //串口接收数据

 /****************mHD_Uart_Send****************************************
 * fd 文件描述符,send_buf     存放串口发送数据,data_len     一帧数据的个数
 * 返回：正确返回为发送数据长度，错误返回为-1
 *******************************************************************/
 int mHD_Uart_Send(int fd, char *send_buf,int data_len);  //串口发送数据

 /*** 读取指定长度的数据 ***********/
 int m_Uart_Safe_Recv_FixLen(int fd,void *buf,int n);










#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
