#ifndef _BSP_SPI_EPS_H_
#define _BSP_SPI_EPS_H_

#define EPS_CS_GPIO		     GPIOA
#define EPS_CS_PIN			   GPIO_Pin_4
#define EPS_CS_LOW()       EPS_CS_GPIO->BSRRH = EPS_CS_PIN
#define EPS_CS_HIGH()      EPS_CS_GPIO->BSRRL = EPS_CS_PIN
#define SPI_BAUD			     SPI_BaudRatePrescaler_256


/*
void bsp_InitSPI1(void);
static void InitSPI1(void);
static void InitSPI1_GPIO(void);
uint16_t EPS_SendByte(uint16_t _ucValue);
void EpsDataProcess(uint16_t data);
void EPS_Init(void);
unsigned char EPS_Update(void);
*/				

#endif
