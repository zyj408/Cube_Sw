#ifndef  __BSP_RECEIVEDEAL_H__
#define  __BSP_RECEIVEDEAL_H__

#include <includes.h>

CPU_INT08U InsDecode(char *InsBuf, CPU_INT16U BufSize);
CPU_INT16U GetCheckSum(CPU_INT16U *Ptr, CPU_INT08U BufSize);

#endif

