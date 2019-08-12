#ifndef __DEV_CONF_H
#define __DEV_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//#define  HQ_FPJ
#define  HQ_YXC
//#define  HQ_HLXT






#include "local_inc/mapp_init.h"
#include  "../mhd_lib/inc/mhd_lib.h"
#include "local_inc/CommFun.h"
#include "local_inc/keyboard_input.h"
#include"local_inc/MPUCtrDev.h"

#if defined HQ_FPJ
#include "Dev/FPJ/FPJ.h"


#elif defined HQ_YXC
#include "Dev/YXC/YXC.h"

#elif defined HQ_HLXT
 #include "Dev/HLJ/HLXT.h"


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

