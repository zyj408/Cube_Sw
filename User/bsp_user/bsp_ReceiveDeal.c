#include <includes.h>
#include "globalavr.h"

enum TEST_STATUS 
{
	TEST_IDLE,
	TEST_SNYC,
	TEST_RCV,
};


enum TEST_STATUS GT_TestState=TEST_IDLE;  /* 地面测试状态初始化 */
unsigned char GT_TestIndex;
char GT_RcvBuff[GT_RCV_SIZE];
char GT_RcvEndFlag=0;            /* 地面测试结束标志位 */
char GT_RcvErr=0;

CPU_INT08U TestRcv(unsigned char rev_data)
{
	OS_ERR err;
	
	switch(GT_TestState)
	{
			case TEST_IDLE:
			{
				if(rev_data == 0XA5)
				{
					GT_TestIndex = 0;
					GT_TestState = TEST_SNYC;
					OSTmrStart((OS_TMR *) &TEST_OT_TIMER, (OS_ERR *) &err);   
				}
				else
				{
					GT_TestIndex = 0;
					OSTmrStop((OS_TMR *) &TEST_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
				}
			}break;
			case TEST_SNYC:
			{
				if(rev_data == 0X5A)
				GT_TestState = TEST_RCV;
				GT_TestIndex = 0;  /* 接收数据索引清零 */
				OSTmrStart((OS_TMR *) &TEST_OT_TIMER, (OS_ERR *) &err);   
			}break;
			case TEST_RCV:
			{
				GT_RcvBuff[GT_TestIndex++] = rev_data;  /* 将接收的数据拷贝至GT_RcvBuff */
				if(GT_RcvEndFlag == 0) 
				{
					if(rev_data == 0x5A)
					{
						GT_RcvEndFlag = 1;
					}
				} else if(GT_RcvEndFlag == 1)
				{
					if(rev_data == 0xA5)
					{
						Mem_Copy(GT_RcvBuff, ID_CommandBuf, GT_TestIndex);
						ID_CommandCnt++;
						GT_TestIndex = 0; 
						GT_TestState = TEST_IDLE;
					}else
					{
						GT_RcvEndFlag = 0;
						OSTmrStop((OS_TMR *) &TEST_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
					}
				}
				OSTmrStart((OS_TMR *) &TEST_OT_TIMER, (OS_ERR *) &err);   
			}break;
			default:
			{
				GT_RcvErr |= 0x80;
			}break;
	}
	return 0;
}



void TestOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	GT_TestIndex = 0; 
	GT_TestState = TEST_IDLE;
	switch(GT_TestState)
	{
			case TEST_IDLE:
			{
				GT_RcvErr |= 0x40;
			}break;
			case TEST_SNYC:
			{
				GT_RcvErr |= 0x20;
			}break;
			case TEST_RCV:
			{
				GT_RcvErr |= 0x10;
			}break;
			default:
				break;
	}
}

CPU_INT16U GetCheckSum(CPU_INT16U *Ptr, CPU_INT08U BufSize)
{
	CPU_INT32U CheckSumTemp;
	CPU_INT16U index;
	
	CheckSumTemp = 0x00000000;
	
	for(index=0; index<BufSize; index++)
		CheckSumTemp+=*(Ptr+index);
	
	return (CPU_INT16U)CheckSumTemp;
}


CPU_INT08U InsDecode(char *InsBuf, CPU_INT16U BufSize)
{

	
	switch(*InsBuf)
	{
		/* 测试指令 */
		case INS_CONN_TST:
		{
			//UartSend(USART1,INS_CONN_TST);
			break;
		}
		case INS_COMM_SWITCH_CLR:
		{
			//UartSend(USART1,INS_COMM_SWITCH_CLR);
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
		case INS_MAG_ON:
		{
			//UartSend(USART1,INS_MAG_ON);
			break;
		}
		case INS_MAG_OFF:
		{
			//UartSend(USART1,INS_MAG_OFF);
			break;
		}			
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

