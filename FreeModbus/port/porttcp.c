/* ----------------------- System includes ----------------------------------*/
#include "assert.h"

/* ----------------------- C includes ---------------------------------------*/
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>

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
#include "usart.h"
#include "esp8266.h"

/* ----------------------- Defines ------------------------------------------*/
///*
#define MB_TCP_PORT_DEV        ( USART2 )
#define MB_TCP_IRQ_CH          ( USART2_IRQn )
#define MB_TCP_IRQ_HANDLER     ( USART2_IRQHandler )
#define MB_TCP_IRQ_PRIORITY    ( 1 )
//*/

/* ----------------------- Defines  -----------------------------------------*/
#define MB_TCP_DEFAULT_PORT 502 /* TCP listening port. */
#define MB_TCP_BUF_SIZE     ( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */

/* ----------------------- Prototypes ---------------------------------------*/
//void vMBPortEventClose( void );
//void vMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule, const CHAR * szFmt, ... );

/* ----------------------- Static variables ---------------------------------*/
#define MB_TCP_CLIENTS  3
//static struct tcp_pcb *pxPCBListen;
//static struct tcp_pcb *pxPCBClientToSend;
//static struct tcp_pcb *pxPCBClient[MB_TCP_CLIENTS];

static UCHAR    aucTCPBuf[MB_TCP_CLIENTS][MB_TCP_BUF_SIZE];
static USHORT   usTCPBufPos[MB_TCP_CLIENTS];
static USHORT   usTCPBufReceived[MB_TCP_CLIENTS];   

/* ----------------------- Static functions ---------------------------------*/
//static err_t    prvxMBTCPPortAccept( void *pvArg, struct tcp_pcb *pxPCB, err_t xErr );
//static err_t    prvxMBTCPPortReceive( void *pvArg, struct tcp_pcb *pxPCB, struct pbuf *p, err_t xErr );
//static void     prvvMBTCPPortError( void *pvArg, err_t xErr );




/* ----------------------- Begin implementation -----------------------------*/
BOOL
xMBTCPPortInit( USHORT usTCPPort )
{
	BOOL    xResult = TRUE;
	UCHAR 	count = 0;
	/*
	char 	TCP_SERVER[40];// = "AT+CIPSERVER=1,%d\r\n";
	USHORT	TCP_PORT = 0;
	char	numPort[6];
	if(usTCPPort == 0)
		TCP_PORT = MB_TCP_DEFAULT_PORT;
	else
		TCP_PORT = (USHORT) usTCPPort;
	//itoa(TCP_PORT, numPort, 10);
	sprintf(numPort, "%d", TCP_PORT); // %u
	strcpy(TCP_SERVER, "AT+CIPSERVER=1,");
	strcat(TCP_SERVER, numPort);
	strcat(TCP_SERVER, "\r\n");
	*/
	UsartPrintf(USART_DEBUG, "ESP8266: CIPMUX\r\n");
	while(ESP8266_SendCmd(ESP8266_MUX_LINK, "OK"))
	{
		DelayXms(50);
		if(count++ >= 10)
		{
			xResult = FALSE;
			break;
		}
	}
	if(count < 10)
	{
		count = 0;
		UsartPrintf(USART_DEBUG, "ESP8266: CIPSERVER\r\n");
		while(ESP8266_SendCmd(TCP_SERVER, "OK"))
		{
			DelayXms(50);
			if(count++ >= 10)
			{
				xResult = FALSE;
				break;
			}
		}
	}
	return xResult;
}

void
vMBTCPPortClose( void )
{
	//BOOL    xResult = TRUE;
	UCHAR 	count = 0;
	UsartPrintf(USART_DEBUG, "ESP8266: CIPSERVER CLOSE\r\n");
	while(ESP8266_SendCmd(ESP8266_NET_SERVER_CAL, "OK"))
	{
		DelayXms(50);
		if(count++ >= 10)
		{
			//xResult = FALSE;
			break;
		}
	}
}

void
vMBTCPPortDisable( void )
{
	vMBTCPPortClose();
}

BOOL
xMBTCPPortGetRequest( UCHAR **ppucMBTCPFrame, USHORT * usTCPLength )
{
	return TRUE;
}

BOOL
xMBTCPPortSendResponse( const UCHAR *pucMBTCPFrame, USHORT usTCPLength )
{
	return TRUE;
}

