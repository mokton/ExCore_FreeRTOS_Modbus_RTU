#ifndef _MODBUSMASTER_H_
#define _MODBUSMASTER_H_

#include "string.h"
#include "stm32f10x.h"

#include "ModBusKit.h"


#define MB_UART					USART3

#define MB_FRAME_MIN_LENGTH		(u8)4
#define MB_FRAME_MAX_LENGTH		(u8)253

typedef struct
{
	unsigned char s_id;			//ModBus---从机地址
	unsigned char mb_cmd;		//ModBus---收到的命令码
	unsigned short r_addr;		//ModBus---寄存器地址
	unsigned short r_len;		//ModBus---寄存器读取长度
	unsigned short *r_value;	//ModBus---寄存器返回的数值
	unsigned short m_addr;		//对应的本地寄存器起始地址
//	unsigned char rev_cmd_cnt;
	unsigned char send : 1;  	//是否在发送

} MB_INFO;

extern MB_INFO mbInfo;


void Modbus_Test(void);
	

void Modbus_Handler(uint8_t *data, uint16_t dataLen);
void Modbus_Read03(uint8_t Unit, uint8_t Function, uint16_t RegStart, uint16_t RegLen, uint8_t *mbFrame);
void Modbus_WriteSHR(uint8_t Unit, uint8_t Function, uint16_t RegAddr, uint16_t RegValue, uint8_t *mbFrame);
void Modbus_CmdSend(USART_TypeDef *USARTx, unsigned char *data, unsigned char len);



#endif
