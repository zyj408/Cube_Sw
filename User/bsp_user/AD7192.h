#ifndef _AD7192_H
#define _AD7192_H
#include "stm32f4xx.h" 


/*AD7192*/
//Communications Register
#define	WEN				        0x00
#define WEN_DIS			      0x80
#define RW_W			        0x00
#define	RW_R		       	  0x40
#define REG_COM_STA		    0x00
#define	REG_MODE		      0x01
#define	REG_CONF		      0x02
#define	REG_DATA 	       	0x03
#define	REG_ID			      0x04
#define	REG_GPOCON		    0x05
#define	REG_OFFSET		    0x06
#define	REG_FS			      0x07
#define	CREAD_EN		      0x04
#define	CREAD_DIS		      0x00

//Status Register
#define	RDY_H			        0x80
#define	RDY_L			        0x00
#define	ERR_H			        0x40
#define	ERR_L			        0x00
#define	NOREF_H			      0x20
#define	NOREF_L			      0x00
#define	PARITY_H		      0x10
#define	PARITY_L		      0x00
#define	CHDST_AIN1_AIN2	  0x00
#define	CHDST_AIN3_AIN4	  0x01
#define	CHDST_TEMP		    0x02
#define	CHDST_AIN2_AIN2  	0x03
#define	CHDST_AIN1_COM	  0x04
#define	CHDST_AIN2_COM	  0x05
#define	CHDST_AIN3_COM	  0x06
#define	CHDST_AIN4_COM	  0x07

//Mode Register
#define	MODE_CONT				  0x000000
#define	MODE_SING				  0x200000
#define	MODE_IDLE				  0x400000
#define	MODE_PD					  0x600000
#define	MODE_INZCL				0x800000
#define	MODE_INFCL				0xA00000
#define	MODE_SYSZCL				0xC00000
#define	MODE_SYSFCL				0xE00000
#define	DAT_STA_EN				0x100000
#define	DAT_STA_DIS				0x000000	  
#define	EXT_XTAL				  0x000000
#define	EXT_CLK					  0x040000
#define	INCLK_MCLK2TRI  	0x080000
#define	INCLK_MCLK2EN			0x0C0000
#define	SINC_4					  0x000000
#define	SINC_3					  0x008000
#define	ENPAR_EN				  0x002000
#define	ENPAR_DIS				  0x000000
#define	CLK_DIV_2				  0x001000
#define	CLK_DIV_DIS				0x000000
#define	SINGLECYCLE_EN	  0x000800
#define	SINGLECYCLE_DIS	  0x000000
#define	REJ60_EN				  0x000400
#define	REJ60_DIS				  0x000000
										
//Configuration Register			
#define CHOP_EN					0x800000
#define	CHOP_DIS				0x000000
#define	REF_IN1					0x000000
#define	REF_IN2					0x100000
#define	AIN1_AIN2				0x000100
#define	AIN3_AIN4				0x000200
#define	TEMP				  	0x000400
#define	AIN2_AIN2				0x000800
#define	AIN1_COM				0x001000
#define	AIN2_COM				0x002000
#define	AIN3_COM				0x004000
#define	AIN4_COM				0x008000
#define	BURN_EN					0x000080
#define	BURN_DIS				0x000000
#define	REFDET_EN				0x000040
#define	REFDET_DIS			0x000000
#define	BUF_EN					0x000010
#define	BUF_DIS					0x000000
#define	UB_UNI					0x000008
#define UB_BI					  0x000000
#define	GAIN_1					0x000000
#define	GAIN_8					0x000003
#define	GAIN_16					0x000004
#define	GAIN_32					0x000005
#define	GAIN_64					0x000006
#define	GAIN_128				0x000007

//GPOCON Register
#define BPDSW_CLOSE			0x40
#define	BPDSW_OPEN			0x00
#define	GP32EN					0x20
#define	GP32DIS					0x00
#define	GP10EN					0x10
#define	GP10DIS					0x00
#define	P3DAT_H					0x08
#define	P3DAT_L					0x00
#define	P2DAT_H					0x04
#define	P2DAT_L					0x00
#define	P1DAT_H					0x02
#define	P1DAT_L					0x00
#define	P0DAT_H					0x01
#define	P0DAT_L					0x00

//No Operation
#define	NOP						0x00

#define AD7192_debug 0

#define AD7192_REF_TEST_UP    	3.0
#define AD7192_REF_TEST_DOWN  	2.0

/* AD7192 CS */
#define AD7192_CS_GPIO			GPIOB
#define AD7192_CS_PIN			GPIO_Pin_12
#define AD7192_CS_LOW()      	AD7192_CS_GPIO->BSRRH = AD7192_CS_PIN
#define AD7192_CS_HIGH()     	AD7192_CS_GPIO->BSRRL = AD7192_CS_PIN

/* AD7192 Set */
#define AD7192_SET_GPIO			GPIOG
#define AD7192_SET_PIN			GPIO_Pin_10
#define AD7192_SET_LOW()      	AD7192_SET_GPIO->BSRRH = AD7192_SET_PIN
#define AD7192_SET_HIGH()     	AD7192_SET_GPIO->BSRRL = AD7192_SET_PIN


/* AD7192 Reset*/
#define AD7192_RESET_GPIO	GPIOD
#define AD7192_RESET_PIN		GPIO_Pin_7
#define AD7192_RESET_LOW()      AD7192_RESET_GPIO->BSRRH = AD7192_RESET_PIN
#define AD7192_RESET_HIGH()     AD7192_RESET_GPIO->BSRRL = AD7192_RESET_PIN

void bsp_InitSPI2(void);
void Init_GPIO(void);
unsigned char AD7192_SendByte(unsigned char byte);
void AD7192_Write(u8 Register_Slected,u32 Data);
u32 AD7192_Read(u8 Register_Slected);
void Init_AD7192(void);
void AD7192StartContinuousConvertion(void);
u32 AD7192ReadConvertingData(void);
void Mag_Set(int delaytime);
void Mag_Reset(int delaytime);
void Get_Mag_Read(double* M);
void Get_Mag_Result(double* Mag);
double AD7192ReadTemperature(void);
#endif

