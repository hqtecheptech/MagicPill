#ifndef SHAREHELPER_H_INCLUDED
#define SHAREHELPER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <iconv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "data.h"

using namespace std;

class ShareHelper
{
public:
    ShareHelper(key_t shared_key);
    ~ShareHelper();
    bool SetSharedStr(string str);//修改共享内存中的字符串
    string GetSharedStr();//获取共享内存中的字符串
    void DeleteSharedMemory(); //Clear shared memory
    void LockShare();//锁定临界区
    void UnlockShare();//解锁临界区
    bool SetSharedMemory(PlcData* data);
    PlcData GetShardMemory();

private:
    int DelSem();//删除信号量
    int PSem();//信号量的P操作，信号量加1
    int VSem();//信号量的V操作，信号量减1

    int shmid;//共享内存id
    int semid;//共享信号量id
    void *shmptr;//共享内存区域

    key_t sharedKey;
};

#endif // SHAREHELPER_H_INCLUDED
