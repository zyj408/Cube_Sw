#include  <includes.h>

void GPS_REV_TASK(void *p_arg)
{	
	(void)p_arg;
	
	printf("Task GPS Receive running \r\n");
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

void GPS_STO_TASK(void *p_arg)
{
	(void)p_arg;
	
	printf("Task GPS Store running \r\n");
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

void GPSOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	
}
