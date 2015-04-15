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


#define	OBC_SU_ON				0x0F
#define OBC_SU_OFF				0xF0
#define OBC_SU_END				0xFF

#define SU_PING					0x00
#define SU_INIT					0x01
#define SU_ID					0x04
#define SU_RSP					0x10
#define SU_SP					0x11
#define SU_HK					0x20
#define SU_DP					0x21
#define SU_STDBY				0x0A
#define SU_SC					0x0B
#define SU_SM					0x0C
#define SU_CAL					0x33


enum FipexStatus_Enum
{
	Stop = 0x00,
	PowerOn,
	ScriptRunning,
	ScriptSleep,
};

extern uint8_t FipexAckWaitFlg;
extern uint8_t ScriptAct;
extern uint8_t FipexPowerOnFlg;
extern uint8_t CurrentScript;  
extern struct FipexOperationParaStr FipexOperationPara;
extern int FipexCurTime;
extern int DelayTime;

extern enum FipexStatus_Enum FipexStatus;

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
void FipexScriptStop(void);
uint8_t bsp_FipexSendCmd(uint8_t *cmd, uint16_t length);
uint8_t bsp_FipexGetCheckSum(unsigned char *cmd, unsigned char* checksum);
void FipexScriptStart(void);




#endif
