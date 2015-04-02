#ifndef __BSP_ADC_H
#define __BSP_ADC_H

/* OBC板模拟采样片选引脚 */
#define OBC_CS_GPIO		     GPIOB
#define OBC_CS_PIN			   GPIO_Pin_1
#define OBC_CS_LOW()       OBC_CS_GPIO->BSRRH = OBC_CS_PIN
#define OBC_CS_HIGH()      OBC_CS_GPIO->BSRRL = OBC_CS_PIN
#define OBC_SPI_BAUD			 SPI_BaudRatePrescaler_256

/* EPS板模拟采样片选引脚 */
#define EPS_CS1_GPIO		   GPIOA
#define EPS_CS1_PIN			   GPIO_Pin_4
#define EPS_CS1_LOW()      EPS_CS1_GPIO->BSRRH = EPS_CS1_PIN
#define EPS_CS1_HIGH()     EPS_CS1_GPIO->BSRRL = EPS_CS1_PIN

/* EPS板模拟采样片选引脚2 */
#define EPS_CS2_GPIO		   GPIOA
#define EPS_CS2_PIN			   GPIO_Pin_15
#define EPS_CS2_LOW()      EPS_CS2_GPIO->BSRRH = EPS_CS2_PIN
#define EPS_CS2_HIGH()     EPS_CS2_GPIO->BSRRL = EPS_CS2_PIN

/* OBC板模拟采样极值 */
#define OBC_AD_HIGH         0x0FFF
#define OBC_AD_LOW          0x0010

/* OBC板模拟采样极值 */
#define EPS_AD_HIGH         0x0FFF
#define EPS_AD_LOW          0x0010

/* OBC板模拟采样极值 */
#define EPS_AD_CS1          0x01
#define EPS_AD_CS2          0x02
#define OBC_AD_CS1          0x03

void bsp_InitSPI1(void);
static void InitSPI1(void);
static void InitSPI1_GPIO(void);
void SPI1_IRQHandler(void);
void ObcDataProcess(uint16_t data);
void ObcAdStart(void);
uint16_t ObcSendByte(uint16_t _ucValue);
void EpsAdStart(void);
uint8_t EpsAdUpdate(uint8_t chip);
uint16_t EpsSendByte(uint16_t _ucValue, uint8_t _chipNum);
void AdDataFliter(uint16_t ad_table[][6], uint8_t channel_num);
uint8_t ObcAdUpdate(void);
#endif
