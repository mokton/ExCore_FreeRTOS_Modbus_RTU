#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"




#define MAXLENGTH 256

#define USART_DEBUG		USART1		//调试打印所使用的串口组


typedef struct
{

	unsigned char buf[256];
	unsigned char count;
	_Bool rev_idle;

} USART_INFO;

extern USART_INFO usartInfo;


void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);

void RS485_RTMode(u8 state);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

void RS485_SendString(unsigned char *str, unsigned short len);
	
void RS485Printf(char *fmt,...);


#endif
