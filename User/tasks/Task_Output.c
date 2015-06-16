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
	
	while((!MTQ_POWER_PIN()) && timeout < 50)  //第一次启动等待至POWER拉高再进行处理
	{
		timeout++;
		BSP_OS_TimeDlyMs(10);
	}
	
	if(MTQ_POWER_PIN())
	{	
		MTQ_WAKEN;
	}
	
	PwmPwrStatusOld = MTQ_POWER_PIN();  //将Pwm引脚的电平值赋值给PwmOld
	
	while(1)
	{
		
		if(PwmPwrStatusOld != MTQ_POWER_PIN())  //Pwm使能信号发生改变
		{
			if(MTQ_POWER_PIN())  //电源开启
			{
				MTQ_WAKEN;   //芯片醒来
				
				for(i=0; i<3; i++)
				{
					PwmOutPut[i].PwmSetDuty = PwmOutPut[i].PwmSetDutyOld;
				}
			}
			else   //电源关闭
			{
				for(i=0; i<3; i++)
				{
					PwmOutPut[i].PwmSetDutyOld = PwmOutPut[i].PwmSetDuty;
					PwmOutPut[i].PwmCurDuty = 0;
					bsp_SetPWMDutyCycle(PwmOutPut[i].PwmCurDuty, i+1);
				}
				
				MTQ_SLEEP;   //关闭芯片
			}
		}
		
		
		if(MTQ_POWER_PIN() && MTQ_SLEEP_PIN())  //MTQ打开
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
