/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H


void bsp_InitRTC(void);
void RTC_Config(void);
void bsp_RTCSet(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
uint32_t bsp_UTCTimeGet(void);

#endif

