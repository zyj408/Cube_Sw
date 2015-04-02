#include  <includes.h>


void STO_UPDATE_TASK(void *p_arg)
{
	(void)p_arg;		/* 避免编译器告警 */
	
	while(1)
	{
		RTC_GetTime(RTC_Format_BIN, &CurTime);  /* 更新星上时间 */
		RTC_GetDate(RTC_Format_BIN, &CurDate);  /* 更新星上日期 */
		RandNum_32 = RNG_GetRandomNumber();     /* 真随机数 */
		
		IWDG_Feed();
		
		BSP_OS_TimeDlyMs(500);
	}
}

void STO_MONITOR_TASK(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

void ADC_SAMPLE_TASK(void *p_arg)
{
	CPU_INT08U round;
	(void)p_arg;		/* 避免编译器告警 */

	EpsAdStart();
	ObcAdStart();
	BSP_OS_TimeDlyMs(100);
	while(1)
	{
		for(round = 0; round < 16; round++)
		{
			EpsAdUpdate(EPS_AD_CS1);
			BSP_OS_TimeDlyMs(1);
			
			EpsAdUpdate(EPS_AD_CS2);
			AdDataFliter(EpsAdValue, 32);
			
			BSP_OS_TimeDlyMs(1);
			ObcAdUpdate();
			AdDataFliter(ObcAdValue, 16);
		}
		
		BSP_OS_TimeDlyMs(1000);
	}
	
}
void FAULT_TASK(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}
