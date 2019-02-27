#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_


//=============================����==============================
//===========�����ṩRTOS���ڴ��������Ҳ����ʹ��C���=========
#include <stdlib.h>

#define NET_MallocBuffer	malloc

#define NET_FreeBuffer		free
//==========================================================


struct NET_SEND_LIST
{

	unsigned short dataLen;			//���ݳ���
	unsigned char *buf;				//����ָ��
	
	struct NET_SEND_LIST *next;		//��һ��

};


typedef struct
{
	
	unsigned int net_time;
	
	unsigned char data_bytes;
	
	char *cmd_resp;					//����ظ�ָ�롣�����ȡ����ص����ݣ�������ȡ��ָ���ڵ�����
	char *cmd_ipd;					//�ڽ���onenetǰͨ�������ȡ�����ݣ���������ʱ�䡢��ȡ����ip��
	char *cmd_hdl;					//������ڷ�������󣬻��ڷ��������������û�ָ���ķ�������
	
/*************************���Ͷ���*************************/
	struct NET_SEND_LIST *head, *end;
	
	unsigned short err : 4; 		//��������
	unsigned short initStep : 4;	//��ʼ������
	unsigned short dataType : 4;	//�趨���ݷ�������--16��
	unsigned short reboot : 1;		//����������־
	unsigned short netWork : 1;		//�������OK
	unsigned short send_ok : 1;		//�����豸��ķ��ͳɹ�
	unsigned short reverse : 1;		//Ԥ��

} NET_DEVICE_INFO;

extern NET_DEVICE_INFO netDeviceInfo;


typedef struct
{

	char lon[16];
	char lat[16];
	
	_Bool flag;

} GPS_INFO;

extern GPS_INFO gps;


#define NET_DEVICE_RST_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define NET_DEVICE_RST_OFF		GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define PHONE_AP_MODE			0	//1-ʹ���ֻ��ȵ�ģʽ������Χ��·��ʱ��AirKiss�޷�ʹ�ã���ʹ�ù̶���ʽ����
									//0-ʹ��AirKiss�ķ�ʽ����

#define NET_TIME_EN				1	//1-��ȡ����ʱ��		0-����ȡ


void NET_DEVICE_IO_Init(void);

_Bool NET_DEVICE_Exist(void);

_Bool NET_DEVICE_Init(unsigned char protocol, char *ip, char *port);

void NET_DEVICE_Reset(void);

_Bool NET_DEVICE_Close(void);

_Bool NET_DEVICE_ReLink(char *ip, char *port);

void NET_DEVICE_CmdHandle(char *cmd);

_Bool NET_DEVICE_SendCmd(char *cmd, char *res);

_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len);

unsigned char *NET_DEVICE_Read(void);

unsigned char *NET_DEVICE_GetIPD(unsigned char *dataPtr);

unsigned char NET_DEVICE_Check(void);

void NET_DEVICE_ReConfig(unsigned char step);

_Bool NET_DEVICE_CheckListHead(void);

unsigned char *NET_DEVICE_GetListHeadBuf(void);

unsigned short NET_DEVICE_GetListHeadLen(void);

unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short dataLen);

_Bool NET_DEVICE_DeleteDataSendList(void);

#endif
