#include  <includes.h>


#define EleOfArray(x) (sizeof(x) / sizeof(x[0]))
CPU_INT16U TelDataBuf[64];  /* ң�����ݴ洢���� */

void TEL_SAM_TASK(void *p_arg)
{
	static char tel_time = 0;
	(void)p_arg;
	
	Mem_Set(TelDataBuf, 0x00, sizeof(TelDataBuf));
	while(1)
	{
		
		if(15 <= (tel_time++))  /* ÿ15�����һ��ң��洢 */
		{
			tel_time=0;
			BSP_OS_SemPost(&SEM_TEL_STO);
		}

		BSP_OS_SemPost(&SEM_CYC_INFO);
		BSP_OS_TimeDlyMs(2000);		
	}
}

void TEL_STO_TASK(void *p_arg)
{
  (void)p_arg;
	
	while (1) 
	{
		BSP_OS_SemWait(&SEM_TEL_STO, 0);
		
//		TelDataBuf[0] = 0xEE;
//		TelDataBuf[1] = 0x01;     /* �������ͣ�1��ң���飬2��GPS�� */
//		TelDataBuf[2] = TelCurStrCnt;
//		TelDataBuf[3] = 0x01;
//		TelDataBuf[4] = CurDate.RTC_Year;
//		TelDataBuf[5] = CurDate.RTC_Month;
//		TelDataBuf[6] = CurDate.RTC_Date;
//		TelDataBuf[7] = CurTime.RTC_Hours;
//		TelDataBuf[8] = CurTime.RTC_Minutes;
//		TelDataBuf[9] = CurTime.RTC_Seconds;
//		TelDataBuf[10] = 1;
//		TelDataBuf[11] = 2;
//		TelDataBuf[12] = 3;
//		TelDataBuf[13] = 4;
//		TelDataBuf[14] = 5;
//		TelDataBuf[15] = 6;
//		TelDataBuf[16] = 7;
//		TelDataBuf[17] = 8;
//		TelDataBuf[62] = GetCheckSum(TelDataBuf, 61);		/* ��������λ */
//		TelDataBuf[63] = 0xEE;
		
//		NOR_WriteBuffer(TelDataBuf, TelCurPtr, EleOfArray(TelDataBuf));

		TelCurStrCnt++;         /* ��ǰң��洢�� */
		
		TelCurPtr+=(64*2);
		/* ң��洢���� */
		
		BSP_OS_TimeDlyMs(500); 					 
	}
}
