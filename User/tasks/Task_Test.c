#include  <includes.h>

uint8_t TestBuf[116];
char DebugMode = '0';
char gps_flg = 0;
char semple_flg = 0;

char rtc_info_switch = 0;
char flash_read_switch = 0;
char flash_write_switch = 0;
char flash_eraser_switch = 0;

char gpsa_sw = 2;
char gpsb_sw = 2;
char mtq_sw = 2;
char wheela_sw = 2;
char wheelb_sw = 2;



void TEST_TASK(void *p_arg)
{	
	int j = 0;
	uint8_t response = 0;
	(void)p_arg;

	printf("--------On Broad Computer Debug Mode--------\r\n");
  printf("--------------------------------------------\r\n");
	printf("[1] RTC Function Test\r\n");
	printf("[2] NorFlash Function Test\r\n");
	printf("[3] EPSSystem Function Test\r\n");
	
	while(1)
	{
		
		
		#if debug_enable
		if (comGetChar(COM1, &response))
		{
		switch(DebugMode)
		{
			case '0':
				DebugMode = response;
			break;
				
			case '1':
				if(response != '1')
					DebugMode = '0';
			break;
				
			case '2':
				if(response == '1')
				{
					TestBuf[0] = 0x11;
					TestBuf[1] = 0x22;
					TestBuf[2] = 0x33;
					TestBuf[3] = 0x44;
					TestBuf[4] = 0x55;
	
					if(NOR_WriteBuffer(TestBuf, NOR_FLASH_SA1, 5) == NOR_SUCCESS)
						printf("Write data sucessfully \r\n");
					else
						printf("Write data error \r\n");
						
				}
				else if(response == '2')
				{
					printf("Read data \r\n");
					for(j=0; j<128; j++)
					{				
						NOR_ReadBuffer(TestBuf, NOR_FLASH_SA1 + j, 1);
						printf("%x ", TestBuf[0]);
						if(j%16 == 0)
							printf("\r\n");
					}
				}
				else if(response == '3')
		   	{
					if(NOR_EraseSector(NOR_FLASH_SA1) == NOR_SUCCESS)
					{
						printf("Eraser NorFlash block (Section1)successfully \r\n");
				  }
					else
					{
						printf("Eraser NorFlash block (Section1)error \r\n");
					}
				}
				else
					DebugMode = '0';
				break;
			}
			
			switch(DebugMode)
			{
				case '0':
					printf("--------On Broad Computer Debug Mode--------\r\n");
					printf("--------------------------------------------\r\n");
					printf("[1] RTC Function Test\r\n");
					printf("[2] NorFlash Function Test\r\n");
					printf("[3] EPSSystem Function Test\r\n");
				break;
					
				case '1':
					printf("--------------RTC Function Test-------------\r\n");
					printf("            RTC base infomation:\r\n");
					if(OBCBootInfo.BootRTC_Source == 0)
						printf("RTC current source: LSE\r\n");
					else if(OBCBootInfo.BootRTC_Source == 1)
					 printf("RTC current source: LSI\r\n");
					else
					 printf("RTC have No source!\r\n");
					printf("Current RTC :%d:%d:%d\r\n", CurTime.RTC_Hours, CurTime.RTC_Minutes, CurTime.RTC_Seconds);	
					printf("Fuction:\r\n");
					printf("[1] Update current time\r\n");
				break;
					
				case '2':
					printf("--------------NorFlash Function Test-------------\r\n");
					printf("           NorFlash storage infomation\r\n");
					printf("Tel Data Storage Points: %x. Percent:%.3f\r\n", TelCurPtr, TelCurPtr/(float)(NOR_FLASH_SA32 - NOR_FLASH_SA0));
					printf("GPS Data Storage Points: %x. Percent:%.3f\r\n", GpsCurPtr, (GpsCurPtr - NOR_FLASH_SA32)/(float)(NOR_FLASH_SA62 - NOR_FLASH_SA32));
					printf("Fuction:\r\n");
					printf("[1] write NorFlash block (add: Section1, data:0x11, 0x22, 0x33, 0x44, 0x55)\r\n");
					printf("[2] read NorFlash block (Section1)\r\n");
				  printf("[3] Eraser NorFlash block (Section1)\r\n");
				break;
			}
			
		}
		#endif	
			
		#if	debug_adcs
			if(gps_flg != 0)
			{	
				gps_flg = 0;
				BSP_OS_SemPost(&SEM_ORB_DET);
			}
			if(semple_flg != 0)
			{
				semple_flg = 0;
				BSP_OS_SemPost(&SEM_CYC_INFO);
			}
			
		#endif 
			
			
			
		
		
		/* ADC1DMA²âÊÔ */
		#if 0
			TestBuf[0] = (char)(CurTime.RTC_Hours);
			TestBuf[1] = (char)(CurTime.RTC_Minutes);
			TestBuf[2] = (char)(CurTime.RTC_Seconds);
			TestBuf[3] = (char)(TelCurPtr >> 24 & 0x00FF);
			TestBuf[4] = (char)(TelCurPtr >> 16 & 0x00FF);
			TestBuf[5] = (char)(TelCurPtr >>  8 & 0x00FF);
			TestBuf[6] = (char)(TelCurPtr >>  0 & 0x00FF);
			TestBuf[7] = (char)(GpsCurPtr >> 24 & 0x00FF);
			TestBuf[8] = (char)(GpsCurPtr >> 16 & 0x00FF);
			TestBuf[9] = (char)(GpsCurPtr >>  8 & 0x00FF);
			TestBuf[10] = (char)(GpsCurPtr >> 0 & 0x00FF);
			TestBuf[11] = (char)(PldCurPtr >> 24 & 0x00FF);
			TestBuf[12] = (char)(PldCurPtr >> 16 & 0x00FF);
			TestBuf[13] = (char)(PldCurPtr >> 8 & 0x00FF);
			TestBuf[14] = (char)(PldCurPtr >> 0 & 0x00FF);
			TestBuf[15] = (char)(TelOverRangeCnt);
			TestBuf[16] = (char)(GpsOverRangeCnt);
			TestBuf[17] = (char)(PldOverRangeCnt);
			
			for(j=0; j<32; j++)
			{
				TestBuf[18+2*j] = (char)(EpsAdValue[j][5] >> 8 & 0x00FF);
				TestBuf[18+2*j+1] = (char)(EpsAdValue[j][5] >> 0 & 0x00FF);	
			}
			
			UartSend(USART1,0x11);
			UartSend(USART1,0x22);
			
			for(j = 0; j < 116; j++)
			{
				UartSend(USART1,TestBuf[j]);
			}
			UartSend(USART1,0xFE);
			UartSend(USART1,0xFF);
		#endif
		
		/* NORFLASH²âÊÔ */
		#if 0
		TestCnt_60s++;
		if(TestCnt_60s>=60)
		{
			TestCnt_60s=0;
			
			
			if(Test_NOR_Flash(FlashSector))
			{
				FlashTestErrCnt++;
			}
			
			FlashSector++;
			if(FlashSector>40)
			{
				FlashSector=0;
			}
			
		}
		
	#endif
		
	#if 1
		if(gpsa_sw != 2)
		{
			if(gpsa_sw == 1)
			{
				gpsa_sw = 2;
				SW_GPSA_ENABLE;
			}
			if(gpsa_sw == 0)
			{
				gpsa_sw = 2;		
				SW_GPSA_DISABLE;				
			}
		}

		if(gpsb_sw != 2)
		{
			if(gpsb_sw == 1)
			{
				gpsb_sw = 2;
				SW_GPSB_ENABLE;
			}
			if(gpsb_sw == 0)
			{
				gpsb_sw = 2;		
				SW_GPSB_DISABLE;				
			}
		}
		
		if(gpsa_sw != 2)
		{
			if(gpsa_sw == 1)
			{
				gpsa_sw = 2;
				SW_GPSA_ENABLE;
			}
			if(gpsa_sw == 0)
			{
				gpsa_sw = 2;		
				SW_GPSA_DISABLE;				
			}
		}
		
		if(mtq_sw != 2)
		{
			if(mtq_sw == 1)
			{
				mtq_sw = 2;
				SW_MTQ_ENABLE;
			}
			if(mtq_sw == 0)
			{
				mtq_sw = 2;		
				SW_MTQ_DISABLE;			
				
			}
		}		
		
		if(wheela_sw != 2)
		{
			if(wheela_sw == 1)
			{
				wheela_sw = 2;
				SW_WHEELA_ENABLE;
			}
			if(wheela_sw == 0)
			{
				wheela_sw = 2;		
				SW_WHEELA_DISABLE;				
			}
		}		
		
		if(wheelb_sw != 2)
		{
			if(wheelb_sw == 1)
			{
				wheelb_sw = 2;
				SW_WHEELB_ENABLE;
			}
			if(wheelb_sw == 0)
			{
				wheelb_sw = 2;		
				SW_WHEELB_DISABLE;				
			}
		}	
	#endif

	BSP_OS_TimeDlyMs(100);
		
	
	}
	
	
}
