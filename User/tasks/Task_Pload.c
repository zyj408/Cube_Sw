#include <includes.h>

#define FIPEX_RX_SIZE 256

uint8_t FipexRxBuf[FIPEX_RX_SIZE] = {0};
uint8_t FipexRxPtr = 0;
uint8_t FipexRxSyncFlg = 0;
uint8_t FipexRxLength = 0;


void PLOAD_SAM_TASK(void *p_arg)
{
	uint8_t Fipex_data;
	uint8_t FipexAckTimeout;
	(void)p_arg;

	
	
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
				DelayTime = 0;  //开始延迟为1ms
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
						case SU_ID:
						case SU_RSP:
						case SU_SP:
						case SU_HK:
						case SU_DP:
						case SU_STDBY:
						case SU_SC:
						case SU_SM:
						case SU_CAL:
							FipexRxPtr = 0; //串口2级接收区指针清零
							comClearRxFifo(COM3);  //清空串口接收FIFO
							bsp_FipexSendCmd(&FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[0], FipexOperationPara.FipexCmdInfo[CurrentScript].CmdLength);
							
							FipexAckWaitFlg = 1; //清零Fipex脚本接收标志位
							FipexRxSyncFlg = 0; //COM3接收同步字清零
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
	
		if(FipexAckWaitFlg)  //未收到SU载荷回传的数据
		{
			FipexAckTimeout = 0;
			FipexRxLength = 0;
			while(FipexAckTimeout < 10)  //接收串口数据（超时设置为10ms）
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
						if(FipexRxPtr == 3) //检测数据长度
						{
							FipexRxLength = FipexRxBuf[2]; //将第2个字符串的数据传入FipexRxLength
						}
						
					}
				}
				else
				{
					FipexAckTimeout++;
				}
				
				BSP_OS_TimeDlyMs(1);
			}
			
			if(FipexAckTimeout == 10)
			{
				//错误
			}
			else
			{
				FipexAckHandle(FipexRxBuf);
			}

		}
		
		else  //获取到SU载荷回传的数据
		{
			if(FipexCurTime++ > DelayTime) //Fipex时间到达延迟时间
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
						break;
				}
				
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
