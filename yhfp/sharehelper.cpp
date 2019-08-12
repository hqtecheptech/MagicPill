#include "sharehelper.h"

//key_t sharedKey = 99;
size_t maxSize = 200;

union semum
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}arg;

ShareHelper::ShareHelper(key_t shared_key)
{
    sharedKey = shared_key;

    shmid = shmget(sharedKey, maxSize, IPC_CREAT | 0777);
    if (shmid == -1)
    {
        std::cout << "Shared memory created error";
    }
    shmptr = shmat(shmid, NULL, 0);
    if (shmptr == (void*)-1)
    {
        std::cout << "shmat error";
    }

    union semum sem;
    sem.val = 1;
    semid = semget(sharedKey, 1, IPC_CREAT | 0777);
    if (semid == -1)
    {
        std::cout << "Create semaphore error";
    }
    semctl(semid, 0, SETVAL, sem);
}

ShareHelper::~ShareHelper()
{

}

bool ShareHelper::SetSharedStr(string str)
{
    int len = str.length();
    if (len >= maxSize)
        return false;
    strncpy((char *)shmptr, str.c_str(), len);
    *(char *)(shmptr + len) = '\0';
    return true;
}

string ShareHelper::GetSharedStr()
{
    string str((char *)shmptr);
    return str;
}

void ShareHelper::DeleteSharedMemory()
{
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, 0);
    DelSem();
}

void ShareHelper::LockShare()
{
    PSem();
}

void ShareHelper::UnlockShare()
{
    VSem();
}

bool ShareHelper::SetSharedMemory(PlcData *data)
{
    memcpy(shmptr, (void*)data, 80);
    return true;
}

PlcData ShareHelper::GetShardMemory()
{
    PlcData allData;
    memcpy((void*)allData.values, shmptr, 80);
    return allData;
}

int ShareHelper::DelSem()
{
    union semum sem;
    sem.val = 0;
    semctl(semid, 0, IPC_RMID, sem);
    return 0;
}

int ShareHelper::PSem()
{
    struct sembuf sops = {0, -1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}

int ShareHelper::VSem()
{
    struct sembuf sops = {0, +1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}
