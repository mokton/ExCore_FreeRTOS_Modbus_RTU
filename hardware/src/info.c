/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	info.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-23
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		V1.0��SSID��PSWD��DEVID��APIKEY���漰��ȡ��
	*				V1.1��ȡ����SSID��PSWD�ı���Ͷ�д���滻Ϊ������������wifi���͵������豸�����Զ����档
	*
	*				��Ҫ��ֻ�е��ⲿ�洢������ʱ���Ŵ��ж�ȡ��Ϣ
	*					  �������ڣ����ȡ�̻��ڴ��������Ϣ
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//Ӳ������
#include "info.h"
#include "at24c02.h"
#include "delay.h"
#include "usart.h"

//Э��
#include "onenet.h"

//C��
#include <string.h>
#include <stdlib.h>




/*
************************************************************
*	�������ƣ�	Info_Check
*
*	�������ܣ�	�����Ϣ�Ƿ����
*
*	��ڲ�����	��
*
*	���ز�����	�����
*
*	˵����		���EEPROM�е���Ϣ�ĳ����Ƿ���ȷ
************************************************************
*/
unsigned char Info_Check(void)
{
	
	unsigned char rData = 0;
	
	AT24C02_ReadByte(PROID_ADDRESS, &rData);	//��ȡ����ֵ
	if(rData == 0 || rData >= 10)				//���Ϊ0�򳬳�
		return 1;
	
	AT24C02_ReadByte(SERIAL_ADDRESS, &rData);	//��ȡ����ֵ
	if(rData == 0 || rData >= 13)				//���Ϊ0�򳬳�
		return 2;
	
	AT24C02_ReadByte(PSWD_ADDRESS, &rData);		//��ȡ����ֵ
	if(rData == 0 || rData >= 10)				//���Ϊ0�򳬳�
		return 3;
        
	return 0;

}

/*
************************************************************
*	�������ƣ�	Info_WifiLen
*
*	�������ܣ�	��ȡ��Ϣ����
*
*	��ڲ�����	sp����Ҫ������Ϣ-��˵��
*
*	���ز�����	�����
*
*	˵����		0-��ȡdevid	1-��ȡ���к�		2-��ȡ����
************************************************************
*/
unsigned char Info_WifiLen(unsigned char sp)
{
	
	unsigned char len = 0;
    
    switch(sp)
    {
        case 0:
            AT24C02_ReadByte(PROID_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 10)					//���Ϊ0�򳬳�
				return 1;
        break;
        
		case 1:
            AT24C02_ReadByte(SERIAL_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 13)					//���Ϊ0�򳬳�
				return 2;
        break;
			
		case 2:
            AT24C02_ReadByte(PSWD_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 10)					//���Ϊ0�򳬳�
				return 3;
        break;
    }
	
	return len;

}

/*
************************************************************
*	�������ƣ�	Info_CountLen
*
*	�������ܣ�	�����ֶγ���
*
*	��ڲ�����	info����Ҫ�����ֶ�
*
*	���ز�����	�ֶγ���
*
*	˵����		���㴮1���������ֶγ���   ��"\r\n"��β
************************************************************
*/
unsigned char Info_CountLen(char *info)
{

	unsigned char len = 0;
	char *buf = strstr(info, ":");		//�ҵ�':'
	
	buf++;								//ƫ�Ƶ���һ���ֽڣ������ֶ���Ϣ��ʼ
	while(1)
	{
		if(*buf == '\r')				//ֱ��'\r'Ϊֹ
			return len;
		
		buf++;
		len++;
	}

}

/*
************************************************************
*	�������ƣ�	Info_Read
*
*	�������ܣ�	��ȡssid��pswd��devid��apikey
*
*	��ڲ�����	��
*
*	���ز�����	��ȡ���
*
*	˵����		0-�ɹ�		1-ʧ��
************************************************************
*/
_Bool Info_Read(void)
{
	
    memset(oneNetInfo.proID, 0, sizeof(oneNetInfo.proID));											//���֮ǰ������
	AT24C02_ReadBytes(PROID_ADDRESS + 1, (unsigned char *)oneNetInfo.proID, Info_WifiLen(0));		//��ȡproid����  ��proid
    DelayXms(10);																					//��ʱ
                
    memset(oneNetInfo.serial, 0, sizeof(oneNetInfo.serial));										//���֮ǰ������
	AT24C02_ReadBytes(SERIAL_ADDRESS + 1, (unsigned char *)oneNetInfo.serial, Info_WifiLen(1));		//��ȡapikey����  ��serial
	
	memset(oneNetInfo.pswd, 0, sizeof(oneNetInfo.pswd));											//���֮ǰ������
	AT24C02_ReadBytes(PSWD_ADDRESS + 1, (unsigned char *)oneNetInfo.pswd, Info_WifiLen(2));			//��ȡapikey����  ��pswd

    return 0;

}

/*
************************************************************
*	�������ƣ�	Info_Alter
*
*	�������ܣ�	����wifi��Ϣ����Ŀ��Ϣ
*
*	��ڲ�����	��Ҫ������ֶ�
*
*	���ز�����	������
*
*	˵����		0-����Ҫ��������		1-��Ҫ��������
************************************************************
*/
_Bool Info_Alter(char *info)
{
    
    char *usart1Tmp;
    unsigned char usart1Count = 0;
	_Bool flag = 0;
        
	if((usart1Tmp = strstr(info, "PROID:")) != (void *)0)								//��ȡproid
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(oneNetInfo.proID, 0, sizeof(oneNetInfo.proID));						//���֮ǰ������
            strncpy(oneNetInfo.proID, usart1Tmp + 6, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save PROID: %s\r\n", oneNetInfo.proID);

			AT24C02_WriteByte(PROID_ADDRESS, strlen(oneNetInfo.proID));					//����proid����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(PROID_ADDRESS + 1,										//����proid
								(unsigned char *)oneNetInfo.proID,
								strlen(oneNetInfo.proID));
            
            flag = 1;
        }
	}
        
	if((usart1Tmp = strstr(info, "SERIAL:")) != (void *)0)								//��ȡserial
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(oneNetInfo.serial, 0, sizeof(oneNetInfo.serial));					//���֮ǰ������
            strncpy(oneNetInfo.serial, usart1Tmp + 7, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save SERIAL: %s\r\n", oneNetInfo.serial);

			AT24C02_WriteByte(SERIAL_ADDRESS, strlen(oneNetInfo.serial));				//����serial����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(SERIAL_ADDRESS + 1,										//����serial
								(unsigned char *)oneNetInfo.serial,
								strlen(oneNetInfo.serial));
            
            flag = 1;
        }
	}
	
	if((usart1Tmp = strstr(info, "PSWD:")) != (void *)0)								//��ȡpswd
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(oneNetInfo.pswd, 0, sizeof(oneNetInfo.pswd));						//���֮ǰ������
            strncpy(oneNetInfo.pswd, usart1Tmp + 5, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save PSWD: %s\r\n", oneNetInfo.pswd);

			AT24C02_WriteByte(PSWD_ADDRESS, strlen(oneNetInfo.pswd));					//����pswd����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(PSWD_ADDRESS + 1,										//����pswd
								(unsigned char *)oneNetInfo.pswd,
								strlen(oneNetInfo.pswd));
            
            flag = 1;
        }
	}
	
	return flag;

}
