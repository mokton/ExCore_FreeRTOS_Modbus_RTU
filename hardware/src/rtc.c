/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	rtc.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-11-09
	*
	*	版本： 		V1.1
	*
	*	说明： 		RTC初始化和功能
	*
	*	修改记录：	V1.1：修复RTC闹钟使用的BUG
						  增加RTC中断控制功能
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "delay.h"
#include "rtc.h"


RTC_INFO rtcInfo;



/*
************************************************************
*	函数名称：	RTC_Init
*
*	函数功能：	RTC初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
_Bool RTC_Init(void)
{
	
#if(USE_EXT_RCC == 1)
	unsigned char errCount = 0;
#endif

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);												//使能后备寄存器访问
	
	BKP_DeInit();																//复位备份区域
#if(USE_EXT_RCC == 1)
	RCC_LSEConfig(RCC_LSE_ON);													//设置外部低速晶振(LSE),使用外设低速晶振
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && errCount < 250)		//检查指定的RCC标志位设置与否,等待低速晶振就绪
	{
		errCount++;
		DelayMs(10);
	}
	if(errCount >= 250)
		return 1;																//初始化时钟失败,晶振有问题
#endif
	
#if(USE_EXT_RCC == 1)
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);										//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
#else
	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);								//设置RTC时钟(HSE/128),选择HES作为RTC时钟
#endif
	RCC_RTCCLKCmd(ENABLE);														//使能RTC时钟
	RTC_WaitForLastTask();														//等待最近一次对RTC寄存器的写操作完成
	RTC_WaitForSynchro();														//等待RTC寄存器同步
	
	RTC_ITConfig(RTC_IT_ALR, ENABLE);											//使能RTC闹钟中断
	RTC_WaitForLastTask();														//等待最近一次对RTC寄存器的写操作完成
	
	RTC_EnterConfigMode();														//允许配置
#if(USE_EXT_RCC == 1)
	RTC_SetPrescaler(32767);													//设置RTC预分频的值
#else
	RTC_SetPrescaler(HSE_VALUE / 128 - 1);										//设置RTC预分频的值
#endif
	RTC_WaitForLastTask();														//等待最近一次对RTC寄存器的写操作完成
	
	RTC_SetCounter(0);															//设置RTC计数器的值
	RTC_WaitForLastTask();														//等待最近一次对RTC寄存器的写操作完成
	
	RTC_ExitConfigMode();														//退出配置模式
	
	RTC_NVIC_Init();
	
	return 0;

}

/*
************************************************************
*	函数名称：	RTC_NVIC_Init
*
*	函数功能：	RTC中断初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void RTC_NVIC_Init(void)
{

#if(RTC_SEC_INT_EN == 1|| RTC_OW_INT_EN == 1 || RTC_ALR_INT_EN == 1 || RTC_ALRE_INT_EN == 1)
	NVIC_InitTypeDef nvicInitStruct;
#endif
	
#if(RTC_ALRE_INT_EN == 1)
	EXTI_InitTypeDef extiInitStructure;
#endif
	
#if(RTC_SEC_INT_EN == 1)
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
#endif
	
#if(RTC_OW_INT_EN == 1)
	RTC_ITConfig(RTC_IT_OW, ENABLE);
#endif
	
#if(RTC_ALR_INT_EN == 1)
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
#endif
	
#if(RTC_SEC_INT_EN == 1 || RTC_OW_INT_EN == 1 || RTC_ALR_INT_EN == 1)
	nvicInitStruct.NVIC_IRQChannel = RTC_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 2;	//强占优先级2
	nvicInitStruct.NVIC_IRQChannelSubPriority = 1;			//子优先级0
	
	NVIC_Init(&nvicInitStruct);
#endif
	
#if(RTC_ALRE_INT_EN == 1)
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	//停止模式下RTC闹钟连到外部中断17上可唤醒
	extiInitStructure.EXTI_Line = EXTI_Line17;
	extiInitStructure.EXTI_LineCmd = ENABLE;
	extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿
	
	EXTI_Init(&extiInitStructure);
	
	nvicInitStruct.NVIC_IRQChannel = RTCAlarm_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);
#endif

}

/*
************************************************************
*	函数名称：	RTC_SetTime
*
*	函数功能：	RTC时间设置
*
*	入口参数：	sec：秒值
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void RTC_SetTime(unsigned int sec)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);												//使能RTC和后备寄存器访问 
	RTC_SetCounter(sec);														//设置RTC计数器的值

	RTC_WaitForLastTask();														//等待最近一次对RTC寄存器的写操作完成

}

/*
************************************************************
*	函数名称：	RTC_AlarmSet
*
*	函数功能：	RTC闹钟初始化
*
*	入口参数：	sec：秒值
*
*	返回参数：	无
*
*	说明：		写入一个32bit的秒数，来代表时间。当与RTC_CNT相等时产生闹钟中断
************************************************************
*/
void RTC_AlarmSet(unsigned int sec)
{

	RTC_WaitForLastTask();
	
	RTC_SetAlarm(RTC_GetCounter() + sec);	//基于当前时间设置闹钟
	
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();					//等待RTC寄存器同步

}

/*
************************************************************
*	函数名称：	RTC_IRQHandler
*
*	函数功能：	RTC一般功能中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void RTC_IRQHandler(void)
{

	//秒中断
	if(RTC_GetITStatus(RTC_IT_SEC))
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);	//清秒中断
		
		rtcInfo.rtc_sec_int = 1;
	}
	//溢出中断
	if(RTC_GetITStatus(RTC_IT_OW))
	{
		RTC_ClearITPendingBit(RTC_IT_OW);	//清溢出中断
		
		rtcInfo.rtc_ow_int = 1;
	}
	//闹钟中断
	if(RTC_GetITStatus(RTC_IT_ALR))
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//清闹钟中断
		
		rtcInfo.rtc_alr_int = 1;
	}

}

/*
************************************************************
*	函数名称：	RTCAlarm_IRQHandler
*
*	函数功能：	RTC闹钟-外部中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void RTCAlarm_IRQHandler(void)
{
	
	rtcInfo.rtc_alre_int = 1;
	
	EXTI_ClearITPendingBit(EXTI_Line17);

}
