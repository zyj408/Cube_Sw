#include <includes.h>




void bsp_InitSwitch(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/*  πƒ‹ ±÷” */
	RCC_AHB1PeriphClockCmd(RCC_WHEEL_A_PORT | RCC_WHEEL_B_PORT | RCC_MTQ_EN_PORT | RCC_RES_PORT |
												 RCC_GPSA_EN_PORT | RCC_GPSB_EN_PORT | RCC_USB_EN_PORT | RCC_SOLAR_EN_PORT, ENABLE);

	/* ≈‰÷√GPIO */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_GPSB_EN_PIN;	
	GPIO_Init(GPIO_GPSB_EN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_WHEEL_A_PIN;	
	GPIO_Init(GPIO_WHEEL_A_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_WHEEL_B_PIN;	
	GPIO_Init(GPIO_WHEEL_B_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_GPSA_EN_PIN;	
	GPIO_Init(GPIO_GPSA_EN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_MTQ_EN_PIN;	
	GPIO_Init(GPIO_MTQ_EN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_USB_EN_PIN;	
	GPIO_Init(GPIO_USB_EN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_SOLAR_EN_PIN;	
	GPIO_Init(GPIO_SOLAR_EN_PORT, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_RES_PIN;	
	GPIO_Init(GPIO_RES_PORT, &GPIO_InitStructure);	
	
	#if OBC_MTQ_ENABLE
	
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
	
	MTQ1_DIR_POS;
	MTQ2_DIR_POS;
	MTQ3_DIR_POS;
	
	
	#endif
	
	outall_en(ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_S0_EN_PORT | RCC_S1_EN_PORT | RCC_S2_EN_PORT | RCC_S3_EN_PORT, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_S0_EN_PIN;	
	GPIO_Init(GPIO_S0_EN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_S1_EN_PIN;	
	GPIO_Init(GPIO_S1_EN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_S2_EN_PIN;	
	GPIO_Init(GPIO_S2_EN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_S3_EN_PIN;	
	GPIO_Init(GPIO_S3_EN_PORT, &GPIO_InitStructure);
	
	SW_S0_DISABLE;
	SW_S1_DISABLE;
	SW_S2_DISABLE;
	SW_S3_DISABLE;
}
