#include <includes.h>
#include "globalavr.h"

const int AD_Channel[16] =  {CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3,
		                         CHANNEL_4, CHANNEL_5, CHANNEL_6, CHANNEL_7,
		                         CHANNEL_8, CHANNEL_9, CHANNEL_10,CHANNEL_11,
		                         CHANNEL_12,CHANNEL_13,CHANNEL_14,CHANNEL_15};

void bsp_InitSPI1(void)
{
	InitSPI1_GPIO();
	InitSPI1();
}
static void InitSPI1(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	Mem_Set(&SPI_InitStructure, 0x00, sizeof(SPI_InitTypeDef));
	Mem_Set(&NVIC_InitStructure, 0x00, sizeof(NVIC_InitTypeDef));
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_Cmd(SPI1, DISABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = OBC_SPI_BAUD;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
		
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void InitSPI1_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	Mem_Set(&GPIO_InitStructure, 0x00, sizeof(GPIO_InitTypeDef));
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* 配置MSK、MISO、MOSI复用功能 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	/* 配置MSK、MISO、MOSI引脚功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 配置片选线 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = OBC_CS_PIN;
	GPIO_Init(OBC_CS_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = EPS_CS1_PIN;
	GPIO_Init(EPS_CS1_GPIO, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = EPS_CS2_PIN;
	GPIO_Init(EPS_CS2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = TEMP_CS_PIN;
	GPIO_Init(TEMP_CS_GPIO, &GPIO_InitStructure);
	
//TEMP_CS_LOW()
	TEMP_CS_HIGH();
	OBC_CS_HIGH();
	EPS_CS1_HIGH();
	EPS_CS2_HIGH();
}

void SPI1_IRQHandler(void)
{
	OSIntEnter();
	if(SPI_I2S_GetITStatus(SPI1, SPI_IT_CRCERR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_IT_CRCERR);
		ObcCommErr++;
	}
	
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_OVR);
		ObcCommErr++;
	}	
	
	if(SPI_I2S_GetITStatus(SPI1, SPI_IT_MODF) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_IT_MODF);
		ObcCommErr++;
	}
	OSIntExit();
}

CPU_INT16U TempSendByte(uint16_t _ucValue)
{
	CPU_INT32U i=0;
	/* 等待上次数据发送完毕 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		__nop();__nop();__nop();;
		if(i++ > 200000)
		{
			//EpsTranOTCnt++;
			return 1;
		}
	}
	TEMP_CS_LOW();
	/* 通过SPI1发送一个字节 */
	SPI_I2S_SendData(SPI1, _ucValue);

	/* 等待接接收一个字节完成 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		__nop();__nop();__nop();;
	  if(i++ > 200000)
	  {
		  //EpsRevOTCnt++;
	  	
	  	return 1;
	  }
  }
	/* 返回SPI接收字节 */
	TEMP_CS_HIGH();
	return SPI_I2S_ReceiveData(SPI1);
}


void TempAdStart(void)
{
	CPU_INT16U command;
	command = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_DOUBLE | DATA_BIN;
  
	TempSendByte(command);
	__nop();__nop();__nop();;
	TempSendByte(ALL_CHANNEL);

}

void TempDataProcess(CPU_INT16U data)
{
	CPU_INT08U index;
	CPU_INT08U channel;
	CPU_INT16U ad_temp;

	channel = (CPU_INT08U)(data >> 12);  /* 获取转换的通道 */
	ad_temp = data & 0x0FFF;  /* 获取该通道的AD转换值 */

	if(ad_temp<TEMP_AD_HIGH && ad_temp>TEMP_AD_LOW)  /* 所采样的AD值在正常范围内 */
	{
		for(index=0; index<4; index++)
		{
			TempAdValue[channel][index] = TempAdValue[channel][index+1];
		}
		TempAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
	}
	else  /* AD值错误 */
	{
		
		for(index=0; index<4; index++)
		{
			TempAdValue[channel][index] = TempAdValue[channel][index+1];
		}
		TempAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
		
		TempAdErr[channel]++;
	}	
}

CPU_INT08U TempAdUpdate(void)
{
	CPU_INT16U data;

	data = TempSendByte(DATA_UPDATE);

	TempDataProcess(data);
	
	return 0;
}

void ObcDataProcess(CPU_INT16U data)
{
	CPU_INT08U index;
	CPU_INT08U channel;
	CPU_INT16U ad_temp;

	channel = (CPU_INT08U)(data >> 12);  /* 获取转换的通道 */
	ad_temp = data & 0x0FFF;  /* 获取该通道的AD转换值 */

	if(ad_temp<OBC_AD_HIGH && ad_temp>OBC_AD_LOW)  /* 所采样的AD值在正常范围内 */
	{
		for(index=0; index<4; index++)
		{
			ObcAdValue[channel][index] = ObcAdValue[channel][index+1];
		}
		ObcAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
	}
	else  /* AD值错误 */
	{
		
		for(index=0; index<4; index++)
		{
			ObcAdValue[channel][index] = ObcAdValue[channel][index+1];
		}
		ObcAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
		
		ObcAdErr[channel]++;
	}
}

void ObcAdStart(void)
{
	CPU_INT16U command;
	command = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_DOUBLE | DATA_BIN;
  
	ObcSendByte(command);
	__nop();__nop();__nop();;
	ObcSendByte(ALL_CHANNEL);

}

CPU_INT08U ObcAdUpdate(void)
{
	CPU_INT16U data;

	data = ObcSendByte(DATA_UPDATE);

	ObcDataProcess(data);
	
	return 0;
}

CPU_INT16U ObcSendByte(uint16_t _ucValue)
{
	CPU_INT32U i=0;
	/* 等待上次数据发送完毕 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		__nop();__nop();__nop();;
		if(i++ > 200000)
		{
			//EpsTranOTCnt++;
			return 1;
		}
	}
	OBC_CS_LOW();
	/* 通过SPI1发送一个字节 */
	SPI_I2S_SendData(SPI1, _ucValue);

	/* 等待接接收一个字节完成 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		__nop();__nop();__nop();;
	  if(i++ > 200000)
	  {
		  //EpsRevOTCnt++;
	  	
	  	return 1;
	  }
  }
	/* 返回SPI接收字节 */
	OBC_CS_HIGH();
	return SPI_I2S_ReceiveData(SPI1);
}

void EpsAdStart(void)
{
	CPU_INT16U command;
	command = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_NORMAL | DATA_BIN;
  
	EpsSendByte(command, EPS_AD_CS1);
	__nop();__nop();__nop();
	EpsSendByte(ALL_CHANNEL, EPS_AD_CS1);

	EpsSendByte(command, EPS_AD_CS2);
	__nop();__nop();__nop();
	EpsSendByte(ALL_CHANNEL, EPS_AD_CS2);
}

void EpsDataProcess(CPU_INT16U data, CPU_INT08U chipNum)
{
	CPU_INT08U index;
	CPU_INT08U channel;
	CPU_INT16U ad_temp;

	channel = (CPU_INT08U)(data >> 12);  /* 获取转换的通道 */
	ad_temp = data & 0x0FFF;  /* 获取该通道的AD转换值 */

	if(chipNum == EPS_AD_CS2)  /* 如果为AD2采样，则通道数加上偏移 */
	{
		channel += 16;
	}
	if(ad_temp<EPS_AD_HIGH && ad_temp>EPS_AD_LOW)  /* 所采样的AD值在正常范围内 */
	{
		for(index=0; index<4; index++)
		{
			EpsAdValue[channel][index] = EpsAdValue[channel][index+1];
		}
		EpsAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
	}
	else  /* AD值错误 */
	{
		
		for(index=0; index<4; index++)
		{
			EpsAdValue[channel][index] = EpsAdValue[channel][index+1];
		}
		EpsAdValue[channel][4] = ad_temp;  /* 将AD值传入AD矩阵 */
		
		EpsAdErr[channel]++;
	}
}

CPU_INT08U EpsAdUpdate(CPU_INT08U chip)
{
	CPU_INT16U data;

	data = EpsSendByte(DATA_UPDATE, chip);

	EpsDataProcess(data, chip);
	
	return 0;
}

CPU_INT16U EpsSendByte(uint16_t _ucValue, uint8_t _chipNum)
{
	CPU_INT32U i=0;
	/* 等待上次数据发送完毕 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		__nop();__nop();__nop();
		if(i++ > 200000)
		{
			EpsTranOTCnt++;
			return 1;
		}
	}
	
	if(_chipNum == EPS_AD_CS1)
	{
		EPS_CS1_LOW();
	}
	else if(_chipNum == EPS_AD_CS2)
	{
		EPS_CS2_LOW();
	}		
	
	/* 通过SPI1发送一个字节 */
	SPI_I2S_SendData(SPI1, _ucValue);

	/* 等待接接收一个字节完成 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		__nop();__nop();__nop();
	  if(i++ > 200000)
	  {
		  EpsRevOTCnt++;
	  	
	  	return 1;
	  }
  }
	/* 返回SPI接收字节 */
		if(_chipNum == EPS_AD_CS1)
	{
		EPS_CS1_HIGH();
	}
	else if(_chipNum == EPS_AD_CS2)
	{
		EPS_CS2_HIGH();
	}		
	
	return SPI_I2S_ReceiveData(SPI1);
}

void AdDataFliter(CPU_INT16U ad_table[][6], CPU_INT08U channel_num)
{
	CPU_INT16U max;
	CPU_INT16U min;
	CPU_INT16U sum;
	CPU_INT08U channel_temp;
	CPU_INT08U index;
	
	for(channel_temp=0; channel_temp<channel_num; channel_temp++)
	{
		max = ad_table[channel_temp][0];
		min = ad_table[channel_temp][0];
		sum = 0;
		for(index=0; index<5; index++)
		{
			max = ad_table[channel_temp][index] > max? ad_table[channel_temp][index]: max;
			min = ad_table[channel_temp][index] < min? ad_table[channel_temp][index]: min;
			sum = sum + ad_table[channel_temp][index];
		}
		
		ad_table[channel_temp][5] = (sum - max - min) / 3;
	}
}
