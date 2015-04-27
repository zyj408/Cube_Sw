#ifndef  __BSP_RECEIVEDEAL_H__
#define  __BSP_RECEIVEDEAL_H__

#include <includes.h>

enum INS_STATUS 
{
	INS_IDLE,
	INS_SNYC,
	INS_RCV,
};

CPU_INT08U InsDecode(uint8_t *InsBuf);
CPU_INT16U GetCheckSum(CPU_INT16U *Ptr, uint8_t BufSize);
void GndTsRxHandle(void);
#endif

