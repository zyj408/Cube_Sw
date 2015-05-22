#include "bsp.h"
#include "globalavr.h"

/* ѡ��RTC��ʱ��Դ */


/* ���� */
#define RTC_TIMEOUT_US 100000
/* ��������RTC��Ƶ */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitRTC
*	����˵��: ��ʼ��RTC
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{
	//RTC_DeInit();
	RTC_Config();   /* ����RTCģ�� */
	
if (OBCBootInfo.BootCnt == 0)   /* ��һ�ο��� */
{  
	/* ����ϵ縴λ��־�Ƿ����� */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* �����ϵ縴λ */
		OBCBootInfo.BootPORRSTCnt++;
	}
}

else  /* ��N�ο��� */
{
	/* ����ϵ縴λ��־�Ƿ����� */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* �����ϵ縴λ */
		OBCBootInfo.BootPORRSTCnt++;
	}
	
	/* ������Ÿ�λ��־�Ƿ����� */
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		/* �������Ÿ�λ */
		OBCBootInfo.BootPINRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		OBCBootInfo.BootSFTRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* �����������Ź���λ */
    OBCBootInfo.BootIWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
		/* �������ڿ��Ź���λ */
		OBCBootInfo.BootWWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
	{
		/* �����͵�ѹ��λ */
    OBCBootInfo.BootLPWRRSTCnt++;
	}
}

}

/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: ��������ʱ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_Config(void)
{	
	int RtcTimeout;  /* RTC��ʱ���� */
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_InitTypeDef  RTC_InitStructure;
	RTC_DateTypeDef  RTC_DateStructure;
	
	Mem_Set(&RTC_TimeStructure, 0x00, sizeof(RTC_TimeTypeDef));
	Mem_Set(&RTC_InitStructure, 0x00, sizeof(RTC_InitTypeDef));	
	Mem_Set(&RTC_DateStructure, 0x00, sizeof(RTC_DateTypeDef));
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/* ʹ��PWRʱ�� */
	PWR_BackupAccessCmd(ENABLE);/* �������RTC */
	
	OBCBootInfo.BootRTC_Source = 0;
try_again:
	if(OBCBootInfo.BootRTC_Source == 0)  /* ѡ��LSE��Ϊʱ��Դ */
	{
		RCC_LSEConfig(RCC_LSE_ON);   	     /* ʹ��LSE����  */
		RtcTimeout = 0;	                   /* �ȴ����� */  

		while((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))//20
		{
			bsp_DelayUS(1);             /* STM32�ӳٺ��� */
		}

		if(RtcTimeout > RTC_TIMEOUT_US)  //18              /* �ж��Ƿ��г�ʱ */
		{	
			
			DEBUG_LOG("Init RTC Wrong: Oscillator time out! \r\n");

			RCC_LSEConfig(RCC_LSE_OFF);
			OBCBootInfo.BootLSE_Error = 1;
			OBCBootInfo.BootRTC_Source = 1;   /* �Զ�����RTCΪ�ڲ�ʱ�� */
		}
	
		if(OBCBootInfo.BootRTC_Source == 0)
		{
			DEBUG_LOG("Enable RTC with LSE \r\n");

			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		/* ѡ��RTCʱ��Դ */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}

	if(OBCBootInfo.BootRTC_Source == 1)	/* ѡ��LSI��Ϊʱ��Դ */
	{
		RCC_LSICmd(ENABLE);  /* ʹ���ڲ�ʱ�� */

		RtcTimeout = 0;      /*  �ȴ��ڲ�ʱ���ȶ�*/
		while((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))
		{
			bsp_DelayUS(1);             /* STM32�ӳٺ��� */
		}
		
		if(RtcTimeout > RTC_TIMEOUT_US)
		{
			DEBUG_LOG("All Oscillator time out! \r\n");
			//RCC_LSICmd(DISABLE);
			OBCBootInfo.BootLSI_Error = 1;
			OBCBootInfo.BootRTC_Source = 2;   /* �Զ�����RTCΪ�ڲ�ʱ�� */
		}
		if(OBCBootInfo.BootRTC_Source == 1)
		{
			DEBUG_LOG("Enable RTC with LSI \r\n");
			
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  /* ѡ��RTCʱ��Դ */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}
	if((OBCBootInfo.BootRTC_Source == 0)||(OBCBootInfo.BootRTC_Source == 1))
	{

	  RCC_RTCCLKCmd(ENABLE);	/* ʹ��RTCʱ�� */
	  RTC_WaitForSynchro();  /* �ȴ�RTC APB�Ĵ���ͬ�� */
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  /* ʹ��ʱ��� */
		RTC_TimeStampPinSelection(RTC_TamperPin_PC13);	
	  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;  /* ����RTC���ݼĴ����ͷ�Ƶ��  */
	  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	  if (RTC_Init(&RTC_InitStructure) == ERROR)  /* ���RTC��ʼ�� */
	  {
			DEBUG_LOG("RTC Init wrong \r\n");
			if(OBCBootInfo.BootRTC_Source == 0)
			{
				OBCBootInfo.BootRTC_Source = 1;
				goto try_again;
			}
	  }

	  /* ���������պ����� */
	  RTC_DateStructure.RTC_Year = 0x14;
	  RTC_DateStructure.RTC_Month = RTC_Month_October;
	  RTC_DateStructure.RTC_Date = 0x23;
	  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;
	  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	  /* ����ʱ���룬�Լ���ʾ��ʽ */
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
	tm->tm_year = CurDate.RTC_Year + 2000;  //15��+2000
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
