
#include "ModbusMaster.h"

#include "usart.h"


MB_INFO mbInfo = {1, 3, 0, 0, NULL, 0, 0};


void Modbus_Handler(uint8_t *data, uint16_t dataLen)
{
//	uint16_t dataLen = sizeof(data);
	uint8_t mbframeCount = 0;
	uint16_t crc = 0;
	uint8_t i = 0;
//	UsartPrintf(USART_DEBUG, "%x\r\n", data);
	if(dataLen >= MB_FRAME_MIN_LENGTH && dataLen <= MB_FRAME_MAX_LENGTH)
	{
		crc = MODBUS_CRC16(data, dataLen - 2);
		for(i=0; i< dataLen; i++)
			UsartPrintf(USART_DEBUG, "%02x ", data[i]);
		UsartPrintf(USART_DEBUG, "\r\n");
		
		if((data[dataLen-2] != (crc & 0x00FF)) || (data[dataLen-1] != (crc >> 8)))
		{
			mbInfo.send = 0;
			UsartPrintf(USART_DEBUG, "CRC Check Error, %x %x, %x %x\r\n", data[dataLen-2], data[dataLen-1], (crc & 0x00FF), crc >> 8);
			return;
		}
		
		i = 0;
		if(mbInfo.s_id != data[i++])
		{
			mbInfo.send = 0;
			UsartPrintf(USART_DEBUG, "Slave ID Error\r\n");
			return;
		}
		if(mbInfo.mb_cmd != data[i++])
		{
			mbInfo.send = 0;
			UsartPrintf(USART_DEBUG, "Command Error\r\n");
			return;
		}
		mbInfo.r_len = data[i++] & 0x00FF;
		mbInfo.r_value = (uint16 *)MODBUS_MallocBuffer(mbInfo.r_len);
		memset(mbInfo.r_value, 0, sizeof(mbInfo.r_len));
		mbInfo.r_len = mbInfo.r_len >> 1;
		for( mbframeCount=0; mbframeCount<mbInfo.r_len; mbframeCount++)
		{
			mbInfo.r_value[mbframeCount] = data[i++] << 8;
//			mbInfo.r_value[mbframeCount] = mbInfo.r_value[mbframeCount] << 8;
			mbInfo.r_value[mbframeCount] += data[i++];
			UsartPrintf(USART_DEBUG, "%#06x ", mbInfo.r_value[mbframeCount]);
		}
		UsartPrintf(USART_DEBUG, "\r\n");
	}
	
}



void Modbus_Test(void)
{
	uint8 *mbFrame;
	uint8 mbLen = 8;
	mbFrame = (uint8 *)MODBUS_MallocBuffer(mbLen);
	memset(mbFrame, 0, mbLen);
	Modbus_Read03(0x01, 0x03, 0x0000, 0x0005, mbFrame);
	Modbus_CmdSend(MB_UART, mbFrame, mbLen);
	MODBUS_FreeBuffer(mbFrame);
}



void Modbus_Read03(uint8_t Unit, uint8_t Function, uint16_t RegStart, uint16_t RegLen, uint8_t *mbFrame)
{
	uint8 mbCount = 0;
	uint16 crc = 0;
	mbFrame[mbCount++] = Unit;
	mbFrame[mbCount++] = Function;
	mbFrame[mbCount++] = RegStart >> 8;
	mbFrame[mbCount++] = RegStart & 0x00FF;
	mbFrame[mbCount++] = RegLen >> 8;
	mbFrame[mbCount++] = RegLen & 0x00FF;
	crc = MODBUS_CRC16(mbFrame, mbCount);
	mbFrame[mbCount++] = crc & 0x00FF;
	mbFrame[mbCount++] = crc >> 8;
}

// Write Single Holding Register
void Modbus_WriteSHR(uint8_t Unit, uint8_t Function, uint16_t RegAddr, uint16_t RegValue, uint8_t *mbFrame)
{
	
}

void Modbus_CmdSend(USART_TypeDef *USARTx, unsigned char *data, unsigned char len)
{
	RS485_RTMode(1);
	Usart_SendString(MB_UART, data, len);
	RS485_RTMode(0);
}



