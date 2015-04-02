#include <includes.h>
#include "globalavr.h"

/*********** PWM模块配置参数 *******************/
uint32_t usPrescaler = 167;   /* PWM时钟预分频系数 */
uint32_t usPeriod = 999;     /* PWM计时周期 */
uint32_t usPWMOffset = 0;    /* PWM占空比补偿 */
/***********************************************/


void bsp_PWMInit(void)
{
	
	uint32_t PWM_CCR;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	Mem_Set(&TIM_OCInitStructure, 0x00, sizeof(TIM_OCInitTypeDef));
	Mem_Set(&TIM_TimeBaseStructure, 0x00, sizeof(TIM_TimeBaseInitTypeDef));
	Mem_Set(&GPIO_InitStructure, 0x00, sizeof(GPIO_InitTypeDef));
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	
	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_Cmd(TIM1, DISABLE);	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//PWM_CCR = (usPeriod + 1) * PwmSetDuty_1 / 100 - 1 - usPWMOffset;
	PWM_CCR = 0;    //初始化为0
	TIM_OCInitStructure.TIM_Pulse = PWM_CCR;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	//PWM_CCR = (usPeriod + 1) * PwmSetDuty_2 / 100 - 1 - usPWMOffset;
	PWM_CCR = 0;    //初始化为0
	TIM_OCInitStructure.TIM_Pulse = PWM_CCR;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	//PWM_CCR = (usPeriod + 1) * PwmSetDuty_3 / 100 - 1 - usPWMOffset;
	PWM_CCR = 0;    //初始化为0
	TIM_OCInitStructure.TIM_Pulse = PWM_CCR;	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}



void bsp_SetPWMDutyCycle(uint16_t PWMValue, unsigned char PWMChannel)
{	
  uint32_t PWM_CCR;
	
	if(PWMValue > 100)    //超范围保护
		PWMValue = 100;
	
	PWM_CCR = (usPeriod + 1) * PWMValue / 100 - 1 - usPWMOffset;

	switch(PWMChannel)
	{
		case 1:
		{
			TIM_SetCompare1(TIM1,PWM_CCR);
		}break;
	
		case 2: 
		{
			TIM_SetCompare2(TIM1,PWM_CCR);
		}break;
		
		case 3: 
		{
			TIM_SetCompare3(TIM1,PWM_CCR);
		}break;
		
		default: 
			break;
	}
	
}
