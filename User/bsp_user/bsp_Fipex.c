#include <includes.h>

struct FipexOperationParaStr FipexOperationPara;  //8条缓冲指令


const uint8_t FipexCmdDeft0[6] = {0x7E, 0x0F, 0x00, 0x0B, 0x3C, 0x00};
const uint8_t FipexCmdDeft1[9] = {0x7E, 0x11, 0x03, 0x04, 0x01, 0x00, 0x17, 0xFF, 0xFF};
const uint8_t FipexCmdDeft2[9] = {0x7E, 0x11, 0x03, 0x05, 0x10, 0x0A, 0x0D, 0xFF, 0xFF};
const uint8_t FipexCmdDeft3[9] = {0x7E, 0x11, 0x03, 0x02, 0xC8, 0x00, 0xD8, 0xFF, 0xFF};
const uint8_t FipexCmdDeft4[6] = {0x7E, 0xF0, 0x00, 0x0C, 0x2C, 0x01};
const uint8_t FipexCmdDeft5[6] = {0x7E, 0xFF, 0x00, 0x20, 0xFF, 0xFF};
const uint8_t FipexCmdDeft6[6] = {0x7E, 0x21, 0x00, 0x21, 0xFF, 0xFF};

enum FipexStatus_Enum FipexStatus;



uint8_t FipexAckWaitFlg = 0;
uint8_t FipexPowerOnFlg = 0;
uint8_t ScriptAct = 0;
uint8_t CurrentScript = 0;  
int FipexCurTime = 0;
int DelayTime = 0;




void FipexScriptStop(void)
{
	CurrentScript = 0;  
	ScriptAct = 0;
	FipexCurTime = 0;
	DelayTime = 0;
}
void FipexScriptStart(void)
{
	FipexStatus = PowerOn;
}

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
	
	FipexOperationPara.FipexCmdInfo[index].CmdDelay = 10;
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

	FipexOperationPara.StartTime = 0x1A56BFC0;
	FipexOperationPara.RepeatTime = 30;  
	FipexOperationPara.CmdCnt = 6;
	
	//FipexCmdStore();
}

uint8_t FipexAckHandle(uint8_t rx_data)
{
	//FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[1];
	//rx_data[1];
	//rx_data[2];
	//rx_data[3];
	//rx_data[XOR];
	
}


/*
 * Fipex协议包格式
 * 字符   1  2  3  4  5  6  7  8  9  10  11  12  13  14
 *       长度
 */


uint8_t FipexInfoCheck(unsigned char* cmd)
{
	unsigned char *ptr_end_temp;
	unsigned char *ptr_temp;
	unsigned char checksum_temp;
	short length = (short)cmd[0];
	unsigned char cmd_cnt = cmd[7];  //指令数
	
	ptr_temp = cmd + 8;
	ptr_end_temp = ptr_temp + length - 1;

	while(cmd_cnt)  //
	{
		if(cmd_cnt == 1)
		{
			if(length != 4)
			{
				return 1;
			}
			
			if(*(ptr_temp + 1) != 0xFF || *(ptr_temp + 2) != 0x01 || *(ptr_temp + 3) != 0xFE)
			{
				if(*(ptr_end_temp) != 0xFE || *(ptr_end_temp - 1) != 0x01 || *(ptr_end_temp - 2) !=0xFF)
				{
					return 1;
				}
			}
			else
			{
				cmd_cnt--;
			}

		}
		else
		{
			if(*(ptr_temp + ptr_temp[2] + 6) == 0x7E)
			{
				bsp_FipexGetCheckSum(ptr_temp, &checksum_temp);

				if(checksum_temp != *(ptr_temp + ptr_temp[2] + 3))
					return 1;

				length -= (ptr_temp[2] + 6);
				if(length < 0)
				{
					return 1;
				}
		
				ptr_temp += (ptr_temp[2] + 6);
				cmd_cnt--;
			}
			else
			{
				return 1; //同步字指令长度出错
			}
		}
	}

	printf("check successful\r\n");
	return 0;
}


uint8_t FipexInfoGet(uint8_t* cmd)
{
	unsigned char i;
	short length = (short)cmd[0]; //指令长度
	unsigned char cmd_cnt = 0;
	unsigned char *cmd_ptr;
	unsigned char *delay_ptr;
	
/*******************************************************/
	printf("Command Frame Check!!!\r\n");
	if(FipexInfoCheck(cmd))
		return 1;

	//Mem_Set((unsigned char *)&FipexOperationPara, 0x00, sizeof(FipexOperationPara));
	memset((unsigned char *)&FipexOperationPara, 0x00, sizeof(FipexOperationPara));
/*******************************************************/	
	for(i=4; i>0; i--)
	{
		FipexOperationPara.StartTime <<= 8;
		FipexOperationPara.StartTime |= cmd[i];
	}
	length -= 4;
/*******************************************************/
	for(i=6; i>4; i--)
	{
		FipexOperationPara.RepeatTime <<= 8;
		FipexOperationPara.RepeatTime |= cmd[i];
	}
	length -= 2;
/*******************************************************/	
	cmd_cnt = cmd[7];
	FipexOperationPara.CmdCnt = cmd_cnt;
	
	cmd_ptr = &cmd[8];
	for(i=0; i<cmd_cnt; i++)
	{
		switch(*(cmd_ptr+1))
		{
			case OBC_SU_ON:
			case OBC_SU_OFF:
				delay_ptr = (cmd_ptr+cmd_ptr[2]+4);
				//Mem_Copy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+4);
				memcpy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+4);
				FipexOperationPara.FipexCmdInfo[i].CmdLength = cmd_ptr[2]+4;
				FipexOperationPara.FipexCmdInfo[i].CmdDelay = *(delay_ptr + 1);
				FipexOperationPara.FipexCmdInfo[i].CmdDelay = (FipexOperationPara.FipexCmdInfo[i].CmdDelay << 8) | *(delay_ptr);
				cmd_ptr = cmd_ptr+cmd_ptr[2]+6;
				break;
			case OBC_SU_END:
				memcpy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, 4);
				FipexOperationPara.FipexCmdInfo[i].CmdLength = 4;
				cmd_ptr = cmd_ptr+4;
				break;
			case SU_PING:
			case SU_INIT:
			case SU_ID:
			case SU_RSP:
			case SU_SP:
			case SU_HK:
			case SU_DP:
			case SU_STDBY:
			case SU_SC:
			case SU_SM:
			case SU_CAL:
				memcpy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+6);
				FipexOperationPara.FipexCmdInfo[i].CmdLength = cmd_ptr[2]+6;
				cmd_ptr = cmd_ptr+cmd_ptr[2]+6;
				break;
		}

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
	
	FipexStatus = Stop;  //Fipex状态为初始化状态
	
	
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
		FipexPowerOnFlg = 1;  //FEPIX上电标志位置位
		
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
	
	FipexTimeOut_ms = 0;
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
	FipexTimeOut_ms = 0;
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
		FipexPowerOnFlg = 0;  //FEPIX上电标志位置位
		
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
		length += 2;
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
