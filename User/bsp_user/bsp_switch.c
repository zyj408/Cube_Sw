#include <includes.h>




void bsp_InitSwitch(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_WHEEL_A_PORT | RCC_WHEEL_B_PORT | RCC_MTQ_EN_PORT | RCC_RES_PORT |
												 RCC_GPSA_EN_PORT | RCC_GPSB_EN_PORT | RCC_USB_EN_PORT | RCC_SOLAR_EN_PORT, ENABLE);

	/* 配置GPIO */

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
	#endif
	
	SW_MTQ_ENABLE;       //打开磁棒供电
	SW_WHEELA_ENABLE;    //打开动量轮A供电
  SW_WHEELB_ENABLE;    //打开动量轮B供电
	SW_GPSA_ENABLE;      //打开GPSA供电
	SW_GPSB_ENABLE;      //打开GPSB供电
	SW_USB_ENABLE;       //打开USB应答机供电
	SW_SOLAR_ENABLE;     //打开太阳能翻板供电
	SW_RES_ENABLE;       //打开保留供电

  
}
