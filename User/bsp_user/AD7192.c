#include "includes.h"
uint32_t AD7192Data = 0;
uint32_t Mag_Fault_Counts;
uint8_t Mag_Error_Flag;

/*
*********************************************************************************************************
*
*	ģ������ : AD7192�Ĵ���������غ���
*	˵    �� : 
        AD7192_SendByte-----------SPI���ֽڷ��ͽ���
	           - ����˵������������SPIͨ��ʱ���ڷ��ͺͽ���ʱ��ͬʱ���е�,����������һ���ֽڵ����ݺ�ҲӦ�����ܵ�һ���ֽڵ�����
	             ��1��stm32�ȵȴ��ѷ��͵������Ƿ�����ɣ����û�з�����ɣ����ҽ���ѭ��200�Σ����ʾ���ʹ��󣬷����յ���ֵΪ0;
	             ��2�����������ɣ�stm32��SPI1���߷���TxData
	             ��3��stm32�ٵȴ����յ������Ƿ������ɣ����û�н�����ɣ����ҽ���ѭ��200�Σ����ʾ���մ����򷵻�ֵ0
	             ��4�������������ˣ��򷵻�STm32��ȡ�����µ�����  
       AD7192_RegEdit------------��AD7192ָ����һ����д������8λ
       WriteToAD7192ViaSPI-------д�Ĵ�������
       ReadFromAD7192ViaSPI------���Ĵ�������

*	�޸ļ�¼ :
*		�汾��  ����           ����    ˵��
*		v1.0    2014.12.5      ����     
*
*	Copyright (C), NJUST
*
*********************************************************************************************************
*/

void bsp_InitSPI2(void)
{
	/*��ʼ��GPIO�ܽţ�����GPIO�ĳ�ʼ���ṹ*/	 
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��ʼ��SPI�ܽţ�����SPI�ĳ�ʼ���ṹ*/	 
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE );	
	/* ����MSK��MISO��MOSI���ù��� */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	/*Ƭѡ�ø�*/	

	/* ����MSK��MISO��MOSI���Ź��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* �Ƚ�ֹSPI  */
	SPI_Cmd(SPI2, DISABLE);		
	/*����SPI1�Ĳ���SPI�ķ��򡢹���ģʽ������֡��ʽ��CPOL��CPHA��NSS�������Ӳ����SPIʱ�ӡ����ݵĴ���λ���Լ�CRC*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE);				/* ʹ��SPI  */

	Init_GPIO();
}

void Init_GPIO(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    	
	GPIO_InitStructure.GPIO_Pin = AD7192_SET_PIN; 
  GPIO_Init(AD7192_SET_GPIO, &GPIO_InitStructure);              //SET PIN
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    	
	GPIO_InitStructure.GPIO_Pin = AD7192_RESET_PIN; 
  GPIO_Init(AD7192_RESET_GPIO, &GPIO_InitStructure);           //RESET PIN
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);                       //CS PIN
	
	AD7192_SET_LOW();
	AD7192_RESET_HIGH();	
}	


static void AD7192_Delaytime(u16 delayime)
{
	u8 ms = 0;
	while (ms<200)
	{
		ms++;
	}
	
}

u8 AD7192_SendByte(u8 byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};
	SPI_I2S_SendData(SPI2, byte);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
	return SPI_I2S_ReceiveData(SPI2);
}

void AD7192_RegEdit(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	unsigned char WriteData;
	unsigned char i;
		
	for(i=0; i<NumberOfByte; i++)
 	{
	 	WriteData = *(WriteBuffer + i);
		*(ReadBuffer + i) = AD7192_SendByte(WriteData);
	}  
}

void AD7192_Write(u8 Register_Slected,u32 Data)
{
	 AD7192_SendByte(Register_Slected<<3);
	 AD7192_SendByte(Data>>16);
	 AD7192_SendByte(Data>>8);
	 AD7192_SendByte(Data);	  	
}


u32 AD7192_Read(u8 Register_Slected)
{
	u32 Data;
	u8 DataBuffer[4];
	
	AD7192_SendByte(WEN|RW_R|(Register_Slected<<3)|CREAD_DIS);


	switch(Register_Slected)
		{
			case REG_ID:			
			case REG_COM_STA: 			
			case REG_GPOCON:
				DataBuffer[0]=AD7192_SendByte(0x00);				
				break;			
			case REG_MODE: 			 
			case REG_CONF: 			  
			case REG_OFFSET:			
			case REG_FS: 					
			case REG_DATA: 				
	      DataBuffer[0]=AD7192_SendByte(0x00);				
			  DataBuffer[1]=AD7192_SendByte(0x00);	
			  DataBuffer[2]=AD7192_SendByte(0x00);	
				break;
			default: 
					break;
		}
		Data=(DataBuffer[0]<<16) + (DataBuffer[1]<<8) +  DataBuffer[2];
		return Data;
}



/*
*********************************************************************************************************
*
*	ģ������ : AD7192��غ���
*	˵    �� : ������AD_7192�����в���
          Init_AD7192-------------------AD71892��ʼ��
	        AD7192_SwRst------------------�����ʼ��
        	AD7192_ZeroCalib--------------��λ��У׼
	        AD7192_FullSCalib-------------������У׼
          AD7192StartContinuousConvertion---��ʼ����ת��
          AD7192ReadConvertingData----------��ȡת�����
*	�޸ļ�¼ :
*		�汾��  ����           ����    ˵��
*		v1.0    2014.12.5      ����     
*
*	Copyright (C), NJUST
*
*********************************************************************************************************
*/
 
void Init_AD7192(void)
{
	u8 i;
  AD7192_CS_HIGH();
  AD7192_Delaytime(1);
  AD7192_CS_LOW();	
  AD7192_Delaytime(1);
	//�����ʼ��
	for(i=0; i<5; i++)
	{
	  AD7192_SendByte(0xFF);
	}                              
  AD7192_Delaytime(1);
	//��λ��У׼
	AD7192_Write(REG_CONF, CHOP_DIS|REF_IN2|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1);
	AD7192_Write(REG_MODE, MODE_INZCL|DAT_STA_EN|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|0x080);
  AD7192_Delaytime(1);
	//������У׼
	 AD7192_Write(REG_CONF, CHOP_DIS|REF_IN2|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1);
	AD7192_Write(REG_MODE, MODE_INFCL|DAT_STA_EN|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_2|SINGLECYCLE_DIS|REJ60_DIS|0x080);
	//AD7192������ȡ
	AD7192StartContinuousConvertion();
}
 
 
void AD7192StartContinuousConvertion()
{
   AD7192_Write(REG_MODE,0x0C0408);
	 AD7192_Write(REG_CONF, 0x80F058);
}



u32 AD7192ReadConvertingData(void)
{
  u32 Data;
	u8 DataBuffer[3];
	u32 wait;
	wait=0;
  do
  {
 		wait++;
		if(wait>2000000)
		{
			Mag_Error_Flag=1;
			Init_AD7192();
	    wait=0;			
			break;
    }
  }
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14));	


	AD7192_SendByte(WEN|RW_R|(REG_DATA<<3)|CREAD_DIS);		

  DataBuffer[0]=AD7192_SendByte(0x00);				
  DataBuffer[1]=AD7192_SendByte(0x00);	
  DataBuffer[2]=AD7192_SendByte(0x00);	

	Data=(DataBuffer[0]<<16) + (DataBuffer[1]<<8) +  DataBuffer[2];
	return Data;
}


void Mag_Set(int delaytime)
{
	AD7192_RESET_LOW(); 
   AD7192_Delaytime(delaytime);
	AD7192_SET_HIGH();  

}

void Mag_Reset(int delaytime)
{
  AD7192_SET_LOW();  
AD7192_Delaytime(delaytime);
	AD7192_RESET_HIGH(); 
}

 
 
 
void Get_Mag_Read(double* M)
{
	int CH=0;
	double vin;	
  while(CH < 4)
	{	
	   	AD7192Data = AD7192ReadConvertingData();
			vin = AD7192Data*5.0/16777215.00000000;	
		  if(CH==0)
	  	{
				M[0]=vin;
	  	}
	   	else if(CH==1)
	  	{
		  	vin=(vin-M[0])*1000;
				M[1]=vin;
		  }
	  	else if(CH==2)
	  	{

			  vin=(vin-M[0])*1000;
				M[2]=vin;
		  }
	  	else if(CH==3)
		  {
				vin=(vin-M[0])*1000;
				M[3]=vin;
		  }
		CH=CH+1;	
	}
  CH=0;
}

void Get_Mag_Result(double* Mag)
{
	double MS[4];
	double MR[4];
	
  Init_AD7192();	//AD7192��ʼ����ģʽ����
	
	Mag_Set(5);
	Get_Mag_Read(MS);
	Mag_Reset(5);
	Get_Mag_Read(MR);
	Mag[0]=((MS[1]-MR[1])/2.0)*100.0;
	Mag[1]=((MS[2]-MR[2])/2.0)*100.0;
	Mag[2]=((MS[3]-MR[3])/2.0)*100.0;
	Mag[3]=(MS[0]+MR[0])/2;       //�ų�ֵ��ȡ

	if(Mag[3] < AD7192_REF_TEST_DOWN||Mag[3] > AD7192_REF_TEST_UP)
		{
     	AD7192_CS_HIGH();
     AD7192_Delaytime(200); 
      AD7192_CS_LOW();	
      Init_AD7192();
			Mag_Fault_Counts++;
     }
	if(Mag_Error_Flag==1)
	{
		Mag[0]=-1;
	  Mag[1]=-1;
	  Mag[2]=-1;
	  Mag[3]=-1;
		Mag_Error_Flag=0;
  }                                   //���ϼ��
	AD7192_CS_HIGH();
}
	
	
double AD7192ReadTemperature()
{
	float temp = 0.0;
	
	AD7192_Write(REG_MODE,MODE_SING|DAT_STA_DIS|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|0x080);
	AD7192_Write(REG_CONF,MODE_SING|CHOP_DIS|REF_IN1|TEMP|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1);
	AD7192Data = AD7192ReadConvertingData();	
	temp = (AD7192Data-0x800000)/2815.0-273.0;
	return temp;
}
