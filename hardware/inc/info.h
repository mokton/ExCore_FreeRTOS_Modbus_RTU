#ifndef _INFO_H_
#define _INFO_H_


#define PROID_ADDRESS       120		//��ʼ��ַ����һ�����ݱ�ʾ���ȡ����19�ֽڡ�
#define SERIAL_ADDRESS      140		//��ʼ��ַ����һ�����ݱ�ʾ���ȡ����16�ֽڡ�
#define PSWD_ADDRESS		160		//��ʼ��ַ����һ�����ݱ�ʾ���ȡ����16�ֽڡ�





unsigned char Info_Check(void);

unsigned char Info_WifiLen(unsigned char sp);

_Bool Info_Read(void);

_Bool Info_Alter(char *info);


#endif
