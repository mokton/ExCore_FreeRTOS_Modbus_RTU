
//Ӳ������
#include "usart.h"
#include "delay.h"

//C��
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

USART_INFO usartInfo;

/*
************************************************************
*	�������ƣ�	Usart1_Init
*
*	�������ܣ�	����1��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA9		RX-PA10
************************************************************
*/
void Usart1_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef usartInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							//��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);							//��USART1��ʱ��
	
	//PA9	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;										//����Ϊ����ģʽ
	gpioInitStruct.GPIO_Pin = GPIO_Pin_9;											//��ʼ��Pin9
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;									//���ص����Ƶ��
	GPIO_Init(GPIOA, &gpioInitStruct);												//��ʼ��GPIOA
	
	//PA10	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;								//����Ϊ��������
	gpioInitStruct.GPIO_Pin = GPIO_Pin_10;											//��ʼ��Pin10
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;									//���ص����Ƶ��
	GPIO_Init(GPIOA, &gpioInitStruct);												//��ʼ��GPIOA
	
	usartInitStruct.USART_BaudRate = baud;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usartInitStruct.USART_Parity = USART_Parity_No;									//��У��
	usartInitStruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usartInitStruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART1, &usartInitStruct);
	
	USART_Cmd(USART1, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvicInitStruct.NVIC_IRQChannel = USART1_IRQn;									//usart1�жϺ�
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;										//�ж�ͨ��ʹ��
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;							//��ռ�ж����ȼ�(ֵԽС���ȼ�Խ��)
	nvicInitStruct.NVIC_IRQChannelSubPriority = 2;									//���ж����ȼ�(ֵԽС���ȼ�Խ��)
	NVIC_Init(&nvicInitStruct);														//��ʼ��NVIC

}

/*
************************************************************
*	�������ƣ�	Usart2_Init
*
*	�������ܣ�	����2��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA2		RX-PA3
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
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usartInitStruct.USART_Parity = USART_Parity_No;									//��У��
	usartInitStruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usartInitStruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART2, &usartInitStruct);
	
	USART_Cmd(USART2, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvicInitStruct.NVIC_IRQChannel = USART2_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

}

/*
************************************************************
*	�������ƣ�	Usart3_Init
*
*	�������ܣ�	����3��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PB10		RX-PB11
************************************************************
*/

void Usart3_Init(unsigned int baud)
{   // Input no ok
	GPIO_InitTypeDef gpioInitStruct;    //����һ���ṹ�������������ʼ��GPIO 
	USART_InitTypeDef usartInitStruct;    
	NVIC_InitTypeDef nvicInitStruct;        
	//ʹ�ܴ��ڵ�RCCʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��USART3���ڵ�GPIOB��ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);    

	//����ʹ�õ�GPIO������ 
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

	//���ô���  
	usartInitStruct.USART_BaudRate = baud;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartInitStruct.USART_Parity = USART_Parity_No;
	usartInitStruct.USART_StopBits = USART_StopBits_1;
	usartInitStruct.USART_WordLength = USART_WordLength_8b;

	// Configure USART3     
	USART_Init(USART3, &usartInitStruct);//���ô���3

	// Enable USART3 Receive interrupts ʹ�ܴ��ڽ����ն� 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
	//���ڷ����ж��ڷ�������ʱ����
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	// Enable the USART3     
	USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3 

	//�����ж�����   
	//Configure the NVIC Preemption Priority Bits       
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    

	// Enable the USART3 Interrupt     
	nvicInitStruct.NVIC_IRQChannel = USART3_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct); 
    
	
	// ����RS485�����������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_1;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct); 
	
	RS485_RTMode(0);
}

/*
************************************************************
*	�������ƣ�	Usart3_RT
*
*	�������ܣ�	����3 RS485 ��д״̬����
*
*	��ڲ�����	state��1 ����, 0 ����
*
*	���ز�����	��
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
*	�������ƣ�	Usart3_Init
*
*	�������ܣ�	����3��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PC10		RX-PC11
************************************************************
*/
/*
void Usart3_Init(unsigned int baud)
{	// Usart3 OK 
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	//USART ���ʱ�ӳ�ʼ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE); 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART ��� GPIO ��ʼ������
	//���� GPIO ��ʼ���ṹ�� 
	//GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_StructInit(&GPIO_InitStructure); 
	//���� USART3 �Ľ��ն˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //PC.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);     //��ʼ�� PC.11 
	//���� USART3 �ķ��Ͷ˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PC.10 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);     //��ʼ�� PC.10 
	
	USART_InitStructure.USART_BaudRate = baud; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	//���� USART3 ���첽˫�����й���ģʽ
	USART_Init(USART3, &USART_InitStructure); 
	//ʹ�� USART3 �����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
	//�ر� USART3 �����ж�
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
	//ʹ�� USART3 
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);//ʹ�ò���ӳ�书�ܣ���������ӳ�书������
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
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
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
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt, ...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//��ʽ��
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
	vsprintf((char *)UsartPrintfBuf, fmt, ap);							//��ʽ��
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
*	�������ƣ�	USART1_IRQHandler
*
*	�������ܣ�	����1�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//�����ж�
	{
		if(usart1Len >= MAXLENGTH)									//��ֹ���ݹ��࣬�����ڴ����
			usart1Len = 0;
		usart1Buf[usart1Len++] = USART1->DR;
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

}


/*UASRT2�ж�ת����ESP8266��ص������ļ�*/
//extern unsigned char usart2Buf[MAXLENGTH];
//extern unsigned char usart2Len;
/*
************************************************************
*	�������ƣ�	USART2_IRQHandler
*
*	�������ܣ�	����2�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
/*
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//�����ж�
	{
		if(usart2Len >= MAXLENGTH)									//��ֹ���ݹ��࣬�����ڴ����
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
*	�������ƣ�	USART3_IRQHandler
*
*	�������ܣ�	����3�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/

//void USART3_IRQHandler(void)
//{

////	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//�����ж�
////	{
////		if(usart3Len >= MAXLENGTH)									//��ֹ���ݹ��࣬�����ڴ����
////			usart3Len = 0;
////		usart3Buf[usart3Len++] = USART3->DR;
////		
////		USART_ClearFlag(USART3, USART_FLAG_RXNE);
////	}
//	
////	RTOS_EnterInt();

//	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)		//�����ж�
//	{
//		usartInfo.count %= sizeof(usartInfo.buf);			//��ֹ���ڱ�ˢ��
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
//		USART3->DR;									//��ȡ����ע�⣺������Ҫ�������ܹ�����жϱ�־λ
//		USART_ClearFlag(USART3, USART_IT_IDLE);
//	}
//	
////	RTOS_ExitInt();

//}

