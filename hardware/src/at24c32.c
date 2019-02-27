

//Ӳ������
#include "at24c32.h"
#include "i2c.h"
#include "delay.h"




/*
************************************************************
*	�������ƣ�	AT24C32_WriteByte
*
*	�������ܣ�	дһ���ֽڵ�EEPROM
*
*	��ڲ�����	regAddr���Ĵ�����ַ
*				byte����Ҫд�������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void AT24C32_WriteByte(unsigned short regAddr, unsigned char byte)
{
	
	I2C_WriteByte(AT24C32_ADDRESS, regAddr, &byte);

}

/*
************************************************************
*	�������ƣ�	AT24C32_WriteBytes
*
*	�������ܣ�	д����ֽڵ�EEPROM
*
*	��ڲ�����	regAddr���Ĵ�����ַ
*				byte����Ҫд������ݻ�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void AT24C32_WriteBytes(unsigned short regAddr, unsigned char *byte, unsigned char len)
{
	
	unsigned char count = 0;
	
	for(; count < len; count++)
	{
		I2C_WriteByte(AT24C32_ADDRESS, regAddr, byte);
		
		regAddr++;											//��ַ����
		byte++;												//ƫ�Ƶ��¸�����
		
		DelayXms(5);										//��Ҫ��ʱ��������һ�����ˣ�����ʮ���ֽ�ʱ��1ms����ʱ��ʱ�޷���֤ȫ���ֽ���ȷд��
	}

}

/*
************************************************************
*	�������ƣ�	AT24C32_ReadByte
*
*	�������ܣ�	��EEPROM��һ���ֽ�
*
*	��ڲ�����	regAddr���Ĵ�����ַ
*				byte����Ҫ��ȡ�����ݵĻ����ַ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void AT24C32_ReadByte(unsigned short regAddr, unsigned char *byte)
{

	I2C_ReadByte(AT24C32_ADDRESS, regAddr, byte);

}

/*
************************************************************
*	�������ƣ�	AT24C32_ReadBytes
*
*	�������ܣ�	��EEPROM������ֽ�
*
*	��ڲ�����	regAddr���Ĵ�����ַ
*				byte����Ҫд������ݻ�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void AT24C32_ReadBytes(unsigned short regAddr, unsigned char *byte, unsigned char len)
{

	I2C_ReadBytes(AT24C32_ADDRESS, regAddr, byte, len);

}

/*
************************************************************
*	�������ƣ�	AT24C32_Clear
*
*	�������ܣ�	ָ����ַ��ʼд������ͬ����
*
*	��ڲ�����	startAddr���Ĵ�����ʼ��ַ
*				byte����Ҫд�������
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		д��ָ��λ��ָ�����ȵ�ָ������
************************************************************
*/
_Bool AT24C32_Clear(unsigned short startAddr, unsigned char byte, unsigned short len)
{
	
	unsigned short count = 0;

	if(startAddr + len > 256)									//��������
		return 1;
	
	for(; count < len; count++)
	{
		I2C_WriteByte(AT24C32_ADDRESS, startAddr, &byte);
		
		startAddr++;											//��ַ����
		
		DelayXms(2);											//��Ҫ��ʱ��������һ�����ˣ�����ʮ���ֽ�ʱ��1ms����ʱ��ʱ�޷���֤ȫ���ֽ���ȷд��
	}
	
	return 0;

}
