#include <includes.h>
#include "globalavr.h"

/*
*********************************************************************************************************
*
*	模块名称 : AD7192寄存器操作相关函数
*	说    明 : 
        AD5722_SendByte-----------SPI逐字节发送接收
	           - 函数说明：由于主机SPI通信时，在发送和接受时是同时进行的,即发送完了一个字节的数据后，也应当接受到一个字节的数据
	             （1）stm32先等待已发送的数据是否发送完成，如果没有发送完成，并且进入循环200次，则表示发送错误，返回收到的值为0;
	             （2）如果发送完成，stm32从SPI1总线发送TxData
	             （3）stm32再等待接收的数据是否接收完成，如果没有接收完成，并且进入循环200次，则表示接收错误，则返回值0
	             （4）如果接收完成了，则返回STm32读取的最新的数据  
       AD5722_RegEdit------------对AD5722指定下一步读写操作，8位
       AD5722_Write-------写寄存器操作
       AD5722_Read------读寄存器操作

*	修改记录 :
*		版本号  日期           作者    说明
*		v1.0    2014.12.15      刘洋     
*
*	Copyright (C), NJUST
*
*********************************************************************************************************
*/
unsigned char MotorChange(unsigned int val)
{
	long int cstr = 0;	
	CPU_SR_ALLOC();
	if(val > 0xFFFFFF)
	{
		return 1;
	}
	//ConfigAD5754R();
	
	CPU_CRITICAL_ENTER(); 
	
	cstr = DAC_Register | DAC_Channel_A | (long int)val;		//VoutA=4.97v
	WriteToAD5754RViaSpi(&cstr);
	cstr = DAC_Register | DAC_Channel_B | (long int)val;		//VoutB=3.32V
	WriteToAD5754RViaSpi(&cstr);
	cstr = Power_Control_Register | PowerUp_ALL;   
	WriteToAD5754RViaSpi(&cstr);	
	
	CPU_CRITICAL_EXIT();
	
	MotorCurOutput = val;
	return 0;
}

unsigned char AD5722_SendByte(unsigned char byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};
  SPI_I2S_SendData(SPI2, byte);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
  return SPI_I2S_ReceiveData(SPI2);
}

void AD5722_RegEdit(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	unsigned char WriteData;
  unsigned char i;
		
	for(i=0; i<NumberOfByte; i++)
 	{
	 	WriteData = *(WriteBuffer + i);
		*(ReadBuffer + i) = AD5722_SendByte(WriteData);
	}  
}

void AD5722_Write(unsigned char code)
{
	unsigned char WriteBuf[1];
	unsigned char ReadBuf[1];

	GPIO_ResetBits(GPIOB, GPIO_Pin_12);      //修改
  	WriteBuf[0] = code;	 	
    AD5722_RegEdit(WriteBuf, ReadBuf, 1);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);    //修改
}

void WriteToAD5754RViaSpi(long int *RegisterData)
{	
	unsigned char WriteBuf[3];
	unsigned char ReadBuf[3];
	
	WriteBuf[0]= *RegisterData>>16;
	WriteBuf[1]= *RegisterData>>8;
  WriteBuf[2]= *RegisterData;

	
	SET_SYNC();
	__nop();__nop();__nop();
	CLR_SYNC();	 /* 清除SYNC信号 */
	__nop();
	

  AD5722_RegEdit(WriteBuf, ReadBuf, 3);

	__nop();__nop();__nop();
		
//	ValueToWrite <<= 1;	//Rotate data
//	Delay(5);

	SET_SYNC();
  __nop();__nop();__nop();

}

void ConfigAD5754R(void)
{
	int i;
	long int *p;
	long int ins[2] = {0, 0};

	ins[0] = Output_Range_Select_Register | Range2_Select | DAC_Channel_ALL;
	ins[1] = Power_Control_Register | PowerUp_ALL;
	p = ins;

	for(i=0; i<2; i++)
	{ 
		WriteToAD5754RViaSpi(p);
		__nop();__nop();__nop();__nop();__nop();   /* NOTE */
		p++;
	}
}

void bsp_InitSPI2(void)
{
	bsp_InitSPI2_GPIO();
	InitSPI2();
}

void bsp_InitSPI2_GPIO(void)
{
	/*初始化GPIO管脚，建立GPIO的初始化结构*/	 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	/*片选置高*/	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	
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
}
void InitSPI2(void)
{
	/*初始化SPI管脚，建立SPI的初始化结构*/	 
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	
	
	/* 先禁止SPI  */
	SPI_Cmd(SPI2, DISABLE);		
	/*配置SPI1的参数SPI的方向、工作模式、数据帧格式、CPOL、CPHA、NSS软件还是硬件、SPI时钟、数据的传输位、以及CRC*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//选择了串行时钟的稳态:时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SPI2_IRQHandler(void)
{
	OSIntEnter();
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_CRCERR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_CRCERR);
		MotorCommErr++;
	}
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_OVR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_OVR);
		MotorCommErr++;
	}	
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_MODF) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_MODF);
		MotorCommErr++;
	}
	OSIntExit();
}

