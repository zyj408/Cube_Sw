#include "bsp.h"
#include "globalavr.h"

/* 选择RTC的时钟源 */


/* 变量 */
#define RTC_TIMEOUT_US 100000
/* 用于设置RTC分频 */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*	函 数 名: bsp_InitRTC
*	功能说明: 初始化RTC
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{
	#if debug_enable
	if(RCC_GetSYSCLKSource() == 0x00)
		printf("Current system clock:HSI\r\n");
	if(RCC_GetSYSCLKSource() == 0x04)
		printf("Current system clock:HSE\r\n");
	else if(RCC_GetSYSCLKSource() == 0x08)
		printf("Current system clock:PLL\r\n");
	#endif
	//RTC_DeInit();
	RTC_Config();   /* 配置RTC模块 */
	
if (OBCBootInfo.BootCnt == 0)   /* 第一次开机 */
{  
	/* 检测上电复位标志是否设置 */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* 发生上电复位 */
		OBCBootInfo.BootPORRSTCnt++;
	}
}

else  /* 第N次开机 */
{
	/* 检测上电复位标志是否设置 */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* 发生上电复位 */
		OBCBootInfo.BootPORRSTCnt++;
	}
	
	/* 检测引脚复位标志是否设置 */
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		/* 发生引脚复位 */
		OBCBootInfo.BootPINRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		OBCBootInfo.BootSFTRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* 发生独立看门狗复位 */
    OBCBootInfo.BootIWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
		/* 发生窗口看门狗复位 */
		OBCBootInfo.BootWWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
	{
		/* 发生低电压复位 */
    OBCBootInfo.BootLPWRRSTCnt++;
	}
}

}

/*
*********************************************************************************************************
*	函 数 名: RTC_Config
*	功能说明: 用于配置时间戳功能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_Config(void)
{	
	int RtcTimeout;  /* RTC超时计数 */
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_InitTypeDef  RTC_InitStructure;
	RTC_DateTypeDef  RTC_DateStructure;
	
	Mem_Set(&RTC_TimeStructure, 0x00, sizeof(RTC_TimeTypeDef));
	Mem_Set(&RTC_InitStructure, 0x00, sizeof(RTC_InitTypeDef));	
	Mem_Set(&RTC_DateStructure, 0x00, sizeof(RTC_DateTypeDef));
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/* 使能PWR时钟 */
	PWR_BackupAccessCmd(ENABLE);/* 允许访问RTC */
	
	

	if(OBCBootInfo.BootRTC_Source == 0)  /* 选择LSE作为时钟源 */
	{
		RCC_LSEConfig(RCC_LSE_ON);   	     /* 使能LSE振荡器  */
		RtcTimeout = 0;	                   /* 等待就绪 */  

		while((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))//20
		{
			bsp_DelayUS(1);             /* STM32延迟函数 */
		}

		if(RtcTimeout > RTC_TIMEOUT_US)  //18              /* 判断是否有超时 */
		{	
			
			DEBUG_LOG("Init RTC Wrong: Oscillator time out! \r\n");

			RCC_LSEConfig(RCC_LSE_OFF);
			OBCBootInfo.BootLSE_Error = 1;
			OBCBootInfo.BootRTC_Source = 1;   /* 自动配置RTC为内部时钟 */
		}
	
		if(OBCBootInfo.BootRTC_Source == 0)
		{
			DEBUG_LOG("Enable RTC with LSE \r\n");

			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		/* 选择RTC时钟源 */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}

	if(OBCBootInfo.BootRTC_Source == 1)	/* 选择LSI作为时钟源 */
	{
		RCC_LSICmd(ENABLE);  /* 使能内部时钟 */

		RtcTimeout = 0;      /*  等待内部时钟稳定*/
		while((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))
		{
			__nop();__nop();__nop();__nop();__nop();/* STM32延迟函数 */
		}
		
		if(RtcTimeout > RTC_TIMEOUT_US)
		{
			DEBUG_LOG("All Oscillator time out! \r\n");
			//RCC_LSICmd(DISABLE);
			OBCBootInfo.BootLSI_Error = 1;
			OBCBootInfo.BootRTC_Source = 2;   /* 自动配置RTC为内部时钟 */
		}
		if(OBCBootInfo.BootRTC_Source == 1)
		{
			DEBUG_LOG("Enable RTC with LSI \r\n");
			
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  /* 选择RTC时钟源 */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}
	if((OBCBootInfo.BootRTC_Source == 0)||(OBCBootInfo.BootRTC_Source == 1))
	{

	  RCC_RTCCLKCmd(ENABLE);	/* 使能RTC时钟 */
	  RTC_WaitForSynchro();  /* 等待RTC APB寄存器同步 */
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  /* 使能时间戳 */
		RTC_TimeStampPinSelection(RTC_TamperPin_PC13);	
	  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;  /* 配置RTC数据寄存器和分频器  */
	  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	  if (RTC_Init(&RTC_InitStructure) == ERROR)  /* 检测RTC初始化 */
	  {
			DEBUG_LOG("RTC Init wrong \r\n");
	  }

	  /* 设置年月日和星期 */
	  RTC_DateStructure.RTC_Year = 0x14;
	  RTC_DateStructure.RTC_Month = RTC_Month_October;
	  RTC_DateStructure.RTC_Date = 0x23;
	  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;
	  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	  /* 设置时分秒，以及显示格式 */
	  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	  RTC_TimeStructure.RTC_Hours   = 0x00;
	  RTC_TimeStructure.RTC_Minutes = 0x00;
	  RTC_TimeStructure.RTC_Seconds = 0x00; 
	  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	}
}

void bsp_RTCTimeGet(struct tm *tm)
{
	unsigned int have_retried = 0;

retry_get_time:	
	RTC_GetTime(RTC_Format_BIN, &CurTime);
	RTC_GetDate(RTC_Format_BIN, &CurDate);
	
	tm->tm_sec = CurTime.RTC_Seconds;
	tm->tm_min = CurTime.RTC_Minutes;
	tm->tm_hour = CurTime.RTC_Hours;


	if (tm->tm_sec == 0 && !have_retried) 
	{
		have_retried = 1;
		goto retry_get_time;
	}	
	
	tm->tm_wday = CurDate.RTC_WeekDay;
	tm->tm_mday = CurDate.RTC_Date;
	tm->tm_mon = CurDate.RTC_Month;
	tm->tm_year = CurDate.RTC_Year + 2000;  //15年+2000
}

uint32_t bsp_UTCTimeGet(void)
{
	struct tm rtc_time;
	uint32_t utc_time;
	
	bsp_RTCTimeGet(&rtc_time);
	/* 946684800: UTC 2000-01-01 08:00:00(Beijing) */
	utc_time = mktime(&rtc_time) - 946684800;
	
	return utc_time;
}


void bsp_RTCSet(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = hour; 
  RTC_TimeStructure.RTC_Minutes = minute;
  RTC_TimeStructure.RTC_Seconds = second;
	
	RTC_DateStructure.RTC_WeekDay = 0;
	RTC_DateStructure.RTC_Date = day;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Year = year;
	
	
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		//printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	{
		//printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	
}
