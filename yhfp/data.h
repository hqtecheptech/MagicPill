#ifndef DATA_H
#define DATA_H

#include <QMetaType>
#include <signal.h>

// History chart figure
#define CP 30
#define CV 15
#define ALERT_COUNT 100

#define SIG_FROM_REMOTE     SIGRTMIN+2  //来自远程的数据更新信号
#define SIG_TO_REMOTE       SIGRTMIN+1   //发送给远程的数据更新信号

/* DB datas length */
#define DB_COMM_DATA 32
#define DB_FLOAT_LEN 320
#define DB_INT_LEN 128
#define DB_UINT32_LEN 128
#define DB_UINT16_LEN 8
#define DB_BOOL_LEN 400

#define CTR_BLOCK_LEN 4

typedef struct {
    int16_t fromPru[CTR_BLOCK_LEN];
    int16_t toPru[CTR_BLOCK_LEN];
} Ctr_Block;

typedef struct {
    uint32_t  dw_link[DB_COMM_DATA];
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
Q_DECLARE_METATYPE(Ctr_Block)

#endif // DATA_H
