#ifndef DATA_H
#define DATA_H

#include <QMetaType>

//char *m_shmkey ="/home/hqtech/test/shmkey";   //共享内存目录
//char *m_semkey ="/home/hqtech/test/semkey";   //消息队列目录
//char *m_msgkey ="/home/hqtech/test/msgkey";   //消息队列目录

/* Signals.  */
//SIGRTMIN = 34
//SIGRTMAX = 64
 //SIGINT
#define SIGFRPU     SIGRTMIN+1   //来自PRU的数据更新信号
#define SIGTRPU     SIGRTMIN+2   //发送给PRU的数据更新信号

/* DB datas length */
#define DB_FLOAT_LEN 200
#define DB_INT_LEN 200
#define DB_UINT16_LEN 500
#define DB_UINT8_LEN 200
#define DB_BOOL_LEN 200

typedef struct {
    float f_data[DB_FLOAT_LEN];
    int i_data[DB_INT_LEN];
    uint16_t dw_data[DB_UINT16_LEN];
    uint8_t w_data[DB_UINT8_LEN];
    uint8_t b_data[DB_BOOL_LEN];
 } Plc_Db;

typedef struct {
    bool isPruConnected;
    int pid;
    int cmd;
    int msgStatus;
} ControllerInfo;

Q_DECLARE_METATYPE(Plc_Db)

#endif // DATA_H
