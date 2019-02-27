/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	info.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-23
	*
	*	版本： 		V1.1
	*
	*	说明： 		V1.0：SSID、PSWD、DEVID、APIKEY保存及读取。
	*				V1.1：取消了SSID和PSWD的保存和读写，替换为了智能配网，wifi类型的网络设备可以自动保存。
	*
	*				重要：只有当外部存储器存在时，才从中读取信息
	*					  若不存在，会读取固化在代码里的信息
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//硬件驱动
#include "info.h"
#include "at24c02.h"
#include "delay.h"
#include "usart.h"

//协议
#include "onenet.h"

//C库
#include <string.h>
#include <stdlib.h>




/*
************************************************************
*	函数名称：	Info_Check
*
*	函数功能：	检查信息是否存在
*
*	入口参数：	无
*
*	返回参数：	检查结果
*
*	说明：		检查EEPROM中的信息的长度是否正确
************************************************************
*/
unsigned char Info_Check(void)
{
	
	unsigned char rData = 0;
	
	AT24C02_ReadByte(PROID_ADDRESS, &rData);	//读取长度值
	if(rData == 0 || rData >= 10)				//如果为0或超出
		return 1;
	
	AT24C02_ReadByte(SERIAL_ADDRESS, &rData);	//读取长度值
	if(rData == 0 || rData >= 13)				//如果为0或超出
		return 2;
	
	AT24C02_ReadByte(PSWD_ADDRESS, &rData);		//读取长度值
	if(rData == 0 || rData >= 10)				//如果为0或超出
		return 3;
        
	return 0;

}

/*
************************************************************
*	函数名称：	Info_WifiLen
*
*	函数功能：	获取信息长度
*
*	入口参数：	sp：需要检查的信息-见说明
*
*	返回参数：	检查结果
*
*	说明：		0-获取devid	1-获取序列号		2-获取密码
************************************************************
*/
unsigned char Info_WifiLen(unsigned char sp)
{
	
	unsigned char len = 0;
    
    switch(sp)
    {
        case 0:
            AT24C02_ReadByte(PROID_ADDRESS, &len);		//读取长度值
			if(len == 0 || len >= 10)					//如果为0或超出
				return 1;
        break;
        
		case 1:
            AT24C02_ReadByte(SERIAL_ADDRESS, &len);		//读取长度值
			if(len == 0 || len >= 13)					//如果为0或超出
				return 2;
        break;
			
		case 2:
            AT24C02_ReadByte(PSWD_ADDRESS, &len);		//读取长度值
			if(len == 0 || len >= 10)					//如果为0或超出
				return 3;
        break;
    }
	
	return len;

}

/*
************************************************************
*	函数名称：	Info_CountLen
*
*	函数功能：	计算字段长度
*
*	入口参数：	info：需要检查的字段
*
*	返回参数：	字段长度
*
*	说明：		计算串1发过来的字段长度   以"\r\n"结尾
************************************************************
*/
unsigned char Info_CountLen(char *info)
{

	unsigned char len = 0;
	char *buf = strstr(info, ":");		//找到':'
	
	buf++;								//偏移到下一个字节，代表字段信息开始
	while(1)
	{
		if(*buf == '\r')				//直到'\r'为止
			return len;
		
		buf++;
		len++;
	}

}

/*
************************************************************
*	函数名称：	Info_Read
*
*	函数功能：	读取ssid、pswd、devid、apikey
*
*	入口参数：	无
*
*	返回参数：	读取结果
*
*	说明：		0-成功		1-失败
************************************************************
*/
_Bool Info_Read(void)
{
	
    memset(oneNetInfo.proID, 0, sizeof(oneNetInfo.proID));											//清除之前的内容
	AT24C02_ReadBytes(PROID_ADDRESS + 1, (unsigned char *)oneNetInfo.proID, Info_WifiLen(0));		//获取proid长度  读proid
    DelayXms(10);																					//延时
                
    memset(oneNetInfo.serial, 0, sizeof(oneNetInfo.serial));										//清除之前的内容
	AT24C02_ReadBytes(SERIAL_ADDRESS + 1, (unsigned char *)oneNetInfo.serial, Info_WifiLen(1));		//获取apikey长度  读serial
	
	memset(oneNetInfo.pswd, 0, sizeof(oneNetInfo.pswd));											//清除之前的内容
	AT24C02_ReadBytes(PSWD_ADDRESS + 1, (unsigned char *)oneNetInfo.pswd, Info_WifiLen(2));			//获取apikey长度  读pswd

    return 0;

}

/*
************************************************************
*	函数名称：	Info_Alter
*
*	函数功能：	更改wifi信息和项目信息
*
*	入口参数：	需要保存的字段
*
*	返回参数：	保存结果
*
*	说明：		0-不需要重新连接		1-需要重新连接
************************************************************
*/
_Bool Info_Alter(char *info)
{
    
    char *usart1Tmp;
    unsigned char usart1Count = 0;
	_Bool flag = 0;
        
	if((usart1Tmp = strstr(info, "PROID:")) != (void *)0)								//提取proid
	{
		usart1Count = Info_CountLen(usart1Tmp);											//计算长度
        if(usart1Count > 0)
        {
            memset(oneNetInfo.proID, 0, sizeof(oneNetInfo.proID));						//清除之前的内容
            strncpy(oneNetInfo.proID, usart1Tmp + 6, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save PROID: %s\r\n", oneNetInfo.proID);

			AT24C02_WriteByte(PROID_ADDRESS, strlen(oneNetInfo.proID));					//保存proid长度
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(PROID_ADDRESS + 1,										//保存proid
								(unsigned char *)oneNetInfo.proID,
								strlen(oneNetInfo.proID));
            
            flag = 1;
        }
	}
        
	if((usart1Tmp = strstr(info, "SERIAL:")) != (void *)0)								//提取serial
	{
		usart1Count = Info_CountLen(usart1Tmp);											//计算长度
        if(usart1Count > 0)
        {
            memset(oneNetInfo.serial, 0, sizeof(oneNetInfo.serial));					//清除之前的内容
            strncpy(oneNetInfo.serial, usart1Tmp + 7, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save SERIAL: %s\r\n", oneNetInfo.serial);

			AT24C02_WriteByte(SERIAL_ADDRESS, strlen(oneNetInfo.serial));				//保存serial长度
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(SERIAL_ADDRESS + 1,										//保存serial
								(unsigned char *)oneNetInfo.serial,
								strlen(oneNetInfo.serial));
            
            flag = 1;
        }
	}
	
	if((usart1Tmp = strstr(info, "PSWD:")) != (void *)0)								//提取pswd
	{
		usart1Count = Info_CountLen(usart1Tmp);											//计算长度
        if(usart1Count > 0)
        {
            memset(oneNetInfo.pswd, 0, sizeof(oneNetInfo.pswd));						//清除之前的内容
            strncpy(oneNetInfo.pswd, usart1Tmp + 5, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save PSWD: %s\r\n", oneNetInfo.pswd);

			AT24C02_WriteByte(PSWD_ADDRESS, strlen(oneNetInfo.pswd));					//保存pswd长度
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(PSWD_ADDRESS + 1,										//保存pswd
								(unsigned char *)oneNetInfo.pswd,
								strlen(oneNetInfo.pswd));
            
            flag = 1;
        }
	}
	
	return flag;

}
