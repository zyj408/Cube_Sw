#ifndef  __BSP_RECEIVEDEAL_H__
#define  __BSP_RECEIVEDEAL_H__

#include <includes.h>

enum INS_STATUS 
{
	INS_IDLE,
	INS_SNYC,
	INS_RCV,
};
enum MAG_TAB_STATUS
{
	MAG_IDLE,
	MAG_INJECT,
	MAG_DONE,
};
CPU_INT08U InsDecode(uint8_t *InsBuf);
void InsGetCheckSum(uint8_t *Ptr, uint8_t buffsize, uint8_t *checksum);
void GndTsRxHandle(void);
#endif

