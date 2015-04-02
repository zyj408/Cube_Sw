#ifndef __BSP_UART_H
#define __BSP_UART_H

void bsp_InitUart(void);
uint8_t UartSend(USART_TypeDef* USARTx, uint16_t Data);
void GrandTest_Init(void);
void GpsB_Init(void);
void Payload_Init(void);
void GpsA_Init(void);
void UsbComm_Init(void);
void UartSendString(USART_TypeDef* USARTx, unsigned char *Str);
#endif


