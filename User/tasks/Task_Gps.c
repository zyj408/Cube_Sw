#include  <includes.h>

void GPS_REV_TASK(void *p_arg)
{	
	uint8_t data_temp;
	(void)p_arg;
	
	//printf("Task GPS Receive running \r\n");
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	comClearRxFifo(COM2);
	GpsRevCnt = 0;
	
	while(1)
	{
		BSP_OS_TimeDlyMs(10000);
		
		comClearRxFifo(COM2);
		GpsRevCnt = 0;	
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

		while(GpsRevCnt < GPS_MAX_REV_SIZE)
		{
			if(comGetChar(COM2, &data_temp))
			{
				GpsRevBuf[GpsRevCnt++] = data_temp;
			}
		}
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		GPS_Analysis(&GpsCurInfo, GpsRevBuf);

	}
}

void GPS_STO_TASK(void *p_arg)
{
	(void)p_arg;
	
	//printf("Task GPS Store running \r\n");
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

void GPSOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	
}
