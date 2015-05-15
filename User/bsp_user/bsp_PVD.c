#include <includes.h>
#include "globalavr.h"

void bsp_PVD_Init(void)
{  
	NVIC_InitTypeDef NVIC_InitStruct;
  EXTI_InitTypeDef EXTI_InitStructure;

  NVIC_InitStruct.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
   

  NVIC_Init(&NVIC_InitStruct);//允许中断
	
	EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16; 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
  EXTI_Init(&EXTI_InitStructure);

  PWR_PVDLevelConfig(PWR_CR_PLS_LEV5); 
  PWR_PVDCmd(ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);
}
//PVD中断函数如下
void PVD_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line16); 
	if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
	{
		PWR_ClearFlag(PWR_FLAG_PVDO);
		LowVolCnt++;
		//VarBak();
	}	
	OSIntExit();
}
