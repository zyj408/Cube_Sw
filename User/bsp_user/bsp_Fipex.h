#ifndef __BSP_FIPEX_H_
#define __BSP_FIPEX_H_

#define GPIO_FIPEX_3V3_EN_PORT	GPIOE			
#define RCC_FIPEX_3V3_PORT      RCC_AHB1Periph_GPIOE
#define GPIO_FIPEX_3V3_EN_PIN		GPIO_Pin_6

#define GPIO_FIPEX_5V_EN_PORT   GPIOE			
#define RCC_FIPEX_5V_PORT       RCC_AHB1Periph_GPIOE
#define GPIO_FIPEX_5V_EN_PIN		GPIO_Pin_6


#define FIPEX_3V3_PIN()         GPIO_ReadOutputDataBit(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_5V_PIN()          GPIO_ReadOutputDataBit(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)

#define FIPEX_3V3_ENABLE        GPIO_SetBits(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_3V3_DISABLE       GPIO_ResetBits(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_5V_ENABLE         GPIO_SetBits(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)
#define FIPEX_5V_DISABLE        GPIO_ResetBits(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)

void bsp_FipexSwitchInit(void);
uint8_t bsp_FipexSendCmd(uint8_t *cmd, uint16_t length);
uint8_t bsp_FipexPowerOn(void);
uint8_t bsp_FipexPowerOff(void);

#endif
