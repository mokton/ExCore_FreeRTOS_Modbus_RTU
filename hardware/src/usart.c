
//硬件驱动
#include "usart.h"
#include "delay.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

USART_INFO usartInfo;

/*
************************************************************
*	函数名称：	Usart1_Init
*
*	函数功能：	串口1初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA9		RX-PA10
************************************************************
*/
void Usart1_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							//打开GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);							//打开USART1的时钟
	
	//PA9	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;										//设置为复用模式
	gpioInitStruct.GPIO_Pin = GPIO_Pin_9;											//初始化Pin9
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;									//承载的最大频率
	GPIO_Init(GPIOA, &gpioInitStruct);												//初始化GPIOA
	
	//PA10	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;								//设置为浮空输入
	gpioInitStruct.GPIO_Pin = GPIO_Pin_10;											//初始化Pin10
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;									//承载的最大频率
	GPIO_Init(GPIOA, &gpioInitStruct);												//初始化GPIOA
	
	usartInitStruct.USART_BaudRate = baud;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usartInitStruct.USART_Parity = USART_Parity_No;									//无校验
	usartInitStruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usartInitStruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART1, &usartInitStruct);
	
	USART_Cmd(USART1, ENABLE);														//使能串口
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvicInitStruct.NVIC_IRQChannel = USART1_IRQn;									//usart1中断号
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;										//中断通道使能
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;							//抢占中断优先级(值越小优先级越高)
	nvicInitStruct.NVIC_IRQChannelSubPriority = 2;									//子中断优先级(值越小优先级越高)
	NVIC_Init(&nvicInitStruct);														//初始化NVIC

}

/*
************************************************************
*	函数名称：	Usart2_Init
*
*	函数功能：	串口2初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_2;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	//PA3	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_3;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	usartInitStruct.USART_BaudRate = baud;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usartInitStruct.USART_Parity = USART_Parity_No;									//无校验
	usartInitStruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usartInitStruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &usartInitStruct);
	
	USART_Cmd(USART2, ENABLE);														//使能串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvicInitStruct.NVIC_IRQChannel = USART2_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

}

/*
************************************************************
*	函数名称：	Usart3_Init
*
*	函数功能：	串口3初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PB10		RX-PB11
************************************************************
*/

void Usart3_Init(unsigned int baud)
{   // Input no ok
	GPIO_InitTypeDef gpioInitStruct;    //声明一个结构体变量，用来初始化GPIO 
	USART_InitTypeDef usartInitStruct;    
	NVIC_InitTypeDef nvicInitStruct;        
	//使能串口的RCC始终
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能USART3所在的GPIOB的始终
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);    

	//串口使用的GPIO口配置 
	// Configure USART3 Tx (PB.10) as alternate function push-pull 
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   
	gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);    

	// Configure USART3 Rx (PB.11) as input floating 
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);    

	//配置串口  
	usartInitStruct.USART_BaudRate = baud;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartInitStruct.USART_Parity = USART_Parity_No;
	usartInitStruct.USART_StopBits = USART_StopBits_1;
	usartInitStruct.USART_WordLength = USART_WordLength_8b;

	// Configure USART3     
	USART_Init(USART3, &usartInitStruct);//配置串口3

	// Enable USART3 Receive interrupts 使能串口接收终端 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
	//串口发送中断在发送数据时开启
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	// Enable the USART3     
	USART_Cmd(USART3, ENABLE);//使能串口3 

	//串口中断配置   
	//Configure the NVIC Preemption Priority Bits       
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    

	// Enable the USART3 Interrupt     
	nvicInitStruct.NVIC_IRQChannel = USART3_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct); 
    
	
	// 配置RS485流向控制引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_1;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct); 
	
	RS485_RTMode(0);
}

/*
************************************************************
*	函数名称：	Usart3_RT
*
*	函数功能：	串口3 RS485 读写状态设置
*
*	入口参数：	state：1 发送, 0 接收
*
*	返回参数：	无
************************************************************
*/
void RS485_RTMode(u8 state)
{
	DelayXms(1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, state > 0 ? Bit_SET : Bit_RESET);
	DelayXms(1);
}

/*
************************************************************
*	函数名称：	Usart3_Init
*
*	函数功能：	串口3初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PC10		RX-PC11
************************************************************
*/
/*
void Usart3_Init(unsigned int baud)
{	// Usart3 OK 
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	//USART 相关时钟初始化配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE); 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART 相关 GPIO 初始化配置
	//定义 GPIO 初始化结构体 
	//GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_StructInit(&GPIO_InitStructure); 
	//配置 USART3 的接收端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //PC.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);     //初始化 PC.11 
	//配置 USART3 的发送端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PC.10 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);     //初始化 PC.10 
	
	USART_InitStructure.USART_BaudRate = baud; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	//配置 USART3 的异步双工串行工作模式
	USART_Init(USART3, &USART_InitStructure); 
	//使能 USART3 接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
	//关闭 USART3 发送中断
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
	//使能 USART3 
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);//使用部分映射功能，开启部分映射功能配置
	USART_Cmd(USART3, ENABLE); 
	
	//NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
}
*/

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

void RS485_SendString(unsigned char *str, unsigned short len)
{
	RS485_RTMode(1);
	Usart_SendString(USART3, str, len);
	RS485_RTMode(0);
}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt, ...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}


void RS485Printf(char *fmt, ...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//格式化
	va_end(ap);
	
	RS485_RTMode(1);
	
	while(*pStr != 0)
	{
		USART_SendData(USART3, *pStr++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
	
	RS485_RTMode(0);
}


extern unsigned char usart1Buf[MAXLENGTH];
extern unsigned char usart1Len;
/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//接收中断
	{
		if(usart1Len >= MAXLENGTH)									//防止数据过多，导致内存溢出
			usart1Len = 0;
		usart1Buf[usart1Len++] = USART1->DR;
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

}


/*UASRT2中断转到了ESP8266相关的驱动文件*/
//extern unsigned char usart2Buf[MAXLENGTH];
//extern unsigned char usart2Len;
/*
************************************************************
*	函数名称：	USART2_IRQHandler
*
*	函数功能：	串口2收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
/*
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//接收中断
	{
		if(usart2Len >= MAXLENGTH)									//防止数据过多，导致内存溢出
			usart2Len = 0;
		usart2Buf[usart2Len++] = USART2->DR;
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}
*/

//extern unsigned char usart3Buf[MAXLENGTH];
//extern unsigned char usart3Len;
/*
************************************************************
*	函数名称：	USART3_IRQHandler
*
*	函数功能：	串口3收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/

//void USART3_IRQHandler(void)
//{

////	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//接收中断
////	{
////		if(usart3Len >= MAXLENGTH)									//防止数据过多，导致内存溢出
////			usart3Len = 0;
////		usart3Buf[usart3Len++] = USART3->DR;
////		
////		USART_ClearFlag(USART3, USART_FLAG_RXNE);
////	}
//	
////	RTOS_EnterInt();

//	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)		//接收中断
//	{
//		usartInfo.count %= sizeof(usartInfo.buf);			//防止串口被刷爆
//		
//        usartInfo.buf[usartInfo.count++] = USART3->DR;
//		
//		USART_ClearFlag(USART3, USART_FLAG_RXNE);
//	}

//	if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) != RESET)
//	{
//		usartInfo.rev_idle = 1;
////		usartInfo.count = 0;
//		
//		USART3->SR;
//		USART3->DR;									//读取数据注意：这句必须要，否则不能够清除中断标志位
//		USART_ClearFlag(USART3, USART_IT_IDLE);
//	}
//	
////	RTOS_ExitInt();

//}

