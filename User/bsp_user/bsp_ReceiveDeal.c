#include <includes.h>
#include "globalavr.h"

enum INS_STATUS InsState=INS_IDLE;  /* 地面测试状态初始化 */

uint16_t InsRxCmdCnt = 0;
uint8_t InsRxIndex;
uint8_t InsRxLength;
uint8_t InsRcvErr=0;
uint8_t GT_ConnStat=0;  /* 没有上位机链接 */

CPU_INT08U TestRcv(unsigned char rev_data)
{
	OS_ERR err;
	
	switch(InsState)
	{
			case INS_IDLE:  //空闲状态
				if(rev_data == 0XA5)
				{
					InsRxIndex = 0;
					InsRxLength = 0;
					InsState = INS_SNYC;
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);   
				}
				else
				{
					InsRxIndex = 0;
					InsRxLength = 0;
					OSTmrStop((OS_TMR *) &COM_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
				}
			break;
				
			case INS_SNYC:  //同步状态
				if(rev_data == 0X5A)
				{
					InsState = INS_RCV;
					InsRxIndex = 0;  /* 接收数据索引清零 */
					InsRxLength = 0;
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);
				}					
			break;
				
			case INS_RCV:  //接收状态
				InsBuf[InsRxIndex] = rev_data;  //0:指令 1:长度 2:数据 3:XOR
				
				if(InsRxIndex == 1)  //获取数据域长度
				{
					InsRxLength = rev_data;
				}
				
				if((InsRxLength + 2) == InsRxIndex)
				{
				
					//Mem_Copy(InsBuf, ID_CommandBuf[ID_CommandCnt], InsRxIndex);
					//ID_CommandCnt++;
				
					
					InsDecode(InsBuf);
					
					InsState = INS_IDLE;
					OSTmrStop((OS_TMR *) &COM_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
				}
				else
				{
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);   
				}
				
				InsRxIndex++;
			break;
				
			default:
				InsRcvErr |= 0x80;
			break;
	}
	return 0;
}



void ComOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	InsRxIndex = 0; 
	InsState = INS_IDLE;
	switch(InsState)
	{
			case INS_IDLE:
			{
				InsRcvErr |= 0x40;
			}break;
			case INS_SNYC:
			{
				InsRcvErr |= 0x20;
			}break;
			case INS_RCV:
			{
				InsRcvErr |= 0x10;
			}break;
			default:
				break;
	}
}

void InsGetCheckSum(uint8_t *Ptr, uint8_t buffsize, uint8_t *checksum)
{
	uint8_t checksumtemp;
	while(buffsize)
	{
		checksumtemp ^= Ptr[buffsize];
		buffsize--;
	}
	*checksum = checksumtemp;
}


void InsSendAck(void)
{
	uint8_t ins_data_temp[10] = {0};
	uint8_t ins_checksum;
	
	ins_data_temp[0] = 0xA5;
  ins_data_temp[1] = 0x5A;
	ins_data_temp[2] = 0x30; //Ack指令码
	ins_data_temp[3] = 0x02; //长度
  ins_data_temp[4] = (uint8_t)(InsRxCmdCnt & 0x00FF);  //数据域
	ins_data_temp[5] = (uint8_t)((InsRxCmdCnt >> 8) & 0x00FF); //数据域

	InsGetCheckSum(&ins_data_temp[4], ins_data_temp[3], &ins_checksum);
	ins_data_temp[6] = ins_checksum;
	comSendBuf(COM6, ins_data_temp, 7);
}

CPU_INT08U InsDecode(uint8_t *InsBuf)
{
	/* 添加校验 */
	
	
	switch(*InsBuf)
	{
		/* 测试指令 */
		case INS_CONN_TST:  //通信测试指令
			GT_ConnStat = 1; //上位机连接
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
			break;
		case INS_COMM_SWITCH_CLR:  //通信体制16小时重置指令
		{
			
			//UartSend(USART1,OBC_ACK);
			break;
		}
		/* 下行数据指令 */	
		case INS_DOWN_TEL:
		{
			//UartSend(USART1,INS_DOWN_TEL);
			break;
		}
		case INS_DOWN_PLD:
		{
			//UartSend(USART1,INS_DOWN_PLD);
			break;
		}			
		case INS_DOWN_CMD:
		{
			//UartSend(USART1,INS_DOWN_CMD);
			break;
		}					
		/* 开关指令 */
		case INS_GPS_A_ON:
		{
			//UartSend(USART1,INS_GPS_A_ON);
			break;
		}		
		
		default:
		{
			//UartSend(USART1,0xFF);
			break;
		}
	}
	
	return 0;
}


void GndTsRxHandle(void)
{
	uint8_t response;
	
	if(comGetChar(COM6, &response)) //获取一个字符
	{
		TestRcv(response);
	}
	
}
