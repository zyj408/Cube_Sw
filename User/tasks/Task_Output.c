#include  <includes.h>
#include "globalavr.h"

CPU_INT08U PwmPwrStatusOld;

void MtqSwitch_CallBack(OS_TMR *p_tmr, void *p_arg)
{
	out_en(OUT_MTQ, DISABLE);
	#if adcs_debug_enable
		printf("MTQ off at 1.8s...\r\n");
	#endif
}

void OUTPUT_TASK(void *p_arg)
{
	char i;
	char timeout = 0;
	(void)p_arg;
	
	while((!MTQ_POWER_PIN()) && timeout < 50)  //��һ�������ȴ���POWER�����ٽ��д���
	{
		timeout++;
		BSP_OS_TimeDlyMs(10);
	}
	
	if(MTQ_POWER_PIN())
	{	
		MTQ_WAKEN;
	}
	
	PwmPwrStatusOld = MTQ_POWER_PIN();  //��Pwm���ŵĵ�ƽֵ��ֵ��PwmOld
	
	while(1)
	{
		
		if(PwmPwrStatusOld != MTQ_POWER_PIN())  //Pwmʹ���źŷ����ı�
		{
			if(MTQ_POWER_PIN())  //��Դ����
			{
				MTQ_WAKEN;   //оƬ����
				
				for(i=0; i<3; i++)
				{
					PwmOutPut[i].PwmSetDuty = PwmOutPut[i].PwmSetDutyOld;
				}
			}
			else   //��Դ�ر�
			{
				for(i=0; i<3; i++)
				{
					PwmOutPut[i].PwmSetDutyOld = PwmOutPut[i].PwmSetDuty;
					PwmOutPut[i].PwmCurDuty = 0;
					bsp_SetPWMDutyCycle(PwmOutPut[i].PwmCurDuty, i+1);
				}
				
				MTQ_SLEEP;   //�ر�оƬ
			}
		}
		
		
		if(MTQ_POWER_PIN() && MTQ_SLEEP_PIN())  //MTQ��
		{
			for(i=0; i<3; i++)
			{
				if(PwmOutPut[i].PwmSetDir != PwmOutPut[i].PwmCurDir || PwmOutPut[i].PwmSetDuty != PwmOutPut[i].PwmCurDuty)
				{
					PwmOutPut[i].PwmCurDuty =  PwmOutPut[i].PwmSetDuty;
					PwmOutPut[i].PwmCurDir = PwmOutPut[i].PwmSetDir;
					
					if(PwmOutPut[0].PwmCurDir)
						MTQ1_DIR_NAG;
					else
						MTQ1_DIR_POS;

					if(PwmOutPut[1].PwmCurDir)
						MTQ2_DIR_NAG;
					else
						MTQ2_DIR_POS;

					if(PwmOutPut[2].PwmCurDir)
						MTQ3_DIR_NAG;
					else
						MTQ3_DIR_POS;
					
					if(PwmOutPut[i].PwmCurDir)
					{
						
						bsp_SetPWMDutyCycle(PwmOutPut[i].PwmCurDuty, i+1);
					}
					else
					{
						
						bsp_SetPWMDutyCycle(100 - PwmOutPut[i].PwmCurDuty, i+1);
					}
				}

			}		
		}
		
		
		
		
		
/*			
		if(MotorCurOutput != MotorSetOutput)
		{
			MotorCurOutput = MotorSetOutput;
			MotorChange(MotorSetOutput);
		}
*/	
		PwmPwrStatusOld = MTQ_POWER_PIN();
		BSP_OS_TimeDlyMs(1);
	}
}
