#include <includes.h>

void bsp_MTQ_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_MTQ_SLEEP_PORT | RCC_MTQ1_DIR_PORT | RCC_MTQ2_DIR_PORT | RCC_MTQ3_DIR_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_MTQ_SLEEP_PIN;	
	GPIO_Init(GPIO_MTQ_SLEEP_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_MTQ1_DIR_PIN;	
	GPIO_Init(GPIO_MTQ1_DIR_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_MTQ2_DIR_PIN;	
	GPIO_Init(GPIO_MTQ2_DIR_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_MTQ3_DIR_PIN;	
	GPIO_Init(GPIO_MTQ3_DIR_PORT, &GPIO_InitStructure);
	
	
	bsp_PWMInit();
	
}

void MTQ_ChangeDir(uint8_t chl, uint8_t value)
{

}

void MTQ_Open(void)
{
	if()
	{
		
	}

}