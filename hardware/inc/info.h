#ifndef _INFO_H_
#define _INFO_H_


#define PROID_ADDRESS       120		//起始地址，第一个数据表示长度。最大19字节。
#define SERIAL_ADDRESS      140		//起始地址，第一个数据表示长度。最大16字节。
#define PSWD_ADDRESS		160		//起始地址，第一个数据表示长度。最大16字节。





unsigned char Info_Check(void);

unsigned char Info_WifiLen(unsigned char sp);

_Bool Info_Read(void);

_Bool Info_Alter(char *info);


#endif
