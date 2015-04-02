#ifndef __BSP_I2C1_H_
#define __BSP_I2C1_H_


#define I2C1_WR	0		/* Ð´¿ØÖÆbit */
#define I2C1_RD	1		/* ¶Á¿ØÖÆbit */

void bsp_InitI2C1(void);
static void i2c1_Delay(void);
void i2c1_Start(void);
void i2c1_Stop(void);
void i2c1_SendByte(uint8_t _ucByte);
uint8_t i2c1_ReadByte(void);
uint8_t i2c1_WaitAck(void);
void i2c1_Ack(void);
void i2c1_NAck(void);
uint8_t i2c1_CheckDevice(uint8_t _Address);
#endif
