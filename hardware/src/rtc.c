/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	rtc.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-11-09
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		RTC��ʼ���͹���
	*
	*	�޸ļ�¼��	V1.1���޸�RTC����ʹ�õ�BUG
						  ����RTC�жϿ��ƹ���
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "delay.h"
#include "rtc.h"


RTC_INFO rtcInfo;



/*
************************************************************
*	�������ƣ�	RTC_Init
*
*	�������ܣ�	RTC��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
_Bool RTC_Init(void)
{
	
#if(USE_EXT_RCC == 1)
	unsigned char errCount = 0;
#endif

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);												//ʹ�ܺ󱸼Ĵ�������
	
	BKP_DeInit();																//��λ��������
#if(USE_EXT_RCC == 1)
	RCC_LSEConfig(RCC_LSE_ON);													//�����ⲿ���پ���(LSE),ʹ��������پ���
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && errCount < 250)		//���ָ����RCC��־λ�������,�ȴ����پ������
	{
		errCount++;
		DelayMs(10);
	}
	if(errCount >= 250)
		return 1;																//��ʼ��ʱ��ʧ��,����������
#endif
	
#if(USE_EXT_RCC == 1)
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);										//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
#else
	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);								//����RTCʱ��(HSE/128),ѡ��HES��ΪRTCʱ��
#endif
	RCC_RTCCLKCmd(ENABLE);														//ʹ��RTCʱ��
	RTC_WaitForLastTask();														//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_WaitForSynchro();														//�ȴ�RTC�Ĵ���ͬ��
	
	RTC_ITConfig(RTC_IT_ALR, ENABLE);											//ʹ��RTC�����ж�
	RTC_WaitForLastTask();														//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	
	RTC_EnterConfigMode();														//��������
#if(USE_EXT_RCC == 1)
	RTC_SetPrescaler(32767);													//����RTCԤ��Ƶ��ֵ
#else
	RTC_SetPrescaler(HSE_VALUE / 128 - 1);										//����RTCԤ��Ƶ��ֵ
#endif
	RTC_WaitForLastTask();														//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	
	RTC_SetCounter(0);															//����RTC��������ֵ
	RTC_WaitForLastTask();														//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	
	RTC_ExitConfigMode();														//�˳�����ģʽ
	
	RTC_NVIC_Init();
	
	return 0;

}

/*
************************************************************
*	�������ƣ�	RTC_NVIC_Init
*
*	�������ܣ�	RTC�жϳ�ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
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
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 2;	//ǿռ���ȼ�2
	nvicInitStruct.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�0
	
	NVIC_Init(&nvicInitStruct);
#endif
	
#if(RTC_ALRE_INT_EN == 1)
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	//ֹͣģʽ��RTC���������ⲿ�ж�17�Ͽɻ���
	extiInitStructure.EXTI_Line = EXTI_Line17;
	extiInitStructure.EXTI_LineCmd = ENABLE;
	extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//������
	
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
*	�������ƣ�	RTC_SetTime
*
*	�������ܣ�	RTCʱ������
*
*	��ڲ�����	sec����ֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void RTC_SetTime(unsigned int sec)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);												//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(sec);														//����RTC��������ֵ

	RTC_WaitForLastTask();														//�ȴ����һ�ζ�RTC�Ĵ�����д�������

}

/*
************************************************************
*	�������ƣ�	RTC_AlarmSet
*
*	�������ܣ�	RTC���ӳ�ʼ��
*
*	��ڲ�����	sec����ֵ
*
*	���ز�����	��
*
*	˵����		д��һ��32bit��������������ʱ�䡣����RTC_CNT���ʱ���������ж�
************************************************************
*/
void RTC_AlarmSet(unsigned int sec)
{

	RTC_WaitForLastTask();
	
	RTC_SetAlarm(RTC_GetCounter() + sec);	//���ڵ�ǰʱ����������
	
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();					//�ȴ�RTC�Ĵ���ͬ��

}

/*
************************************************************
*	�������ƣ�	RTC_IRQHandler
*
*	�������ܣ�	RTCһ�㹦���ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void RTC_IRQHandler(void)
{

	//���ж�
	if(RTC_GetITStatus(RTC_IT_SEC))
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);	//�����ж�
		
		rtcInfo.rtc_sec_int = 1;
	}
	//����ж�
	if(RTC_GetITStatus(RTC_IT_OW))
	{
		RTC_ClearITPendingBit(RTC_IT_OW);	//������ж�
		
		rtcInfo.rtc_ow_int = 1;
	}
	//�����ж�
	if(RTC_GetITStatus(RTC_IT_ALR))
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//�������ж�
		
		rtcInfo.rtc_alr_int = 1;
	}

}

/*
************************************************************
*	�������ƣ�	RTCAlarm_IRQHandler
*
*	�������ܣ�	RTC����-�ⲿ�ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void RTCAlarm_IRQHandler(void)
{
	
	rtcInfo.rtc_alre_int = 1;
	
	EXTI_ClearITPendingBit(EXTI_Line17);

}
