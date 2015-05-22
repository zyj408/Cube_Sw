/*
*********************************************************************************************************
*
*	ģ������ : �ײ�����ģ��
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ���
*	 	       Ӧ�ó���ֻ�� #include bsp.h ���ɣ�����Ҫ#include ÿ��ģ��� h �ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����         ����    ˵��
*		v1.0    2012-12-17  Eric2013  ST�̼���V1.0.2�汾��
*	
*	Copyright (C), 2013-2014
*   QQ����Ⱥ��216681322
*   BLOG: http://blog.sina.com.cn/u/2565749395
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

#include "stm32f4xx.h"

#include <stdio.h>			/* ��Ϊ�õ���printf���������Ա����������ļ� */
#include "string.h"

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_uart_fifo.h"
#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_nor_flash.h"
#include "bsp_rtc.h"
#include "bsp_iwdg.h"
#include "bsp_cpu_flash.h"
#include "bsp_sdio_sd.h"
#include "bsp_fsmc_sram.h"



//#include "bsp_spi_flash.h"
//#include "bsp_uart.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_eeprom_24xx.h"
//#include "bsp_si4730.h"
//#include "bsp_hmc5883l.h"
//#include "bsp_mpu6050.h"
//#include "bsp_bh1750.h"
//#include "bsp_bmp085.h"
//#include "bsp_wm8978.h"

//#include "bsp_nand_flash.h"

//#include "LCD_RA8875.h"
//#include "LCD_SPFD5420.h"
//#include "bsp_touch.h"
//#include "bsp_camera.h"
//#include "bsp_ad7606.h"
//#include "bsp_gps.h"
//#include "bsp_oled.h"
//#include "bsp_mg323.h"


#include "bsp_spi_eps.h"
#include "bsp_spi_motor.h"
#include "bsp_adc.h"
#include "bsp_pwm.h"
#include "bsp_PVD.h"
#include "bsp_rng.h"
#include "bsp_gps.h"
#include "bsp_ReceiveDeal.h"
#include "ad7490.h"
#include "bsp_i2c0.h"
#include "bsp_i2c1.h"
#include "bsp_switch.h"
#include "AD7192.h"
#include "bsp_Fipex.h"


void bsp_Init(void);
void bsp_DelayUS(uint32_t _ulDelayTime);
void BSP_Tick_Init (void);
CPU_INT32U  BSP_CPU_ClkFreq (void);
static void NVIC_Configuration(void);

#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

#define SD_FILESYSTEM_ENABLE   0

void DEBUG_LOG(const char *format, ...);
#endif
