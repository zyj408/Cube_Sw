#include <includes.h>
#include "globalavr.h"

void SPI2_IRQHandler(void)
{
	OSIntEnter();
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_CRCERR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_CRCERR);
		MotorCommErr++;
	}
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_OVR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_OVR);
		MotorCommErr++;
	}	
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_MODF) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_MODF);
		MotorCommErr++;
	}
	OSIntExit();
}

