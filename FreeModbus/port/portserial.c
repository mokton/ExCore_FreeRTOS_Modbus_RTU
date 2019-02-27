/*
 * FreeModbus Libary: STR71x Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/11/02 23:14:44 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "assert.h"

/* ----------------------- FreeRTOS includes --------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

/* ----------------------- STM32 includes ----------------------------------*/
#include "stm32f10x.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


#include "delay.h"


/* ----------------------- Defines ------------------------------------------*/
///*
#define MB_UART_DEV             ( USART3 )
#define MB_UART_CLOCK           ( RCC_APB1Periph_USART3 )
//#define MB_UART_AFIO_CLOCK      ( RCC_APB2Periph_AFIO )
#define MB_UART_PORT_CLOCK      ( RCC_APB2Periph_GPIOB )
#define MB_UART_TX_PORT         ( GPIOB )
#define MB_UART_TX_PIN          ( GPIO_Pin_10 )
#define MB_UART_RX_PORT         ( GPIOB )
#define MB_UART_RX_PIN          ( GPIO_Pin_11 )
#define MB_UART_IRQ_CH          ( USART3_IRQn )
#define MB_UART_IRQ_HANDLER     ( USART3_IRQHandler )
#define MB_IRQ_PRIORITY         ( 1 )
//*/
/*
#define MB_UART_DEV             ( USART2 )
#define MB_UART_CLOCK           ( RCC_APB1Periph_USART2 )
#define MB_UART_AFIO_CLOCK      ( RCC_APB2Periph_AFIO )
#define MB_UART_PORT_CLOCK      ( RCC_APB2Periph_GPIOA )
#define MB_UART_TX_PORT         ( GPIOA )
#define MB_UART_TX_PIN          ( GPIO_Pin_2 )
#define MB_UART_RX_PORT         ( GPIOA )
#define MB_UART_RX_PIN          ( GPIO_Pin_3 )
#define MB_UART_IRQ_CH          ( USART2_IRQn )
#define MB_UART_IRQ_HANDLER     ( USART2_IRQHandler )
#define MB_IRQ_PRIORITY         ( 1 )
*/
/*
#define MB_UART_DEV             ( USART1 )
#define MB_UART_CLOCK           ( RCC_APB2Periph_USART1 )
#define MB_UART_AFIO_CLOCK      ( RCC_APB2Periph_AFIO )
#define MB_UART_PORT_CLOCK      ( RCC_APB2Periph_GPIOA )
#define MB_UART_TX_PORT         ( GPIOA )
#define MB_UART_TX_PIN          ( GPIO_Pin_9 )
#define MB_UART_RX_PORT         ( GPIOA )
#define MB_UART_RX_PIN          ( GPIO_Pin_10 )
#define MB_UART_IRQ_CH          ( USART1_IRQn )
#define MB_UART_IRQ_HANDLER     ( USART1_IRQHandler )
#define MB_IRQ_PRIORITY         ( 1 )
*/
/* ----------------------- Start implementation -----------------------------*/

BOOL xMBPortSerialInit( UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            xResult = TRUE;
	
	//ESP8266_Init();
	
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    ( void )ucPort;

    switch ( eParity )
    {
    case MB_PAR_EVEN:
        USART_InitStruct.USART_Parity = USART_Parity_Even;
        break;
    case MB_PAR_ODD:
        USART_InitStruct.USART_Parity = USART_Parity_Odd;
        break;
    case MB_PAR_NONE:
        USART_InitStruct.USART_Parity = USART_Parity_No;
        break;
    default: break;
    }

    switch ( ucDataBits )
    {
    case 7:
        if( eParity == MB_PAR_NONE )
        {
            /* not supported by our hardware. */
            xResult = FALSE;
        }
        else
        {
            USART_InitStruct.USART_WordLength = USART_WordLength_8b;
        }
        break;
    case 8:
        if( eParity == MB_PAR_NONE )
        {
            USART_InitStruct.USART_WordLength = USART_WordLength_8b;
        }
        else
        {
            USART_InitStruct.USART_WordLength = USART_WordLength_9b;
        }
        break;
    default:
        xResult = FALSE;
    }

    if( xResult != FALSE )
    {	
		/* RS485 receive & transmit mode switch pin */
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
//	
//		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
//		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStruct); 
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct); 
		
        /* Enable the USART GPIO clock */
//        RCC_APB2PeriphClockCmd(MB_UART_PORT_CLOCK | MB_UART_CLOCK | MB_UART_AFIO_CLOCK, ENABLE);
		RCC_APB2PeriphClockCmd(MB_UART_PORT_CLOCK , ENABLE); //使能USART3所在的GPIOB的始终
		RCC_APB1PeriphClockCmd(MB_UART_CLOCK, ENABLE); 

        /* GPIO Configration PA.09 output PA.10 input mode */
        GPIO_InitStruct.GPIO_Pin = MB_UART_TX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(MB_UART_TX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = MB_UART_RX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(MB_UART_RX_PORT, &GPIO_InitStruct);

        /* Configure the UART. */
        USART_InitStruct.USART_BaudRate = ulBaudRate;
        USART_InitStruct.USART_StopBits = USART_StopBits_1;
        USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
        USART_Init(MB_UART_DEV, &USART_InitStruct);
        USART_Cmd(MB_UART_DEV, ENABLE);
        vMBPortSerialEnable( FALSE, FALSE );

        /* Configure the IEC for the UART interrupts. */
        NVIC_InitStruct.NVIC_IRQChannel = MB_UART_IRQ_CH;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = MB_IRQ_PRIORITY;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStruct);
		
    }
	
    return xResult;
}

/*
**	state: 0 read, 1 write
*/
void RS485_Mode(UCHAR state)
{
	//DelayUs(1);
	//ENTER_CRITICAL_SECTION(  );
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, state > 0 ? Bit_SET : Bit_RESET);
	//GPIO_WriteBit(GPIOC, GPIO_Pin_4, state > 0 ? Bit_SET : Bit_RESET);
	//GPIO_WriteBit(GPIOC, GPIO_Pin_5, state > 0 ? Bit_SET : Bit_RESET);
	//while(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1) != state > 0 ? Bit_SET : Bit_RESET);
	//EXIT_CRITICAL_SECTION(  );
	//DelayUs(1);
	
}

void vMBPortSerialClose(void)
{
    vMBPortSerialEnable(FALSE, FALSE);
    USART_Cmd(MB_UART_DEV, DISABLE);
}

void vMBPortClose(void)
{
    vMBPortSerialClose();
    vMBPortTimersDisable();
}

void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
        USART_ITConfig(MB_UART_DEV, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(MB_UART_DEV, USART_IT_RXNE, DISABLE);
    }

    if( xTxEnable )
    {
//		RS485_Mode(1);
        USART_ITConfig(MB_UART_DEV, USART_IT_TXE, ENABLE);
    }
    else
    {
//		RS485_Mode(0);
        USART_ITConfig(MB_UART_DEV, USART_IT_TXE, DISABLE);
    }
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
    USART_SendData(MB_UART_DEV, ucByte);
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = USART_ReceiveData(MB_UART_DEV);
    return TRUE;
}



void MB_UART_IRQ_HANDLER(void)
{
//	if(esp8266_ok==0)
//	{
//		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
//		{
//			if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //防止串口被刷爆
//			esp8266_buf[esp8266_cnt++] = USART2->DR;
//			
//			USART_ClearFlag(USART2, USART_FLAG_RXNE);
//		}
//	}
//	else
//	{
		if(USART_GetITStatus(MB_UART_DEV, USART_IT_TXE) == SET)
		{
			//function eMBInit in mb.c pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM
			pxMBFrameCBTransmitterEmpty();
		}
		if(USART_GetITStatus(MB_UART_DEV, USART_IT_RXNE) == SET)
		{
			//function eMBInit in mb.c pxMBFrameCBByteReceived = xMBRTUReceiveFSM
			pxMBFrameCBByteReceived();
			USART_ClearITPendingBit(MB_UART_DEV, USART_IT_RXNE);
		}
//	}
}

