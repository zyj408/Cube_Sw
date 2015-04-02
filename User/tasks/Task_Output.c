#include  <includes.h>
#include "globalavr.h"

CPU_INT16U PwmCurDutyOld_1;
CPU_INT16U PwmCurDutyOld_2;
CPU_INT16U PwmCurDutyOld_3;
CPU_INT08U PwmPwrStatusOld;
void OUTPUT_TASK(void *p_arg)
{
	char timeout = 0;
	(void)p_arg;
	
	//PwmCurDuty_1 = PwmSetDuty_1;
  //PwmCurDuty_2 = PwmSetDuty_2;
  //PwmCurDuty_3 = PwmSetDuty_3;
	
	while((!MTQ_POWER_PIN()) && timeout < 50)  //第一次启动等待至POWER拉高再进行处理
	{
		timeout++;
		BSP_OS_TimeDlyMs(100);
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
				
				PwmSetDuty_1 = PwmCurDutyOld_1;
				PwmSetDuty_2 = PwmCurDutyOld_2;
				PwmSetDuty_3 = PwmCurDutyOld_3;
			}
			else   //电源关闭
			{
				PwmCurDutyOld_1 = PwmCurDuty_1;
				PwmCurDutyOld_2 = PwmCurDuty_2;
				PwmCurDutyOld_3 = PwmCurDuty_3;
				PwmCurDuty_1 = 0;
				PwmCurDuty_2 = 0;
				PwmCurDuty_3 = 0;
				bsp_SetPWMDutyCycle(PwmCurDuty_1, 1);
				bsp_SetPWMDutyCycle(PwmCurDuty_2, 2);
				bsp_SetPWMDutyCycle(PwmCurDuty_3, 3);
				
				MTQ_SLEEP;   //关闭芯片
			}
		}
		
		
		if(MTQ_POWER_PIN() && MTQ_SLEEP_PIN())  //MTQ打开
		{
			if(PwmCurDuty_1 != PwmSetDuty_1)
			{
				PwmCurDuty_1 = PwmSetDuty_1;
				bsp_SetPWMDutyCycle(PwmCurDuty_1, 1);			
			}
			if(PwmCurDuty_2 != PwmSetDuty_2)
			{
				PwmCurDuty_2 = PwmSetDuty_2;
				bsp_SetPWMDutyCycle(PwmCurDuty_2, 2);			
			}
			if(PwmCurDuty_3 != PwmSetDuty_3)
			{
				PwmCurDuty_3 = PwmSetDuty_3;
				bsp_SetPWMDutyCycle(PwmCurDuty_3, 3);
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
		BSP_OS_TimeDlyMs(100);
	}
}
