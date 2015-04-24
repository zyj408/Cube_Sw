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



/* SU responses */
#define SU_R_ACK				0x02		/* Acknowledge response, DATA LEN: 0 */
#define SU_R_NACK				0x03		/* Negative acknowledge response, DATA LEN: 1 */
#define SU_R_ID					0x04		/* Identification message, DATA LEN: 1 */
#define SU_R_HK					0x2000	/* Housekeeping data packet, DATA LEN: 46 */
#define SU_R_SDP				0x30		/* Science Data Package, DATA LEN: variable */
#define	SU_R_CAL				0x33		/* All calibration values, DATA LEN: 40 */

/* SU_R_NACK: 'EFLAG' */
#define	SyncError				0x01		/* Packet reception timeout */
#define FCSError				0x02		/* Frame Check Sum (XOR) wrong */
#define wPID						0x03		/* Parameter ID unknown */
#define	POOR						0x04		/* Parameter out of range */
#define wMode						0x05		/* Shall be used if a commanded state change is not allowed */
#define	wCMD						0x06		/* Wrong command */
#define wLEN						0x07		/* LEN wrong */


#define IS_OBC_SU_CMD(CMD)          (((CMD) == OBC_SU_ON)|| \
                                    ((CMD) == OBC_SU_OFF) || \
                                    ((CMD) == OBC_SU_END))

#define IS_FIPEX_CMD(CMD)           (((CMD) == SU_PING)	|| \
                                    ((CMD) == SU_INIT)	|| \
																		((CMD) == SU_ID)	|| \
																		((CMD) == SU_RSP)	|| \
																		((CMD) == SU_SP)	|| \
																		((CMD) == SU_HK)	|| \
																		((CMD) == SU_DP)	|| \
																		((CMD) == SU_STDBY)	|| \
																		((CMD) == SU_SC)	|| \
																		((CMD) == SU_SM)	|| \
																		((CMD) == SU_CAL))







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
uint8_t bsp_FipexGetCheckSum(unsigned char *cmd, unsigned char* checksum, unsigned char trans_flag);
void FipexScriptStart(void);
uint8_t FipexAckHandle(uint8_t *rx_data);
void FipexScienceDataStore(uint8_t * rx_data);


#endif
