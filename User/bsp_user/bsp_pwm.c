#include <includes.h>
#include "globalavr.h"

/*********** PWM模块配置参数 *******************/
uint32_t usPrescaler = 167;   /* PWM时钟预分频系数 */
uint32_t usPeriod = 999;     /* PWM计时周期 */
uint32_t usPWMOffset = 0;    /* PWM占空比补偿 */

uint32_t usCapturePeriod = 9999;     /* PWM计时周期 */
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

void bsp_PWMCaptureInit(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_ICInitTypeDef	       TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	Mem_Set(&GPIO_InitStructure, 0x00, sizeof(GPIO_InitTypeDef));
	Mem_Set(&TIM_ICInitStructure, 0x00, sizeof(TIM_ICInitTypeDef));
	Mem_Set(&TIM_TimeBaseStructure, 0x00, sizeof(TIM_TimeBaseInitTypeDef));
	Mem_Set(&NVIC_InitStructure, 0x00, sizeof(NVIC_InitTypeDef));
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	TIM_Cmd(TIM10, DISABLE);		/* 关闭定时输出 */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usCapturePeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM10, &TIM_ICInitStructure);
	TIM_ICInit(TIM11, &TIM_ICInitStructure);
	
	TIM_ARRPreloadConfig(TIM10, ENABLE);
	TIM_ITConfig(TIM10, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
	
	TIM_ARRPreloadConfig(TIM11, ENABLE);
	TIM_ITConfig(TIM11, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM11, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}



float PWM_Feq;
float PWM_Feq1;
void TIM1_UP_TIM10_IRQHandler(void)
{
	static uint16_t this_time10_CH1 = 0; 
	static uint16_t last_time10_CH1 = 0; 
	uint16_t TimerCount_CH1;
	if(TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET) 
	{         
		TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);

			this_time10_CH1 = TIM_GetCapture1(TIM10);
			
			if(this_time10_CH1 > last_time10_CH1)
			{ 
				TimerCount_CH1 = (this_time10_CH1 - last_time10_CH1); 
			} 
			else 
			{ 
				TimerCount_CH1 = ((usCapturePeriod - last_time10_CH1) + this_time10_CH1);  //周期
			}
			last_time10_CH1 = this_time10_CH1;			
			PWM_Feq = 1000000.0 / TimerCount_CH1;			
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	static uint16_t this_time11_CH1 = 0; 
	static uint16_t last_time11_CH1 = 0; 
	uint16_t TimerCount_CH1;
	
	if(TIM_GetITStatus(TIM11, TIM_IT_CC1) != RESET) 
	{         
		TIM_ClearITPendingBit(TIM11, TIM_IT_CC1);

			this_time11_CH1 = TIM_GetCapture1(TIM11);
			
			if(this_time11_CH1 > last_time11_CH1)
			{ 
				TimerCount_CH1 = (this_time11_CH1 - last_time11_CH1); 
			} 
			else 
			{ 
				TimerCount_CH1 = ((usCapturePeriod - last_time11_CH1) + this_time11_CH1);  //周期
			}
			last_time11_CH1 = this_time11_CH1;			
			PWM_Feq1 = 1000000.0 / TimerCount_CH1;			
	}
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
