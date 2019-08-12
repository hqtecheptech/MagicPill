
#ifndef _FILE_READANDWRITE_H
#define _FILE_READANDWRITE_H

#include<string.h>
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>

typedef struct {
    uint8_t     bitnum;
    uint16_t   name;
    uint8_t     ver;
    uint8_t     setdata[64];
    uint16_t   esturn[12];
} Mod_set;

typedef struct {
    uint8_t  Devbaud;
    uint8_t  Modbaud;
} Mod_system;


extern Mod_system M_SetSysData;
extern Mod_set M_SetData[];

uint8_t mHD_MPU_Read_SetFile(void);

#endif
