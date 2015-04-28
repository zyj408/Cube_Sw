#include <includes.h>

#define FIPEX_RX_SIZE 256

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
	uint8_t FipexAckTimeout;
	(void)p_arg;

	FipexScriptStart();
	while(1)
	{
		switch(FipexStatus)
		{
			case Stop:
				break;
			case PowerOn:  //初始化
				FipexCurTime = 0;
				CurrentScript = 0;
				ScriptAct = 1;
				DelayTime = 1;  //开始延迟为1ms
				FipexStatus = ScriptRunning;
			case ScriptRunning:
				if(ScriptAct == 1) //脚本动作
				{
					ScriptAct = 0;
					switch (FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[1])
					{
						case OBC_SU_ON:
							if(bsp_FipexPowerOn())
							{
								//上电错误
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
								DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_OFF:
							if(bsp_FipexPowerOff())
							{
								//掉电错误
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
							DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_END:
							FipexCurTime = 0;
							DelayTime = FipexOperationPara.RepeatTime * 1000;
							FipexStatus = ScriptSleep; //状态转换为脚本睡眠操作
							break;
						
						case SU_PING:
						case SU_INIT:
						case SU_SP:	
						case SU_STDBY:
						case SU_SC:
						case SU_SM:
							FipexAckWaitFlg = 1; //清零Fipex脚本接收标志位
						case SU_ID:
						case SU_RSP:
						case SU_HK:
						case SU_DP:
						case SU_CAL:
							comClearRxFifo(COM3);  //清空串口接收FIFO
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
		if(FipexAckWaitFlg)  //需要SU回传ACK
		{
update:			
			FipexAckTimeout = 0;
			FipexRxLength = 0;
			FipexRxSyncFlg = 0;
			while(FipexAckTimeout < 50)  //接收串口数据（超时设置为5ms）
			{
				if(comGetChar(COM3, &Fipex_data))  //串口获取一个数据
				{
					FipexAckTimeout = 0; //计时清零
					
					if(!FipexRxSyncFlg)  //未同步
					{
						if(Fipex_data == 0x7E)
						{
							FipexRxSyncFlg = 1;  
							FipexRxPtr = 0;   //将同步字放入帧头
							FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						}
					}
					else  //同步情况下
					{
						FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						
						if(FipexRxPtr == 205)  //收到检测信号
						{
							FipexRxSyncFlg = 0;
							DataResult = FipexAckHandle(FipexRxBuf);
							
							if(DataResult == 1) //请求重新发送
							{
								FipexRxSyncFlg = 0; //同步字符
								bsp_FipexSendCmd((uint8_t*)FipexRspFrame, sizeof(FipexRspFrame)); //发送SU_RSP帧
								comClearRxFifo(COM3);  //清空串口接收FIFO
								FipexAckTimeout = 0; //计时清零
								
								DEBUG_LOG("Fipex: OBC request for repeat!\r\n");
											
								continue;
							}
							else if(DataResult == 2) //发送CMD
							{
								FipexRxSyncFlg = 0; //同步字
								bsp_FipexSendCmd(&FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[0], FipexOperationPara.FipexCmdInfo[CurrentScript].CmdLength);
								comClearRxFifo(COM3);  //清空串口接收FIFO
								FipexAckTimeout = 0; //计时清零
								
								DEBUG_LOG("Fipex: Fipex request for CMD!\r\n");
								
								continue;
							}
							
							else if(DataResult == 3) //错误
							{
								FipexAckWaitFlg = 0;  //关闭Ack接收标志位
								FipexErrorFlg = 1;
								goto error;
							}
							
							else if(DataResult == 4) //接收到数据帧
							{
								goto update;
							}
							
							else //解析正确
							{
								FipexAckWaitFlg = 0;
								goto next;
							}
						} //if 一帧接收完成
					} //else 同步情况下
				}
	
					FipexAckTimeout++;
					//bsp_DelayUS(1000);
					BSP_OS_TimeDlyMs(1);
			}
			
			/* 跳出Fipex接收循环 */
			if(FipexAckTimeout == 50)  // 发生超时
			{
				DEBUG_LOG("Fipex: Fipex ACK Timeout!\r\n");
				
				if(FipexRspSendCnt == 0)
				{
					FipexRxSyncFlg = 0; //同步字符
					bsp_FipexSendCmd((uint8_t *)FipexRspFrame, sizeof(FipexRspFrame)); //发送SU_RSP帧
					comClearRxFifo(COM3);  //清空串口接收FIFO
					FipexRspSendCnt = 1; //SU_RSP发送	
				}
				else
				{
					FipexAckWaitFlg = 0;  //关闭Ack接收标志位
					FipexErrorFlg = 1;
					goto error;
					/* 错误处理 */
				}
			}

			

		}//if(FipexAckWaitFlg)
		
		else
		{
			if(comGetChar(COM3, &Fipex_data))
			{
				FipexAckTimeout = 0; //计时清零
					
					if(!FipexRxSyncFlg)  //未同步
					{
						if(Fipex_data == 0x7E)
						{
							FipexRxSyncFlg = 1;  
							FipexRxPtr = 0;   //将同步字放入帧头
							FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						}
					}
					else  //同步情况下
					{
						FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						
						if(FipexRxPtr == 205)  //收到检测信号
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
				
next:		if(FipexCurTime++ > DelayTime) //Fipex时间到达延迟时间 //延时计算
				{
					switch(FipexStatus)
					{
						case Stop:
						case PowerOn:  
							break;
						case ScriptRunning:
							if(CurrentScript == FipexOperationPara.CmdCnt - 1)  //脚本操作到达最后，进入脚本睡眠阶段
							{
								FipexCurTime = 0;
								DelayTime = FipexOperationPara.RepeatTime * 1000;
								FipexStatus = ScriptSleep; //状态转换为脚本睡眠操作
							}
							else  //脚本操作切换
							{
								FipexCurTime = 0;
								CurrentScript++;
								ScriptAct = 1;  //脚本动作标志位置位
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
