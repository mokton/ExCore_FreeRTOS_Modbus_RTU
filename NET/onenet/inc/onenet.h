#ifndef _ONENET_H_
#define _ONENET_H_





struct MODBUS_LIST
{

	unsigned short *buf;		//ModBus---������ֵ�����
	unsigned short dataLen;		//ModBus---������ֵ�ĸ���
	
	struct MODBUS_LIST *next;	//��һ��

};


typedef struct
{

    char serial[12];
    char pswd[12];
	char proID[12];
	
	char ip[16];
	char port[8];
	
	const unsigned char protocol;	//Э�����ͺ�		1-edp	2-nwx	3-jtext		4-Hiscmd
									//				5-jt808			6-modbus	7-mqtt
									//				8-gr20			9-reg		10-HTTP(�Զ���)
	
	unsigned char s_addr;			//ModBus---������ַ
	unsigned char m_cmd;			//ModBus---�յ���������
	unsigned short r_addr;			//ModBus---�Ĵ�����ַ
	unsigned short r_len;			//ModBus---�Ĵ�����ȡ����
	unsigned char rev_cmd_cnt;		//�յ����������
/*************************���Ͷ���*************************/
	struct MODBUS_LIST *head, *end;
	
	unsigned char netWork : 1;		//1-OneNET����ɹ�		0-OneNET����ʧ��
	unsigned char sendData : 2;
	unsigned char errCount : 3;		//�������
	unsigned char heartBeat : 1;	//����
	unsigned char reverse : 1;		//����

} ONETNET_INFO;

extern ONETNET_INFO oneNetInfo;





#define CHECK_CONNECTED			0	//������
#define CHECK_CLOSED			1	//�ѶϿ�
#define CHECK_GOT_IP			2	//�ѻ�ȡ��IP
#define CHECK_NO_DEVICE			3	//���豸
#define CHECK_INITIAL			4	//��ʼ��״̬
#define CHECK_NO_CARD			5	//û��sim��
#define CHECK_NO_ERR			255 //

#define SEND_TYPE_OK			0	//
#define SEND_TYPE_DATA			1	//
#define SEND_TYPE_HEART			2	//




void OneNet_DevLink(const char *serial, const char *pswd, const char *devid);

unsigned char OneNet_SendData(unsigned short *value_table, unsigned short value_table_cnt);

unsigned char OneNet_SendData_Heart(void);

void OneNET_CmdHandle(void);

void OneNet_RevPro(unsigned char *cmd, unsigned short len);

_Bool OneNet_CheckListHead(void);

unsigned short *OneNet_GetListHeadBuf(void);

unsigned short OneNet_GetListHeadLen(void);

unsigned char OneNet_AddDataSendList(unsigned short *buf ,unsigned short dataLen);

_Bool OneNet_DeleteDataSendList(void);

#endif
