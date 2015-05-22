#include <includes.h>

#define FIPEX_RX_SIZE 256
#define FIPEX_ACK_TIMEOUT 500



uint8_t FipexRxBuf[FIPEX_RX_SIZE] = {0};
uint8_t FipexRxPtr = 0;
uint8_t FipexRxSyncFlg = 0;
uint8_t FipexRxLength = 0;
uint8_t FipexRspSendCnt = 0;
uint8_t FipexErrorFlg = 0;
uint8_t DataResult;


const uint8_t FipexRspFrame[4] = {0x7E, 0x11, 0x00, 0x11};

void PLOAD_SAM_TASK(void *p_arg)
{
	uint8_t Fipex_data;
	uint16_t FipexAckTimeout;
	(void)p_arg;

	FipexScriptStart();
	while(1)
	{
		switch(FipexStatus)
		{
			case Stop:
				break;
			case PowerOn:  //��ʼ��
				FipexCurTime = 0;
				CurrentScript = 0;
				ScriptAct = 1;
				DelayTime = 1;  //��ʼ�ӳ�Ϊ1ms
				FipexStatus = ScriptRunning;
			case ScriptRunning:
				if(ScriptAct == 1) //�ű�����
				{
					ScriptAct = 0;
					switch (FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[1])
					{
						case OBC_SU_ON:
							if(bsp_FipexPowerOn())
							{
								//�ϵ����
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
								DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_OFF:
							if(bsp_FipexPowerOff())
							{
								//�������
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
							DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_END:
							FipexCurTime = 0;
							DelayTime = FipexOperationPara.RepeatTime * 1000;
							FipexStatus = ScriptSleep; //״̬ת��Ϊ�ű�˯�߲���
							break;
						
						case SU_PING:
						case SU_INIT:
						case SU_SP:	
						case SU_STDBY:
						case SU_SC:
						case SU_SM:
							FipexAckWaitFlg = 1; //����Fipex�ű����ձ�־λ
						case SU_ID:
						case SU_RSP:
						case SU_HK:
						case SU_DP:
						case SU_CAL:
							comClearRxFifo(COM3);  //��մ��ڽ���FIFO
							bsp_FipexSendCmd(&FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[0], FipexOperationPara.FipexCmdInfo[CurrentScript].CmdLength);
							DelayTime = 1;
							break;
					}
				}
				break;
			
				
			case ScriptSleep:	
			break;
			default:
			break;
		}
/*****************************************************************/
		if(FipexAckWaitFlg)  //��ҪSU�ش�ACK
		{
update:			
			FipexAckTimeout = 0;
			FipexRxLength = 0;
			FipexRxSyncFlg = 0;
			while(FipexAckTimeout < FIPEX_ACK_TIMEOUT)  //���մ������ݣ���ʱ����Ϊ5ms��
			{
				if(comGetChar(COM3, &Fipex_data))  //���ڻ�ȡһ������
				{
					FipexAckTimeout = 0; //��ʱ����
					
					if(!FipexRxSyncFlg)  //δͬ��
					{
						if(Fipex_data == 0x7E)
						{
							FipexRxSyncFlg = 1;  
							FipexRxPtr = 0;   //��ͬ���ַ���֡ͷ
							FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						}
					}
					else  //ͬ�������
					{
						FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						
						if(FipexRxPtr == 205)  //�յ�����ź�
						{
							FipexRxSyncFlg = 0;
							DataResult = FipexAckHandle(FipexRxBuf);
							
							if(DataResult == 1) //�������·���
							{
								FipexRxSyncFlg = 0; //ͬ���ַ�
								bsp_FipexSendCmd((uint8_t*)FipexRspFrame, sizeof(FipexRspFrame)); //����SU_RSP֡
								comClearRxFifo(COM3);  //��մ��ڽ���FIFO
								FipexAckTimeout = 0; //��ʱ����
								
								DEBUG_LOG("Fipex: OBC request for repeat!\r\n");
											
								continue;
							}
							else if(DataResult == 2) //����CMD
							{
								FipexRxSyncFlg = 0; //ͬ����
								bsp_FipexSendCmd(&FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[0], FipexOperationPara.FipexCmdInfo[CurrentScript].CmdLength);
								comClearRxFifo(COM3);  //��մ��ڽ���FIFO
								FipexAckTimeout = 0; //��ʱ����
								
								DEBUG_LOG("Fipex: Fipex request for CMD!\r\n");
								
								continue;
							}
							
							else if(DataResult == 3) //����
							{
								FipexAckWaitFlg = 0;  //�ر�Ack���ձ�־λ
								FipexErrorFlg = 1;
								goto error;
							}
							
							else if(DataResult == 4) //���յ�����֡
							{
								goto update;
							}
							
							else //������ȷ
							{
								FipexAckWaitFlg = 0;
								goto next;
							}
						} //if һ֡�������
					} //else ͬ�������
				}
	
					FipexAckTimeout++;
					//bsp_DelayUS(1000);
					BSP_OS_TimeDlyMs(1);
			}
			
			/* ����Fipex����ѭ�� */
			if(FipexAckTimeout == FIPEX_ACK_TIMEOUT)  // ������ʱ
			{
				DEBUG_LOG("Fipex: Fipex ACK Timeout!\r\n");
				
				if(FipexRspSendCnt == 0)
				{
					FipexRxSyncFlg = 0; //ͬ���ַ�
					bsp_FipexSendCmd((uint8_t *)FipexRspFrame, sizeof(FipexRspFrame)); //����SU_RSP֡
					comClearRxFifo(COM3);  //��մ��ڽ���FIFO
					FipexRspSendCnt = 1; //SU_RSP����	
				}
				else
				{
					FipexAckWaitFlg = 0;  //�ر�Ack���ձ�־λ
					FipexErrorFlg = 1;
					goto error;
					/* ������ */
				}
			}

			

		}//if(FipexAckWaitFlg)
		
		else
		{
			if(comGetChar(COM3, &Fipex_data))
			{
				FipexAckTimeout = 0; //��ʱ����
					
					if(!FipexRxSyncFlg)  //δͬ��
					{
						if(Fipex_data == 0x7E)
						{
							FipexRxSyncFlg = 1;  
							FipexRxPtr = 0;   //��ͬ���ַ���֡ͷ
							FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						}
					}
					else  //ͬ�������
					{
						FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						
						if(FipexRxPtr == 205)  //�յ�����ź�
						{
							FipexRxSyncFlg = 0;
							DataResult = FipexAckHandle(FipexRxBuf);
						}
					}
			}
		}
		
error:  if(FipexErrorFlg)
				{
					FipexAckWaitFlg = 0;
					FipexErrorFlg = 0;
					
					DEBUG_LOG("Fipex: Fipex Error!\r\n");
				}	
				
next:		if(FipexCurTime++ > DelayTime) //Fipexʱ�䵽���ӳ�ʱ�� //��ʱ����
				{
					switch(FipexStatus)
					{
						case Stop:
						case PowerOn:  
							break;
						case ScriptRunning:
							if(CurrentScript == FipexOperationPara.CmdCnt - 1)  //�ű�����������󣬽���ű�˯�߽׶�
							{
								FipexCurTime = 0;
								DelayTime = FipexOperationPara.RepeatTime * 1000;
								FipexStatus = ScriptSleep; //״̬ת��Ϊ�ű�˯�߲���
							}
							else  //�ű������л�
							{
								FipexCurTime = 0;
								CurrentScript++;
								ScriptAct = 1;  //�ű�������־λ��λ
							}
							break;
							
						case ScriptSleep:
							FipexStatus = PowerOn;
						
						#if debug_enable
							//printf("Fipex: OBC for next script loop!\r\n");
						#endif 
						
							break;
					}
			}
		BSP_OS_TimeDlyMs(1);
	}
}

void PLOAD_STO_TASK(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}
