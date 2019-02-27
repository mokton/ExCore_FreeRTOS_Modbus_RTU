/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		OneNETƽ̨Ӧ��ʾ��
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "net_device.h"

//Э���ļ�
#include "onenet.h"
#include "fault.h"
#include "modbuskit.h"

//Ӳ������
#include "usart.h"
#include "delay.h"

//C��
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MODBUS_SLAVE_ADDR		1


ONETNET_INFO oneNetInfo = {"1234567", "1234567", "113237",
							"183.230.40.42", "2987",
							6,
							0, 0, 0, 0, 0, NULL, NULL,
							0, 0, 0, 1, 0};



//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	serial�����к�
//				pswd������
//				devid�������豸��devid���ƷID
//
//	���ز�����	��
//
//	˵����		��onenetƽ̨�������ӣ��ɹ������oneNetInfo.netWork����״̬��־
//==========================================================
void OneNet_DevLink(const char *serial, const char *pswd, const char *proid)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};				//Э���
	
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\nPROID: %s,	SERIAL: %s,	PSWD: %s\r\n"
								, proid, serial, pswd);
	
	if(MODBUS_Connect(serial, pswd, proid, &modbusPacket) == 0)
	{
		NET_DEVICE_SendData(modbusPacket._data, modbusPacket._len);		//�ϴ�ƽ̨
		//NET_DEVICE_AddDataSendList(edpPacket._data, edpPacket._len);	//��������
		
		MODBUS_DeleteBuffer(&modbusPacket);								//ɾ��
		
		oneNetInfo.s_addr = MODBUS_SLAVE_ADDR;
	}
	
}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	��
//
//	���ز�����	SEND_TYPE_OK-���ͳɹ�	SEND_TYPE_DATA-��Ҫ����
//
//	˵����		
//==========================================================
unsigned char OneNet_SendData(unsigned short *value_table, unsigned short value_table_cnt)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};					//Э���
	
	if(MODBUS_PacketCmd(oneNetInfo.s_addr, oneNetInfo.m_cmd, value_table, value_table_cnt, &modbusPacket) == 0)
	{
		UsartPrintf(USART_DEBUG, "Tips:	ModBus Send %d Bytes\r\n", modbusPacket._len);
		
		NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len);	//��������
		
		MODBUS_DeleteBuffer(&modbusPacket);									//ɾ��
		
		return SEND_TYPE_OK;
	}
	else
		return SEND_TYPE_DATA;
	
}

//==========================================================
//	�������ƣ�	OneNet_HeartBeat
//
//	�������ܣ�	������������
//
//	��ڲ�����	��
//
//	���ز�����	SEND_TYPE_OK-���ͳɹ�	SEND_TYPE_HEART-��Ҫ����
//
//	˵����		
//==========================================================
unsigned char OneNet_SendData_Heart(void)
{
	
	MODBUS_PACKET_STRUCTURE modbusPacket = {NULL, 0, 0, 0};				//Э���
	
	if(!oneNetInfo.netWork)												//�������Ϊ���� �� ��Ϊ�����շ�ģʽ
		return SEND_TYPE_HEART;
	
	if(MODBUS_PacketPing(&modbusPacket))
		return SEND_TYPE_HEART;
	
	oneNetInfo.heartBeat = 1;
	
	//NET_DEVICE_SendData(edpPacket._data, edpPacket._len);				//��ƽ̨�ϴ���������
	NET_DEVICE_AddDataSendList(modbusPacket._data, modbusPacket._len);	//��������
	
	MODBUS_DeleteBuffer(&modbusPacket);									//ɾ��
	
	UsartPrintf(USART_DEBUG, "Tips:	HeartBeat Ok\r\n");
	
	return SEND_TYPE_OK;
	
}

//==========================================================
//	�������ƣ�	OneNET_CmdHandle
//
//	�������ܣ�	��ȡƽ̨rb�е�����
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNET_CmdHandle(void)
{
	
	unsigned char *dataPtr = NULL, *ipdPtr = NULL;					//����ָ��

	dataPtr = NET_DEVICE_Read();									//�ȴ�����

	if(dataPtr != NULL)												//������Ч
	{
		ipdPtr = NET_DEVICE_GetIPD(dataPtr);						//����Ƿ���ƽ̨����
		if(ipdPtr != NULL)
		{
			netDeviceInfo.send_ok = 1;
			
			if(netDeviceInfo.netWork)
				OneNet_RevPro(ipdPtr, netDeviceInfo.data_bytes);	//���д���
			else
				netDeviceInfo.cmd_ipd = (char *)ipdPtr;
		}
		else
		{
			if(strstr((char *)dataPtr, "SEND OK") != NULL)
			{
				netDeviceInfo.send_ok = 1;
				
				UsartPrintf(USART_DEBUG, "Tips:	Send Ok\r\n");
			}
			else if(strstr((char *)dataPtr, "CLOSE") != NULL && netDeviceInfo.netWork)
			{
				UsartPrintf(USART_DEBUG, "WARN:	���ӶϿ���׼������\r\n");
				
				net_fault_info.net_fault_level = net_fault_info.net_fault_level_r = NET_FAULT_LEVEL_1;
			}
			else
				NET_DEVICE_CmdHandle((char *)dataPtr);
		} 
	}

}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		ƽ̨�����·���ѯ����
//==========================================================
void OneNet_RevPro(unsigned char *cmd, unsigned short len)
{
	
	unsigned char s_addr = 0;
	
	//ƽ̨�ֶ������·���ʽ��{"cmd":"123456"}
	//ע��������ֻ��Ϊ�����ַ����Ҹ���������ż�����������һ�����ݽ������Զ������·�
	//��������ר�Żظ������������ϴ�������������
	
	if(MODBUS_UnPacketCmd(&s_addr, &oneNetInfo.m_cmd, &oneNetInfo.r_addr, &oneNetInfo.r_len, cmd, len) == 0)
	{
		UsartPrintf(USART_DEBUG, "�ӻ���ַ: %X, ����: %X, �Ĵ�����ַ: %X, ����: %x\r\n",
									s_addr, oneNetInfo.m_cmd, oneNetInfo.r_addr, oneNetInfo.r_len);
		
		if(s_addr == oneNetInfo.s_addr)
		{
			if(++oneNetInfo.rev_cmd_cnt >= 250)
				oneNetInfo.rev_cmd_cnt = 0;
		}
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MODBUS_UnPacketCmd Err\r\n");

}


/******************************************************************************************
										��Ϣ����
******************************************************************************************/

//==========================================================
//	�������ƣ�	OneNet_CheckListHead
//
//	�������ܣ�	�����Ϣ����ͷ�Ƿ�Ϊ��
//
//	��ڲ�����	��
//
//	���ز�����	0-��	1-��Ϊ��
//
//	˵����		
//==========================================================
_Bool OneNet_CheckListHead(void)
{

	if(oneNetInfo.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	OneNet_GetListHeadBuf
//
//	�������ܣ�	��ȡ��������Ҫ���͵�����ָ��
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵�����ָ��
//
//	˵����		
//==========================================================
unsigned short *OneNet_GetListHeadBuf(void)
{

	return oneNetInfo.head->buf;

}

//==========================================================
//	�������ƣ�	OneNet_GetListHeadLen
//
//	�������ܣ�	��ȡ��������Ҫ���͵����ݳ���
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵����ݳ���
//
//	˵����		
//==========================================================
unsigned short OneNet_GetListHeadLen(void)
{

	return oneNetInfo.head->dataLen;

}

//==========================================================
//	�������ƣ�	OneNet_AddDataSendList
//
//	�������ܣ�	����Ϣ����β����һ����Ϣ����
//
//	��ڲ�����	buf����Ҫ���͵�����
//				dataLen�����ݳ���(���ָ���)
//
//	���ز�����	0-�ɹ�	����-ʧ��
//
//	˵����		�첽���ͷ�ʽ
//==========================================================
unsigned char OneNet_AddDataSendList(unsigned short *buf ,unsigned short dataLen)
{
	
	struct MODBUS_LIST *current = (struct MODBUS_LIST *)NET_MallocBuffer(sizeof(struct MODBUS_LIST));
																	//�����ڴ�
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned short *)NET_MallocBuffer(dataLen << 1);//�����ڴ�
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);									//ʧ�����ͷ�
		return 2;
	}
	
	if(oneNetInfo.head == NULL)										//���headΪNULL
		oneNetInfo.head = current;									//headָ��ǰ������ڴ���
	else															//���head��ΪNULL
		oneNetInfo.end->next = current;								//��endָ��ǰ������ڴ���
	
	memcpy(current->buf, buf, dataLen << 1);						//��������
	current->dataLen = dataLen;
	current->next = NULL;											//��һ��ΪNULL
	
	oneNetInfo.end = current;										//endָ��ǰ������ڴ���
	
	return 0;

}

//==========================================================
//	�������ƣ�	OneNet_DeleteDataSendList
//
//	�������ܣ�	������ͷɾ��һ������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
_Bool OneNet_DeleteDataSendList(void)
{
	
	struct MODBUS_LIST *next = oneNetInfo.head->next;		//��������ͷ����һ�����ݵ�ַ
	
	oneNetInfo.head->dataLen = 0;
	oneNetInfo.head->next = NULL;
	NET_FreeBuffer(oneNetInfo.head->buf);					//�ͷ��ڴ�
	NET_FreeBuffer(oneNetInfo.head);						//�ͷ��ڴ�
	
	oneNetInfo.head = next;									//����ͷָ����һ������
	
	return 0;

}
