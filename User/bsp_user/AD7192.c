#include "includes.h"
uint32_t AD7192Data = 0;
uint32_t Mag_Fault_Counts;
uint8_t Mag_Error_Flag;

/*
*********************************************************************************************************
*
*	模块名称 : AD7192寄存器操作相关函数
*	说    明 : 
        AD7192_SendByte-----------SPI逐字节发送接收
	           - 函数说明：由于主机SPI通信时，在发送和接受时是同时进行的,即发送完了一个字节的数据后，也应当接受到一个字节的数据
	             （1）stm32先等待已发送的数据是否发送完成，如果没有发送完成，并且进入循环200次，则表示发送错误，返回收到的值为0;
	             （2）如果发送完成，stm32从SPI1总线发送TxData
	             （3）stm32再等待接收的数据是否接收完成，如果没有接收完成，并且进入循环200次，则表示接收错误，则返回值0
	             （4）如果接收完成了，则返回STm32读取的最新的数据  
       AD7192_RegEdit------------对AD7192指定下一步读写操作，8位
       WriteToAD7192ViaSPI-------写寄存器操作
       ReadFromAD7192ViaSPI------读寄存器操作

*	修改记录 :
*		版本号  日期           作者    说明
*		v1.0    2014.12.5      刘洋     
*
*	Copyright (C), NJUST
*
*********************************************************************************************************
*/

void bsp_InitSPI2(void)
{
	/*初始化GPIO管脚，建立GPIO的初始化结构*/	 
	GPIO_InitTypeDef GPIO_InitStructure;
	/*初始化SPI管脚，建立SPI的初始化结构*/	 
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE );	
	/* 配置MSK、MISO、MOSI复用功能 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	/*片选置高*/	

	/* 配置MSK、MISO、MOSI引脚功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 先禁止SPI  */
	SPI_Cmd(SPI2, DISABLE);		
	/*配置SPI1的参数SPI的方向、工作模式、数据帧格式、CPOL、CPHA、NSS软件还是硬件、SPI时钟、数据的传输位、以及CRC*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE);				/* 使能SPI  */

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
*	模块名称 : AD7192相关函数
*	说    明 : 包含对AD_7192的所有操作
          Init_AD7192-------------------AD71892初始化
	        AD7192_SwRst------------------软件初始化
        	AD7192_ZeroCalib--------------零位置校准
	        AD7192_FullSCalib-------------满量程校准
          AD7192StartContinuousConvertion---开始连续转换
          AD7192ReadConvertingData----------读取转换结果
*	修改记录 :
*		版本号  日期           作者    说明
*		v1.0    2014.12.5      刘洋     
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
	//软件初始化
	for(i=0; i<5; i++)
	{
	  AD7192_SendByte(0xFF);
	}                              
  AD7192_Delaytime(1);
	//零位置校准
	AD7192_Write(REG_CONF, CHOP_DIS|REF_IN2|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1);
	AD7192_Write(REG_MODE, MODE_INZCL|DAT_STA_EN|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|0x080);
  AD7192_Delaytime(1);
	//满量程校准
	 AD7192_Write(REG_CONF, CHOP_DIS|REF_IN2|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1);
	AD7192_Write(REG_MODE, MODE_INFCL|DAT_STA_EN|INCLK_MCLK2EN|SINC_4|ENPAR_EN|CLK_DIV_2|SINGLECYCLE_DIS|REJ60_DIS|0x080);
	//AD7192连续读取
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
	
  Init_AD7192();	//AD7192初始化和模式配置
	
	Mag_Set(5);
	Get_Mag_Read(MS);
	Mag_Reset(5);
	Get_Mag_Read(MR);
	Mag[0]=((MS[1]-MR[1])/2.0)*100.0;
	Mag[1]=((MS[2]-MR[2])/2.0)*100.0;
	Mag[2]=((MS[3]-MR[3])/2.0)*100.0;
	Mag[3]=(MS[0]+MR[0])/2;       //磁场值读取

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
  }                                   //故障检测
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
