#include <includes.h>

struct FipexOperationParaStr FipexOperationPara;  //8������ָ��


const uint8_t FipexCmdDeft0[6] = {0x7E, 0x0F, 0x00, 0x0B, 0x3C, 0x00};
const uint8_t FipexCmdDeft1[9] = {0x7E, 0x11, 0x03, 0x02, 0x0A, 0x00, 0x1A, 0x01, 0x00};
const uint8_t FipexCmdDeft2[9] = {0x7E, 0x0C, 0x00, 0x0C, 0xFF, 0xFF};
const uint8_t FipexCmdDeft3[9] = {0x7E, 0xF0, 0x00, 0x0C, 0x2C, 0x01};
const uint8_t FipexCmdDeft4[6] = {0x7E, 0xF0, 0x00, 0x0C, 0x2C, 0x01};
const uint8_t FipexCmdDeft5[6] = {0x7E, 0xFF, 0x00, 0x20, 0xFF, 0xFF};
//const uint8_t FipexCmdDeft6[6] = {0x7E, 0x21, 0x00, 0x21, 0xFF, 0xFF};

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
	Mem_Copy(command_temp, cmd, length_temp);  //�����ݿ������ֲ�����
	comWaitTxFifoEmpty(COM3); // �ȴ�����3�������
	
	comSendBuf(COM3, command_temp, length);
	return 0;
}

void FipexCmdRead(void)  //��CPU_INTER��ȡ����
{
	DEBUG_LOG("Upload Fipex Command from CPU internal Flash\r\n");
	
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
			DEBUG_LOG("Internal Flash program error!\r\n");
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
#if SD_FILESYSTEM_ENABLE
static unsigned long FipexFileSize = 0;
#endif
void FipexScienceDataStore(uint8_t *rx_data)
{
	#if SD_FILESYSTEM_ENABLE
	uint32_t bw;
	uint8_t ret;
	#endif
	uint8_t i;

	uint8_t length = rx_data[2] + 4 + 24;
	uint8_t *ptr_obc;
	
	ptr_obc = rx_data + length + 5;
	
	*ptr_obc++ = CurDate.RTC_Date;
	*ptr_obc++ = CurTime.RTC_Hours;
	*ptr_obc++ = CurTime.RTC_Minutes;
	*ptr_obc++ = CurTime.RTC_Seconds;

	for(i=0; i<20; i++)
		*ptr_obc++ = 0xFF;
	
	#if SD_FILESYSTEM_ENABLE
	ret = f_open(&f_file, "0:/fipex.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS); // NOTE:�����ļ������ȫӢ��
	if (ret != 0)
	{
		DEBUG_LOG("0:/fipex.bin open error\r\n");
		return;
	}
	DEBUG_LOG("Fipex store data->");
	printf("0:/fipex.fsize: %ld\r\n", FipexFileSize);
	
	ret = f_lseek(&f_file, FipexFileSize);
	if (ret != 0)
	{
		DEBUG_LOG("0:/fipex.bin lseek error\r\n");
		f_close(&f_file);
		return;
	}
	
	length = rx_data[2] + 4 + 24;
	f_write(&f_file, (rx_data + 1), length, &bw);
	if(length != bw)
	{
		DEBUG_LOG("0:/fipex.bin write error\r\n");
		f_close(&f_file);
		return;
	}
	
	ret = f_close(&f_file);
	if(ret == 0)
		DEBUG_LOG("0:/fipex.bin store ok\r\n");
	
	FipexFileSize += length;
	
	#else
		comSendBuf(COM6, (rx_data + 1), length);
	#endif
}


uint8_t NACK_flag = 0;
extern uint8_t FipexRspSendCnt;
uint8_t FipexAckHandle(uint8_t *rx_data)
{
	uint8_t CheckSumTemp;
	
	if(rx_data[0] != 0x7E)
	{
		if(FipexRspSendCnt == 0)
		{
			FipexRspSendCnt = 1;
			return 1;  //������SU_RSP
		}
		else
		{
			FipexRspSendCnt = 0;
			return 3;
		}
		
	}
	
	bsp_FipexGetCheckSum(rx_data, &CheckSumTemp, 1);  //��ȡУ����
	if(CheckSumTemp != *(rx_data + rx_data[2] + 4))
	{
		if(FipexRspSendCnt == 0)
		{
			FipexRspSendCnt = 1;
			return 1;  //������SU_RSP
		}
		else
		{
			FipexRspSendCnt = 0;
			return 3;
		}
		
	}
	
	/****************Response Handling****************/

	switch(rx_data[1]) 
	{
		case	SU_R_ACK:
			break;
		case SU_R_ID:
			break;
		case SU_R_NACK:
			if((rx_data[4] == SyncError) || (rx_data[4] == FCSError)) 
			{
					/* was 2nd NACK? */
					if(NACK_flag == 1)
					{
						/* ERROR */
						NACK_flag = 0;
						return 3;
					} 
					else //NACK_flag = 0;
					{
						NACK_flag++;
						/* resend CMD */
						return 2;
					}
			} 
			else 
			{
				/* ERROR */
				return 3;
			}
		default:
			/* Add TIME, ATTITUDE, POSITION, and store */
			FipexScienceDataStore(rx_data);
		return 4;
	}
	
	return 0;
}



/*
 * FipexЭ�����ʽ
 * �ַ�   1  2  3  4  5  6  7  8  9  10  11  12  13  14
 *       ����
 */


uint8_t FipexInfoCheck(unsigned char* cmd)
{
	unsigned char *ptr_end_temp;
	unsigned char *ptr_temp;
	unsigned char checksum_temp;
	short length = (short)cmd[0];
	unsigned char cmd_cnt = cmd[7];  //ָ����
	
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
			
			if(*(ptr_temp + 1) != 0xFF || *(ptr_temp + 2) != 0x01 || *(ptr_temp + 3) != 0xFE)  //ptr_temp ͨ���ۼӵó���
			{
				return 1;
			}
			else  //��ȷ
			{
				if(*(ptr_end_temp) != 0xFE || *(ptr_end_temp - 1) != 0x01 || *(ptr_end_temp - 2) !=0xFF) //ptr_end_temp ͨ��length����ó�
				{
					return 1;
				}
				
				cmd_cnt--;
			}

		}
		else
		{
			if(*(ptr_temp + ptr_temp[2] + 6) == 0x7E)	//ָ���sr��ʼ���ƣ����ξ���cmd-id��len��xor��delay0��������һ��sr����+6����len��Ϊ0����Ҫ��data���ȣ���ptr_temp[2]
			{
				if(IS_OBC_SU_CMD(ptr_temp[1]) || IS_FIPEX_CMD(ptr_temp[1]))
				{
					bsp_FipexGetCheckSum(ptr_temp, &checksum_temp, 0);
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
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
	}

	//printf("check successful\r\n");
	return 0;
}


uint8_t FipexInfoGet(uint8_t* cmd)
{
	unsigned char i;
	short length = (short)cmd[0]; //ָ���
	unsigned char cmd_cnt = 0;
	unsigned char *cmd_ptr;
	unsigned char *delay_ptr;
	
/*******************************************************/
	DEBUG_LOG("Command Frame Check!!!\r\n");
	if(FipexInfoCheck(cmd))
		return 1;

	Mem_Set((unsigned char *)&FipexOperationPara, 0x00, sizeof(FipexOperationPara));
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
				Mem_Copy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+4);

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
	

void FipexInfomationInit(void)  //��FLASH�ж�ȡָ����Ϣ
{

	DEBUG_LOG("Read Fipex Command from CPU Flash\r\n");
	
	bsp_ReadCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara));
	
	FipexStatus = Stop;  //Fipex״̬Ϊ��ʼ��״̬
	
}


void bsp_FipexSwitchInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_FIPEX_3V3_PORT | RCC_FIPEX_5V_PORT, ENABLE);

	/* ����GPIO */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_3V3_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_3V3_EN_PORT, &GPIO_InitStructure);
	
	FIPEX_3V3_DISABLE;  //����3.3V����
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_5V_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_5V_EN_PORT, &GPIO_InitStructure);
	FIPEX_5V_DISABLE;   //����5V����
	
}

uint8_t bsp_FipexPowerOn(void)
{
  uint32_t FipexTimeOut_ms;
	
	FIPEX_3V3_ENABLE;//Fipex 3.3����Դ�ϵ�
	
	FipexTimeOut_ms = 0;
	while((FIPEX_3V3_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//����ϵ������Ƿ�����(ʱ������300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//�����ʱtodo
	{
		FIPEX_3V3_DISABLE;
		
		DEBUG_LOG("Fipex 3v3 power on fail!\r\n");
		
		return 1;  //3.3V�ϵ����
	}
	
	
	BSP_OS_TimeDlyMs(10);  //�ϵ�֮���ӳ�
	
	FIPEX_5V_ENABLE;	//Fipex5����Դ�ϵ�
	
	FipexTimeOut_ms = 0;
	while((FIPEX_5V_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//����ϵ������Ƿ�����(ʱ������300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//�����ʱtodo
	{
		FIPEX_5V_DISABLE;
		FIPEX_3V3_DISABLE;
		
		DEBUG_LOG("Fipex 5V power on fail!\r\n");
		
		return 2;  //5V�ϵ����
	}
	else
	{
		FipexPowerOnFlg = 1;  //FEPIX�ϵ��־λ��λ
		
		#if debug_enable
		//printf("Fipex power on successfully!\r\n");
		#endif
	}
	BSP_OS_TimeDlyMs(1000);	//�ȴ�1000ms
	
	
	//bsp_FipexSendCmd(FipexCmd, 8);//���ڷ�������ָ��
	return 0;
}

uint8_t bsp_FipexPowerOff(void)
{
	uint32_t FipexTimeOut_ms;
	
	//���ڷ��͹ر�ָ��
	
	//�ظ���ȷ
	
	FIPEX_5V_DISABLE;//�ر�Fipex 5����Դ����
	
	FipexTimeOut_ms = 0;
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//���5�������Ƿ�ر�(ʱ������200ms)
	{
		bsp_DelayUS(1);
	}
	
	if(FipexTimeOut_ms > 290*1000)
	{
		DEBUG_LOG("Fipex power 5V off fail!\r\n");
		return 1;
	}
	
	FIPEX_3V3_DISABLE;//�ر�Fipex3.3������
	FipexTimeOut_ms = 0;
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//���3.3�������Ƿ�ر�(ʱ������200ms)
	{
		bsp_DelayUS(1);
	}

	if(FipexTimeOut_ms > 290*1000)
	{
		DEBUG_LOG("Fipex power 3V3 off fail!\r\n");
		return 2;
	}
	else
	{
		FipexPowerOnFlg = 0;  //FEPIX�ϵ��־λ��λ
		
		#if debug_enable
		//printf("Fipex power off successfully!\r\n");
		#endif
	}
	
	return 0;
}

uint8_t bsp_FipexGetCheckSum(unsigned char *cmd, unsigned char* checksum, unsigned char trans_flag)  //
{
	unsigned char checksum_temp = 0;
	unsigned char length = 0;

	if (*cmd == 0x7E)  //����ĸ�Ƿ����0x7E
	{
		length = cmd[2];  //cmd[1]��cmd[length+3]����У��
		length += 2;
		
		if(trans_flag)
			length++;
		
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
		return 1;  //֡ͷ����0x7E
	}
}
