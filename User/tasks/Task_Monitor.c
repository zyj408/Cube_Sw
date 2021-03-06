#include  <includes.h>

#define EPS_CHECK_TIME	100
void STO_UPDATE_TASK(void *p_arg)
{
	(void)p_arg;		/* 避免编译器告警 */
	
	while(1)
	{
		CurUTCTime = bsp_UTCTimeGet(); //得到UTC全局时间
		//RTC_GetTime(RTC_Format_BIN, &CurTime);  /* 更新星上时间 */
		//RTC_GetDate(RTC_Format_BIN, &CurDate);  /* 更新星上日期 */
		
		RandNum_32 = RNG_GetRandomNumber();     /* 真随机数 */
		
		IWDG_Feed();
		
		BSP_OS_TimeDlyMs(1000);
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

uint8_t eps_time = 0;
void ADC_SAMPLE_TASK(void *p_arg)
{
	CPU_INT08U round;
	(void)p_arg;		/* 避免编译器告警 */

	EpsAdStart();
	ObcAdStart();
	TempAdStart();
	
//	AD7192StartContinuousConvertion(AIN1_COM|AIN2_COM|AIN3_COM|AIN4_COM);
	BSP_OS_TimeDlyMs(100);
	
	while(1)
	{
		
		BSP_OS_MutexWait(&MUTEX_MAG_CAP, 0);
		for(round = 0; round < 16; round++)
		{
			EpsAdUpdate(EPS_AD_CS1);
			//BSP_OS_TimeDlyMs(1);
			
			EpsAdUpdate(EPS_AD_CS2);
			//BSP_OS_TimeDlyMs(1);
			
			ObcAdUpdate();
			//BSP_OS_TimeDlyMs(1);
			
			TempAdUpdate();
			//BSP_OS_TimeDlyMs(1);
		}
		BSP_OS_MutexPost(&MUTEX_MAG_CAP);
		
		AdDataFliter(EpsAdValue, EpsAdValueAver, 32);
		AdDataFliter(ObcAdValue,ObcAdValueAver, 16);
		AdDataFliter(TempAdValue,TempAdValueAver, 16);
		
		if(eps_time++ > EPS_CHECK_TIME)
		{
			eps_time = EPS_CHECK_TIME;
			adc_to_real(EpsAdValueAver, &eps_adc_data); //将adc值填入eps_adc_data结构体
			eps_data_handling();//数据处理，eps状态量填入相关结构体
		}
		BSP_OS_TimeDlyMs(100);
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
