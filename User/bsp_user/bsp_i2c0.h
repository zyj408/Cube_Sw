#ifndef __BSP_I2C0_H_
#define __BSP_I2C0_H_


#define I2C0_WR	0		/* Ð´¿ØÖÆbit */
#define I2C0_RD	1		/* ¶Á¿ØÖÆbit */

void bsp_InitI2C0(void);
static void i2c0_Delay(void);
void i2c0_Start(void);
void i2c0_Stop(void);
void i2c0_SendByte(uint8_t _ucByte);
uint8_t i2c0_ReadByte(void);
uint8_t i2c0_WaitAck(void);
void i2c0_Ack(void);
void i2c0_NAck(void);
uint8_t i2c0_CheckDevice(uint8_t _Address);
#endif
