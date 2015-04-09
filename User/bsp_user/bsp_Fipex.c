#include <includes.h>


uint8_t FepixPowerOnFlg = 0;

uint8_t bsp_FipexSendCmd(uint8_t *cmd, uint16_t length)
{
	uint8_t command_temp[32] = {0};
	uint16_t length_temp;

	length_temp = length;
	Mem_Copy(command_temp, cmd, length_temp);  //将数据拷贝至局部变量
	comWaitTxFifoEmpty(COM3); // 等待串口3数据清空
	
	comSendBuf(COM3, command_temp, length);
	return 0;
}



void bsp_FipexSwitchInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_FIPEX_3V3_PORT | RCC_FIPEX_5V_PORT, ENABLE);

	/* 配置GPIO */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_3V3_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_3V3_EN_PORT, &GPIO_InitStructure);
	
	FIPEX_3V3_DISABLE;  //拉低3.3V引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_5V_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_5V_EN_PORT, &GPIO_InitStructure);
	FIPEX_5V_DISABLE;   //拉低5V引脚
}

uint8_t bsp_FipexPowerOn(void)
{
  uint32_t FipexTimeOut_ms;
	
	FIPEX_3V3_ENABLE;//Fipex 3.3伏电源上电
	
	FipexTimeOut_ms = 0;
	while((FIPEX_3V3_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//检查上电引脚是否拉高(时间限制300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//如果超时todo
	{
		FIPEX_3V3_DISABLE;
		
		#if debug_enable
		printf("Fipex 3v3 power on fail!\r\n");
		#endif
		
		return 1;  //3.3V上电错误
	}
	
	
	BSP_OS_TimeDlyMs(10);  //上电之间延迟
	
	FIPEX_5V_ENABLE;	//Fipex5伏电源上电
	
	FipexTimeOut_ms = 0;
	while((FIPEX_5V_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//检查上电引脚是否拉高(时间限制300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//如果超时todo
	{
		FIPEX_5V_DISABLE;
		FIPEX_3V3_DISABLE;
		
		#if debug_enable
		printf("Fipex 5V power on fail!\r\n");
		#endif
		
		return 2;  //5V上电错误
	}
	else
	{
		FepixPowerOnFlg = 1;  //FEPIX上电标志位置位
		
		#if debug_enable
		printf("Fipex power on successfully!\r\n");
		#endif
	}
	BSP_OS_TimeDlyMs(1000);	//等待1000ms
	
	
	//串口发送启动指令
	return 0;
}

uint8_t bsp_FipexPowerOff(void)
{
	uint32_t FipexTimeOut_ms;
	
	//串口发送关闭指令
	
	//回复正确
	
	FIPEX_5V_DISABLE;//关闭Fipex 5伏电源供电
	
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//检查5伏供电是否关闭(时间限制200ms)
	{
		bsp_DelayUS(1);
	}
	
	if(FipexTimeOut_ms > 290*1000)
	{
		#if debug_enable
			printf("Fipex power 5V off fail!\r\n");
		#endif
		return 1;
	}
	
	FIPEX_3V3_DISABLE;//关闭Fipex3.3伏供电
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//检查3.3伏供电是否关闭(时间限制200ms)
	{
		bsp_DelayUS(1);
	}

	if(FipexTimeOut_ms > 290*1000)
	{
		#if debug_enable
			printf("Fipex power 3V3 off fail!\r\n");
		#endif
		return 2;
	}
	else
	{
		FepixPowerOnFlg = 0;  //FEPIX上电标志位置位
		
		#if debug_enable
		printf("Fipex power off successfully!\r\n");
		#endif
	}
	
	return 0;
}

uint8_t bsp_FipexGetCheckSum(unsigned char *cmd, unsigned char* checksum)
{
	unsigned char checksum_temp = 0;
	unsigned char length = 0;

	if (*cmd == 0x7E)  //首字母是否等于0x7E
	{
		length = cmd[2];  //cmd[1]到cmd[length+3]进行校验
		length += 3;
		while (length)
		{
			checksum_temp ^= cmd[length];
			length--;
		}
		*checksum = checksum_temp;
		return 0;
	}
	else
	{
		return 1;  //帧头不是0x7E
	}
}
