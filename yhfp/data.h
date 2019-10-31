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

// History chart figure
#define CP 30
#define CV 15
#define ALERT_COUNT 100

#define SIGFRPU     SIGRTMIN+1   //来自PRU的数据更新信号
#define SIGTRPU     SIGRTMIN+2   //发送给PRU的数据更新信号

/* DB datas length */
#define DB_FLOAT_LEN 96
#define DB_INT_LEN 32
#define DB_UINT32_LEN 128
#define DB_UINT16_LEN 128
#define DB_BOOL_LEN 320


typedef struct {
    uint32_t  dw_link[16];
    float f_data[DB_FLOAT_LEN];
    int i_data[DB_INT_LEN];
    uint32_t dw_data[DB_UINT32_LEN];
    uint16_t w_data[DB_UINT16_LEN];
    uint8_t b_data[DB_BOOL_LEN];
 } Plc_Db;

typedef struct {
    bool isPruConnected;
    int pid;
    int cmd;
    int msgStatus;
} ControllerInfo;

typedef struct {
    int deviceId;
    int deviceGroup;
    int deviceIndex;
    char name[50];
    char dataType[5];
    float address;
    char value[20];
    int index;
    char insertTime[30];
} HistData;

typedef enum {
    Horizontal,
    Vertical
} WatchDirection;

Q_DECLARE_METATYPE(Plc_Db)
Q_DECLARE_METATYPE(HistData)

#endif // DATA_H
