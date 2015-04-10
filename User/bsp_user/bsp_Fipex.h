#ifndef __BSP_FIPEX_H_
#define __BSP_FIPEX_H_

#define GPIO_FIPEX_3V3_EN_PORT	GPIOG			
#define RCC_FIPEX_3V3_PORT      RCC_AHB1Periph_GPIOG
#define GPIO_FIPEX_3V3_EN_PIN		GPIO_Pin_6

#define GPIO_FIPEX_5V_EN_PORT   GPIOG			
#define RCC_FIPEX_5V_PORT       RCC_AHB1Periph_GPIOG
#define GPIO_FIPEX_5V_EN_PIN		GPIO_Pin_7


#define FIPEX_3V3_PIN()         GPIO_ReadOutputDataBit(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_5V_PIN()          GPIO_ReadOutputDataBit(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)

#define FIPEX_3V3_ENABLE        GPIO_SetBits(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_3V3_DISABLE       GPIO_ResetBits(GPIO_FIPEX_3V3_EN_PORT, GPIO_FIPEX_3V3_EN_PIN)
#define FIPEX_5V_ENABLE         GPIO_SetBits(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)
#define FIPEX_5V_DISABLE        GPIO_ResetBits(GPIO_FIPEX_5V_EN_PORT, GPIO_FIPEX_5V_EN_PIN)

extern uint8_t FepixPowerOnFlg;


struct FipexCmdStr
{
	uint8_t    FipexCmd[32];
	uint8_t    CmdLength;
	uint16_t   CmdDelay;
};

struct FipexOperationParaStr
{
	struct FipexCmdStr FipexCmdInfo[10];
	uint32_t StartTime;
	uint16_t RepeatTime;
	uint8_t  CmdCnt;
};
	
void bsp_FipexSwitchInit(void);
uint8_t bsp_FipexSendCmd(uint8_t *cmd, uint16_t length);
uint8_t bsp_FipexPowerOn(void);
uint8_t bsp_FipexPowerOff(void);
void FipexInfomationInit(void);
void FipexSetDefaultInfo(void);
#endif
