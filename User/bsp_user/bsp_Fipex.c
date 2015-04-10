#include <includes.h>

struct FipexOperationParaStr FipexOperationPara;  //8条缓冲指令

const uint8_t FipexCmdDeft0[6] = {0x7E, 0x0B, 0x00, 0x0B, 0x3C, 0x00};
const uint8_t FipexCmdDeft1[9] = {0x7E, 0x11, 0x03, 0x04, 0x01, 0x00, 0x17, 0xFF, 0xFF};
const uint8_t FipexCmdDeft2[9] = {0x7E, 0x11, 0x03, 0x05, 0x10, 0x0A, 0x0D, 0xFF, 0xFF};
const uint8_t FipexCmdDeft3[9] = {0x7E, 0x11, 0x03, 0x02, 0xC8, 0x00, 0xD8, 0xFF, 0xFF};
const uint8_t FipexCmdDeft4[6] = {0x7E, 0x0C, 0x00, 0x0C, 0x2C, 0x01};
const uint8_t FipexCmdDeft5[6] = {0x7E, 0x20, 0x00, 0x20, 0xFF, 0xFF};
const uint8_t FipexCmdDeft6[6] = {0x7E, 0x21, 0x00, 0x21, 0xFF, 0xFF};


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

void FipexCmdRead(void)  //从CPU_INTER读取数据
{
	#if debug_enable
	printf("Upload Fipex Command from CPU internal Flash\r\n");
	#endif 
	
	bsp_ReadCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara));
}

uint8_t FipexCmdUpdate(uint8_t *cmd, uint8_t index, uint8_t length)
{
	FipexOperationPara.FipexCmdInfo[index].CmdLength = length;
	
	Mem_Set(&FipexOperationPara.FipexCmdInfo[index].FipexCmd[0], 0x00, 32);
	Mem_Copy(&FipexOperationPara.FipexCmdInfo[index].FipexCmd[0], cmd, length);
	
	return 0;
}

uint8_t FipexCmdStore(void)
{
	uint8_t TryTime;
	TryTime = 2;
	while(bsp_WriteCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara)) && TryTime--);
	
	if(!TryTime)
	{
		#if debug_enable
			printf("Internal Flash program error!\r\n");
		#endif
	}
	return 0;
}

void FipexSetDefaultInfo(void)
{
	FipexCmdUpdate((uint8_t *)FipexCmdDeft0, 0, FipexCmdDeft0[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft1, 1, FipexCmdDeft1[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft2, 2, FipexCmdDeft2[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft3, 3, FipexCmdDeft3[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft4, 4, FipexCmdDeft4[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft5, 5, FipexCmdDeft5[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft6, 6, FipexCmdDeft6[2] + 0x06);
	FipexOperationPara.StartTime = 0x1A56BFC0;
	FipexOperationPara.RepeatTime = 0x0E10;
	FipexOperationPara.CmdCnt = 10;
	
	FipexCmdStore();
}
uint8_t FipexInfoGet(uint8_t* cmd, uint8_t size)
{
	uint8_t i;
	int16_t length = (int16_t)cmd[0]; //指令长度
	uint8_t cmd_cnt = 0;
	uint8_t *cmd_ptr;
	uint8_t *delay_ptr;
	
	
	Mem_Set((uint8_t *)&FipexOperationPara, 0x00, sizeof(FipexOperationPara));
/*******************************************************/	
	for(i=4; i>0; i--)
	{
		FipexOperationPara.StartTime = (FipexOperationPara.StartTime | cmd[i]) << 8;
	}
	length -= 4;
/*******************************************************/
	for(i=6; i>4; i--)
	{
		FipexOperationPara.RepeatTime = (FipexOperationPara.RepeatTime | cmd[i]) << 8;
	}
	length -= 2;
/*******************************************************/	
	cmd_cnt = cmd[7];
	
	
	cmd_ptr = &cmd[8];
	for(i=0; i<cmd_cnt; i++)
	{
		if(*cmd_ptr != 0x7E) //不是同步字
		{
			#if debug_enable
				printf("Sync error!\r\n");
			#endif		
			return 1;  //检测同步字错误	
		}
		delay_ptr = (cmd_ptr+cmd_ptr[2]+4);
		
		Mem_Copy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+4);
		FipexOperationPara.FipexCmdInfo[i].CmdLength = cmd_ptr[2]+4;
		FipexOperationPara.FipexCmdInfo[i].CmdDelay = *(delay_ptr + 1);
		FipexOperationPara.FipexCmdInfo[i].CmdDelay = (FipexOperationPara.FipexCmdInfo[i].CmdDelay << 8) | *(delay_ptr);
		
		if(length -  6 - cmd_ptr[2]< 0)
		{
			#if debug_enable
				printf("Size error!\r\n");
			#endif	
			return 2; //字符检测错误

		}
			
		cmd_ptr = delay_ptr + 2;
	}
	
	return 0;
}
	
void FipexInfomationInit(void)  //从FLASH中读取指令信息
{
	#if debug_enable
		char i, j;
	#endif
	
	#if debug_enable
	printf("Read Fipex Command from CPU Flash\r\n");
	#endif 
	
	bsp_ReadCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara));

	#if debug_enable
	printf("Fipex Command infomation:\r\n");
	
	for(i=0; i<8; i++)
	{
		printf("Cmd: ");
		for(j=0; j<32; j++)
		{
			printf("%x ", FipexOperationPara.FipexCmdInfo[i].FipexCmd[j]);
		}
		printf("||Command Delay: %d\r\n", FipexOperationPara.FipexCmdInfo[i].CmdDelay);
	}
	#endif 
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
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	
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
	
	
	//bsp_FipexSendCmd(FipexCmd, 8);//串口发送启动指令
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
