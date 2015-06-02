#include <includes.h>
#include "globalavr.h"


#define SWITCH_TIMEOUT_MS  5
enum INS_STATUS InsState=INS_IDLE;  /* 地面测试状态初始化 */




OS_ERR   err;
uint16_t InsRxCmdCnt = 0;
uint8_t InsRxIndex;
uint8_t InsRxLength;
uint8_t InsRcvErr=0;
uint8_t GT_ConnStat=0;  /* 没有上位机链接 */
uint8_t InsCheckSumError = 0;

CPU_INT08U TestRcv(unsigned char rev_data)
{
	OS_ERR err;
	
	switch(InsState)
	{
			case INS_IDLE:  //空闲状态
				if(rev_data == 0XEB)
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
				if(rev_data == 0X50)
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
	uint8_t checksumtemp = 0;
	while(buffsize)
	{
		buffsize--;
		checksumtemp ^= Ptr[buffsize];
		
	}
	*checksum = checksumtemp;
}


void InsSendAck(void)
{
	uint8_t ins_data_temp[20] = {0};
	uint8_t ins_checksum;
	
	ins_data_temp[0] = 0xEB;
	ins_data_temp[1] = 0x50;
	ins_data_temp[2] = 0x30; //Ack指令码
	ins_data_temp[3] = 0x02; //长度
	ins_data_temp[4] = (uint8_t)(InsRxCmdCnt & 0x00FF);  //数据域
	ins_data_temp[5] = (uint8_t)((InsRxCmdCnt >> 8) & 0x00FF); //数据域

	InsGetCheckSum(&ins_data_temp[4], ins_data_temp[3], &ins_checksum);
	ins_data_temp[6] = ins_checksum;
	comSendBuf(COM1, ins_data_temp, 7);
}


void InsSendHouseKeepingData(void)
{
	uint8_t *p, *q;
	uint8_t ins_checksum;
	uint8_t ins_data_temp[240];
	
	p = ins_data_temp;
	q = p+3;
	*p++ = 0xEB;
	*p++ = 0x50;
	*p++ = 0x31; //Ack指令码
	*p++ = 0x06; //长度
	*p++ = (uint8_t)(InsRxCmdCnt & 0x00FF);  //数据域
	*p++ = (uint8_t)((InsRxCmdCnt >> 8) & 0x00FF); //数据域
	*p++ = CurDate.RTC_Date;
	*p++ = CurTime.RTC_Hours;
	*p++ = CurTime.RTC_Minutes;
	*p++ = CurTime.RTC_Seconds;
	
	*p++ = (uint8_t)(TelCurPtr & 0xFF); 
	*p++ = (uint8_t)((TelCurPtr >> 8) & 0xFF); 
	*p++ = (uint8_t)((TelCurPtr >> 16) & 0xFF); 
	*p++ = (uint8_t)((TelCurPtr >> 24) & 0xFF); 
	
	*p++ = (uint8_t)(GpsCurPtr & 0xFF); 
	*p++ = (uint8_t)((GpsCurPtr >> 8) & 0xFF); 
	*p++ = (uint8_t)((GpsCurPtr >> 16) & 0xFF); 
	*p++ = (uint8_t)((GpsCurPtr >> 24) & 0xFF); 	

	*p++ = (uint8_t)((uint16_t)PWM_Feq1 & 0xFF);
	*p++ = (uint8_t)(((uint16_t)PWM_Feq1 >> 8) & 0xFF);

	*p++ = (uint8_t)((uint16_t)PWM_Feq2 & 0xFF);
	*p++ = (uint8_t)(((uint16_t)PWM_Feq2 >> 8) & 0xFF);
	
	*p++ = 0xFF;
	
	Mem_Copy(p, (uint8_t*)(&eps_bat), sizeof(eps_bat));
	p += sizeof(eps_bat);
	*p++ = 0xFF;
	Mem_Copy(p, (uint8_t*)(&eps_state), sizeof(eps_state));
	p += sizeof(eps_state);
	*p++ = 0xFF;
	Mem_Copy(p, (uint8_t*)(&eps_adc_data), sizeof(eps_adc_data));
	p += sizeof(eps_adc_data);
	
	*q = p - q;
	
	InsGetCheckSum(q+1, *q, &ins_checksum);
	*p = ins_checksum;
	comSendBuf(COM1, ins_data_temp, p - q + 4);

}


CPU_INT08U InsDecode(uint8_t *InsBuf)
{
	uint8_t ins_checksum;
	uint8_t timeout_ms;
	int32_t data_inject = 0;
	uint8_t i;
	
	
	
	InsGetCheckSum(&InsBuf[2], InsBuf[1], &ins_checksum);
	if(ins_checksum != InsBuf[(InsBuf[1] + 2)])  //校验码错误
	{
		InsCheckSumError++;
		return 1;
	}

	switch(InsBuf[0])
	{
		/* 测试指令 */
		case INS_CONN_TST:  //通信测试指令
			GT_ConnStat = 1; //上位机连接
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
		case INS_COMM_SWITCH_CLR:  //通信体制16小时重置指令
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
		/* 下行数据指令 */	
		case INS_DOWN_TEL:
			InsRxCmdCnt++;  //指令计数加1
			InsSendHouseKeepingData();
		break;
		case INS_DOWN_PLD:
			InsRxCmdCnt++;  //指令计数加1
		break;		
		case INS_DOWN_CMD:
			InsRxCmdCnt++;  //指令计数加1
		break;				
		case INS_SD_CLR:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;				
		case INS_FLASH_RST:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;				
		case INS_OBC_RST:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;				
		case INS_OBC_REV:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;				
		
		/* 开关指令 */
		case INS_MTQ_ON:  //开磁棒
			timeout_ms = 0; 
			while(SW_MTQ_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_MTQ, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_MTQ_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;
		case INS_MTQ_OFF:  //关磁棒
			timeout_ms = 0; 
			while(SW_MTQ_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_MTQ, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_MTQ_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			
		break;
		case INS_GPS_A_ON:  //开GPS_A
			timeout_ms = 0; 
			while(SW_GPSA_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_GPSA, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_GPSA_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			
		break;		
		case INS_GPS_A_OFF:  //关GPS_A
			timeout_ms = 0; 
			while(SW_GPSA_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_GPSA, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_GPSA_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;	
		case INS_GPS_B_ON:  //开GPS_B
			timeout_ms = 0; 
			while(SW_GPSB_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_GPSB, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_GPSB_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_GPS_B_OFF: //关GPS_B
			timeout_ms = 0; 
			while(SW_GPSB_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_GPSB, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_GPSB_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;			
		case INS_RSV_ON:
			timeout_ms = 0; 
			while(SW_RES_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_RES, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_RES_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_RSV_OFF:
			timeout_ms = 0; 
			while(SW_RES_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_RES, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_RES_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;			
		case INS_MW_A_ON:
			timeout_ms = 0; 
			while(SW_WHEELA_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_WHEELA, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_WHEELA_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_MW_A_OFF:
			timeout_ms = 0; 
			while(SW_WHEELA_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_WHEELA, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_WHEELA_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_MW_B_ON:
			timeout_ms = 0; 
			while(SW_WHEELB_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_WHEELB, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_WHEELB_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_MW_B_OFF:
			timeout_ms = 0; 
			while(SW_WHEELB_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_WHEELB, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_WHEELB_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_SLBRD_ON:
			timeout_ms = 0; 
			while(SW_SOLAR_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_DEPLOY, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_SOLAR_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_SLBRD_OFF:
			timeout_ms = 0; 
			while(SW_SOLAR_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_DEPLOY, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_SOLAR_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_USB_ON:
			timeout_ms = 0; 
			while(SW_USB_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_USB, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_USB_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_USB_OFF:
			timeout_ms = 0; 
			while(SW_USB_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_USB, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_USB_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S1_ON:
			timeout_ms = 0; 
			while(SW_S0_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_HEAT, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S0_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S1_OFF:
			timeout_ms = 0; 
			while(SW_S0_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_HEAT, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S0_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S2_ON:
			timeout_ms = 0; 
			while(SW_S1_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				SW_S1_ENABLE;;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S1_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S2_OFF:
			timeout_ms = 0; 
			while(SW_S1_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				SW_S1_DISABLE;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S1_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S3_ON:
			timeout_ms = 0; 
			while(SW_S2_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_FIPEX5V, ENABLE);
				SW_S2_ENABLE;;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S2_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S3_OFF:
			timeout_ms = 0; 
			while(SW_S2_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_FIPEX5V, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S2_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S4_ON:
			timeout_ms = 0; 
			while(SW_S3_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_FIPEX3V, ENABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S3_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		case INS_S4_OFF:
			timeout_ms = 0; 
			while(SW_S3_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				out_en(OUT_FIPEX3V, DISABLE);
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && SW_S3_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;		
		
		case INS_MTQ1_DIR_POS:
			timeout_ms = 0; 
			while(MTQ_DIR1_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ1_DIR_POS;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR1_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;
		case INS_MTQ1_DIR_NAG:
			timeout_ms = 0; 
			while(MTQ_DIR1_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ1_DIR_NAG;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR1_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
		break;
		case INS_MTQ2_DIR_POS:
			timeout_ms = 0; 
			while(MTQ_DIR2_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ2_DIR_POS;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR2_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}			
		break;
		case INS_MTQ2_DIR_NAG:
			timeout_ms = 0; 
			while(MTQ_DIR2_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ2_DIR_NAG;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR2_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}			
		break;		
		case INS_MTQ3_DIR_POS:
			timeout_ms = 0; 
			while(MTQ_DIR3_PIN() == Bit_RESET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ3_DIR_POS;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR3_PIN() == Bit_RESET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}			
		break;
		case INS_MTQ3_DIR_NAG:
			timeout_ms = 0; 
			while(MTQ_DIR3_PIN() == Bit_SET && timeout_ms < SWITCH_TIMEOUT_MS)
			{
				MTQ3_DIR_NAG;
				BSP_OS_TimeDlyMs(1);
				timeout_ms++;
			}
			if(timeout_ms == SWITCH_TIMEOUT_MS && MTQ_DIR3_PIN() == Bit_SET)
			{
					//错误
			}
			else
			{
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}			
		break;	

		case INS_MTQ1_PWM:
			if(InsBuf[6] <= 100)
			{
				PwmSetDuty_1 = InsBuf[6];
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				//错误
			}
		break;

		case INS_MTQ2_PWM:
			if(InsBuf[6] <= 100)
			{
				PwmSetDuty_2 = InsBuf[6];
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				//错误
			}
		break;

		case INS_MTQ3_PWM:
			if(InsBuf[6] <= 100)
			{
				PwmSetDuty_3 = InsBuf[6];
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				//错误
			}
		break;
			
		case INS_DET:
			//upXwAdcsReDmp = VALID;
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsReDmp = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsReDmp = INVALID;
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;		
		case INS_STA:			//去掉

			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_DUMP_FOEV_DIS:			//去掉
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_SW_MAG_A:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_SW_MAG_B:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
		case INS_SW_1200:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_SW_9600:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_CW_ON:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_COM_TRAN_OFF:
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
		
		/* 数据注入指令 */
		case INS_CTL_P_PRA:
			//upXwAdcsConP
			//upXwAdcsConPFlag = VALID / INVALID
		
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsConPFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsConPFlag = INVALID;
				
				data_inject = 0;
				for(i=7; i<11; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsConP = (double)(data_inject / 100000.0);
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;
		case INS_CTL_D_PRA:
			//upXwAdcsConD
			//upXwAdcsConDFlag = VALID / INVALID
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsConDFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsConDFlag = INVALID;
				
				data_inject = 0;
				for(i=7; i<11; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsConD = (double)(data_inject / 100000.0);
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
			
		break;		
		case INS_ZJD_CTL:
			//upXwAdcsConZ
			//upXwAdcsConZFlag = VALID / INVALID
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsConZFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsConZFlag = INVALID;
				
				data_inject = 0;
				for(i=7; i<11; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsConZ = (double)(data_inject / 100000.0);
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;
		case INS_DMP_FLAG:
			//upXwAdcsDmpFlgFlag = VALID / INVALID
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsDmpFlgFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsDmpFlgFlag = INVALID;
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;		
		case INS_FLT_FLAG:
			//upXwAdcsFltFlgFlag = VALID / INVALID
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsFltFlgFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsFltFlgFlag = INVALID;
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;
		case INS_CTL_FLAG:
			//upXwAdcsCtlFlgFlag = VALID / INVALID
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsCtlFlgFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsCtlFlgFlag = INVALID;
				
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;		
		case INS_CNT_DMP_FLAG:			//去掉
			//cntDmpFlag
		break;
		case INS_CNT_FLT_FLAG:			//去掉
			//cntPitcomFlag
		break;						
		case INS_CNT_CTL_FLAG:			//去掉
			//cntAttStaFlag
		break;						
		case INS_ORB_TLE_FLAG:
			//upXwAdcsTLEFlag =  VALID / INVALID
			//double     upXwAdcsTLEBstar;
      // upXwAdcsTLEEcco;
      // upXwAdcsTLEInclo;
      // upXwAdcsTLEArgpo;
      // upXwAdcsTLEJdsatepoch;
      // upXwAdcsTLEMo;
      // upXwAdcsTLENo;
			// upXwAdcsTLENodeo;
			if(InsBuf[6] == 0xFF || InsBuf[6] == 0x00)
			{
				if(InsBuf[6] == 0xFF)
					upXwAdcsTLEFlag = VALID;
				else if(InsBuf[6] == 0x00)
					upXwAdcsTLEFlag = INVALID;
				
				data_inject = 0;
				for(i=7; i<11; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEBstar = (double)(data_inject / 1000000.0);
				
				data_inject = 0;
				for(i=11; i<15; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEEcco = (double)(data_inject / 1000000.0);

				data_inject = 0;
				for(i=15; i<19; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEInclo = (double)(data_inject / 1000000.0);			
				
				data_inject = 0;
				for(i=19; i<23; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEArgpo = (double)(data_inject / 1000000.0);	
				
				data_inject = 0;
				for(i=23; i<27; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEJdsatepoch = (double)(data_inject / 1000000.0);	
				
				data_inject = 0;
				for(i=27; i<31; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLEMo = (double)(data_inject / 1000000.0);	
				
				data_inject = 0;
				for(i=31; i<35; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLENo = (double)(data_inject / 1000000.0);	

				data_inject = 0;
				for(i=35; i<39; i++)
				{
					data_inject <<=8;
					data_inject |= InsBuf[i];
				}
				upXwAdcsTLENodeo = (double)(data_inject / 1000000.0);	
				
				upXwAdcsTLEFlag = VALID;
				InsRxCmdCnt++;  //指令计数加1
				InsSendAck();
			}
			else
			{
				
			}
		break;
		case INS_ADCS_TIME_IN:
			for(i=6; i<10; i++)
			{
				data_inject <<=8;
				data_inject |= InsBuf[i];
			}
				upDelta_TinSat = (double)(data_inject / 1000000.0);	
				updateTimeFlag = VALID;
				OSTimeSet (0, &err);
			
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;		
		case INS_EQUP_INPUT:
			
		break;
		case INS_TEL_ADRS:
			
		break;						
		case INS_GPS_ADRS:
			
		break;		
		
		
		case INS_TIME_IN:
			
			bsp_RTCSet(InsBuf[6],InsBuf[7],InsBuf[8],InsBuf[9],InsBuf[10],InsBuf[11]); //todo
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
		
		case INS_FIPEX_SCRIPT_IN:
			
			FipexScriptStop();
			FipexInfoGet(&InsBuf[6]);
			FipexScriptStart();  //开始Fipex脚本
			InsRxCmdCnt++;  //指令计数加1
			InsSendAck();
		break;
				
		default:
			//UartSend(USART1,0xFF);
			break;
	}
	
	return 0;
}


void GndTsRxHandle(void)
{
	uint8_t response;
	
	while(comGetChar(COM1, &response)) //获取一个字符
	{
		TestRcv(response);
	}
	
}
