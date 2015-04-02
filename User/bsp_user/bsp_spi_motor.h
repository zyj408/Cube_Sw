#ifndef  BSP_MOTOR_SPI_H
#define  BSP_MOTOR_SPI_H


#define SET_SYNC()		GPIO_SetBits(GPIOB,GPIO_Pin_12)	//P4.6->SYNC
#define CLR_SYNC()		GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define Read  0x800000
#define Write 0x000000

//通道选择
#define DAC_Channel_A    0x000000
#define DAC_Channel_B    0x020000
#define DAC_Channel_ALL  0x040000

//寄存器选择
#define DAC_Register                  0x000000
#define Output_Range_Select_Register  0x080000
#define Power_Control_Register        0x100000
#define Control_Register              0x180000

// Output Range Config
/*
Range1: 0~5V
Range2: 0~10V
Range3: 0~10.8V
Range4: -5~+5V
Range5: -10~+10V
Range6: -10.8~+10.8V
*/

#define Range1_Select 0x000000
#define Range2_Select 0x000001
#define Range3_Select 0x000002
#define Range4_Select 0x000003
#define Range5_Select 0x000004
#define Range6_Select 0x000005

// 电压控制
#define PowerUp_ALL         0x00001F

#define PowerUp_Channel_A   0x000001
#define PowerDown_Channel_A 0xFFFFFE

#define PowerUp_Channel_B   0x000002
#define PowerDown_Channel_B 0xFFFFFD

#define PowerUp_Channel_C   0x000004
#define PowerDown_Channel_C 0xFFFFFB

#define PowerUp_Channel_D   0x000008
#define PowerDown_Channel_D 0xFFFFF7

#define PowerUp_REF         0x000010
#define PowerDown_REF       0xFFFFEF

//General Config

#define Nop 0x180000 //for readback

//Control Register

#define Control        0x190000

#define TSD_Enable     0x000008
#define TSD_Disable    0x000000

#define Clamp_Enable   0x000004
#define TSD_Disable    0x000000
					  
#define Clear_Select0  0x000002
#define Clear_Select1  0x000000

#define SDO_Disable    0x000001
#define SDO_Enable     0x000000

#define Clear 0x1C0000
#define Load 0x1D0000


void ConfigAD5754R(void);
unsigned char MotorChange(unsigned int val);
unsigned char AD5722_SendByte(unsigned char byte);
void AD5722_RegEdit(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte);
void AD5722_Write(unsigned char code);
void WriteToAD5754RViaSpi(long int *RegisterData);
void ConfigAD5754R(void);
void bsp_InitSPI2_GPIO(void);
void bsp_InitSPI2(void);
void InitSPI2(void);
#endif
