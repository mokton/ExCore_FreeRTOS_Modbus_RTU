//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//OS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


//Ӳ������
#include "hwtimer.h"
#include "i2c.h"
#include "iwdg.h"
#include "at24c02.h"
#include "selfcheck.h"
#include "info.h"
#include "rtc.h"
#include "clock.h"

#include "delay.h"

#include "led.h"
#include "digitalport.h"

//Э��
#include "onenet.h"
#include "fault.h"

//�����豸
#include "net_device.h"

//����������
#include "dataStreamName.h"

//C��
#include <string.h>
#include <time.h>
#include <stdio.h>


/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );



//���Ź�����
#define IWDG_TASK_PRIO		11
#define IWDG_STK_SIZE		64
TaskHandle_t IWDG_Task_Handler;
void IWDG_Task(void *pvParameters);

//��������
#define USART_TASK_PRIO		10
#define USART_STK_SIZE		256
TaskHandle_t USART_Task_Handler;
void USART_Task(void *pvParameters);

//״̬�������
#define STATUS_TASK_PRIO		9
#define STATUS_STK_SIZE		256
TaskHandle_t STATUS_Task_Handler;
void STATUS_Task(void *pvParameters);

//���ϴ�������
#define FAULT_TASK_PRIO		8 //
#define FAULT_STK_SIZE		256
TaskHandle_t FAULT_Task_Handler;
void FAULT_Task(void *pvParameters);

//���ݷ�������
#define SEND_TASK_PRIO		6
#define SEND_STK_SIZE		64
TaskHandle_t SEND_Task_Handler;
void SEND_Task(void *pvParameters);

//�����ʼ������
#define NET_TASK_PRIO		5 //
#define NET_STK_SIZE		512
TaskHandle_t NET_Task_Handler;
void NET_Task(void *pvParameters);

//���ݷ�������
#define DATA_TASK_PRIO		4 //
#define DATA_STK_SIZE		512
TaskHandle_t DATA_Task_Handler;
void DATA_Task(void *pvParameters);

//���ݷ�������
#define DATALIST_TASK_PRIO		3 //
#define DATALIST_STK_SIZE		128
TaskHandle_t DATALIST_Task_Handler;
void DATALIST_Task(void *pvParameters);

//��Ϣ��������
#define ALTER_TASK_PRIO		2 //
#define ALTER_STK_SIZE		256
TaskHandle_t ALTER_Task_Handler;
void ALTER_Task(void *pvParameters);

//ʱ������
#define CLOCK_TASK_PRIO		1 //
#define CLOCK_STK_SIZE		256
TaskHandle_t CLOCK_Task_Handler;
void CLOCK_Task(void *pvParameters);


#define NET_COUNT	7			//�������

#define NET_TIME	60			//�趨ʱ��--��λ��

unsigned short timerCount = 0;	//ʱ�����--��λ��


TimerHandle_t t1_Thdl;


char myTime[24];

//������
DATA_STREAM dataStream[] = {
								{ZW_REDLED, &ledStatus.Led1Sta, TYPE_BOOL, 1},
								{ZW_GREENLED, &ledStatus.Led2Sta, TYPE_BOOL, 1},
								{ZW_YELLOWLED, &ledStatus.Led3Sta, TYPE_BOOL, 1},
								{ZW_BLUELED, &ledStatus.Led4Sta, TYPE_BOOL, 1},
//								{ZW_TEMPERATURE, &sht20Info.tempreture, TYPE_FLOAT, 1},
//								{ZW_HUMIDITY, &sht20Info.humidity, TYPE_FLOAT, 1},
								{ZW_TIME, myTime, TYPE_STRING, 1},
//								{"GPS", &gps, TYPE_GPS, 0},
								{ZW_ERRTYPE, &net_fault_info.net_fault_level_r, TYPE_UCHAR, 1},
							};
unsigned char dataStreamCnt = sizeof(dataStream) / sizeof(dataStream[0]);



/*
#define LED1_TASK_PRIO		1
#define LED1_STK_SIZE		32
TaskHandle_t LED1_Task_Handler;
void LED1_Task(void *pvParameters);

#define LED2_TASK_PRIO		1
#define LED2_STK_SIZE		32
TaskHandle_t LED2_Task_Handler;
void LED2_Task(void *pvParameters);

#define LED3_TASK_PRIO		1
#define LED3_STK_SIZE		32
TaskHandle_t LED3_Task_Handler;
void LED3_Task(void *pvParameters);

#define LED4_TASK_PRIO		1
#define LED4_STK_SIZE		32
TaskHandle_t LED4_Task_Handler;
void LED4_Task(void *pvParameters);

#define LED5_TASK_PRIO		2
#define LED5_STK_SIZE		32
TaskHandle_t LED5_Task_Handler;
void LED5_Task(void *pvParameters);

#define DO1_TASK_PRIO		3
#define DO1_STK_SIZE		32
TaskHandle_t DO1_Task_Handler;
void DO1_Task(void *pvParameters);

#define DO2_TASK_PRIO		3
#define DO2_STK_SIZE		32
TaskHandle_t DO2_Task_Handler;
void DO2_Task(void *pvParameters);
*/

/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
/*
void Hardware_Init(void)
{
	prvSetupHardware();
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);								//�жϿ�������������

	Delay_Init();																//Timer4��ʼ��
	
	Led_Init();																//LED��ʼ��
	
	DIO_Init();
	
	Usart1_Init(115200); 														//��ʼ������   115200bps
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alterInfo.alterBuf, sizeof(alterInfo.alterBuf), USART_RX_TYPE);
#endif
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//��ʾ��ʼ�����

}*/

void Hardware_Init(void)
{
	prvSetupHardware();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//�жϿ�������������

	Delay_Init();																//Timer4��ʼ����ʼ��
	
	Led_Init();																	//LED��ʼ��
	
	//IIC_Init();																	//���IIC���߳�ʼ��
	
	Usart1_Init(115200); 														//��ʼ������   115200bps
	UsartPrintf(USART_DEBUG, "INFO:	Usart1 Init.\r\n");
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alterInfo.alterBuf, sizeof(alterInfo.alterBuf), USART_RX_TYPE);
#endif
	
	RTC_Init();																	//��ʼ��RTC
	
	//Check_PowerOn(); 															//�ϵ��Լ�

	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//����ǿ��Ź���λ����ʾ
	{
		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
		
		RCC_ClearFlag();														//������Ź���λ��־λ
		
		net_fault_info.net_fault_level = net_fault_info.net_fault_level_r
														= NET_FAULT_LEVEL_5;	//����ȼ�5
		
		netDeviceInfo.reboot = 1;
		
		if(!Info_Check() && checkInfo.EEPROM_OK)								//���EEPROM������Ϣ
			Info_Read();
	}
	else
	{
		//�ȶ���ssid��pswd��devid��apikey
		if(!Info_Check() && checkInfo.EEPROM_OK)								//���EEPROM������Ϣ
		{
			//AT24C02_Clear(0, 255, 256);Iwdg_Feed();
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in EEPROM\r\n");
			Info_Read();
		}
		else //û������
		{
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in ROM\r\n");
		}
		
		UsartPrintf(USART_DEBUG, "2.DEVID: %s,     APIKEY: %s\r\n"
								, oneNetInfo.devID, oneNetInfo.apiKey);
		
		netDeviceInfo.reboot = 0;
	}
	
	Iwdg_Init(4, 1250); 														//64��Ƶ��ÿ��625�Σ�����1250�Σ�2s
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//��ʾ��ʼ�����

}


/*
************************************************************
*	�������ƣ�	OS_TimerCallBack
*
*	�������ܣ�	��ʱ�������״̬��־λ
*
*	��ڲ�����	�����ʱ�����
*
*	���ز�����	��
*
*	˵����		��ʱ�����񡣶�ʱ�������״̬�������������趨ʱ�����������ӣ������ƽ̨����
************************************************************
*/
void OS_TimerCallBack(TimerHandle_t xTimer)
{
	
	if(oneNetInfo.errCount >= NET_COUNT)								//������ʹ�������ﵽNET_COUNT��
	{
		UsartPrintf(USART_DEBUG, "Tips:	Timer Check Err-Send\r\n");
		
		oneNetInfo.errCount = 0;
		
		net_fault_info.net_fault_level = NET_FAULT_LEVEL_1;				//����ȼ�1
	}
	
	if(oneNetInfo.netWork == 0)											//����ڹ涨ʱ�������绹δ����ɹ�
	{
		if(++timerCount >= NET_TIME) 									//�������Ͽ���ʱ
		{
			UsartPrintf(USART_DEBUG, "Tips:	Timer Check Err-Init\r\n");
			
			timerCount = 0;
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_3;			//����ȼ�3
		}
	}
	else
	{
		timerCount = 0;													//�������
	}

}

int main(void)
{	
	Hardware_Init();
	
	//����Ӧ������
	
	xTaskCreate((TaskFunction_t)IWDG_Task, "IWDG", IWDG_STK_SIZE, NULL, IWDG_TASK_PRIO, (TaskHandle_t*)&IWDG_Task_Handler);
	
	xTaskCreate((TaskFunction_t)USART_Task, "USART", USART_STK_SIZE, NULL, USART_TASK_PRIO, (TaskHandle_t*)&USART_Task_Handler);
	
	xTaskCreate((TaskFunction_t)STATUS_Task, "STATUS", STATUS_STK_SIZE, NULL, STATUS_TASK_PRIO, (TaskHandle_t*)&STATUS_Task_Handler);
	
	xTaskCreate((TaskFunction_t)FAULT_Task, "FAULT", FAULT_STK_SIZE, NULL, FAULT_TASK_PRIO, (TaskHandle_t*)&FAULT_Task_Handler);
	
	//xTaskCreate((TaskFunction_t)SENSOR_Task, "SENSOR", SENSOR_STK_SIZE, NULL, SENSOR_TASK_PRIO, (TaskHandle_t*)&SENSOR_Task_Handler);
	
	xTaskCreate((TaskFunction_t)SEND_Task, "SEND", SEND_STK_SIZE, NULL, SEND_TASK_PRIO, (TaskHandle_t*)&SEND_Task_Handler);
	
	xTaskCreate((TaskFunction_t)NET_Task, "NET", NET_STK_SIZE, NULL, NET_TASK_PRIO, (TaskHandle_t*)&NET_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DATA_Task, "DATA", DATA_STK_SIZE, NULL, DATA_TASK_PRIO, (TaskHandle_t*)&DATA_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DATALIST_Task, "DATALIST", DATALIST_STK_SIZE, NULL, DATALIST_TASK_PRIO, (TaskHandle_t*)&DATALIST_Task_Handler);
	
	xTaskCreate((TaskFunction_t)ALTER_Task, "ALTER", ALTER_STK_SIZE, NULL, ALTER_TASK_PRIO, (TaskHandle_t*)&ALTER_Task_Handler);
	
	xTaskCreate((TaskFunction_t)CLOCK_Task, "CLOCK", CLOCK_STK_SIZE, NULL, CLOCK_TASK_PRIO, (TaskHandle_t*)&CLOCK_Task_Handler);
	
	t1_Thdl = xTimerCreate("Timer1", 200, pdTRUE, (void *)1, (TimerCallbackFunction_t)OS_TimerCallBack);
	xTimerStart(t1_Thdl, 1);
	
	UsartPrintf(USART_DEBUG, "4.OSStart\r\n");		//��ʾ����ʼִ��
	
	/*
	xTaskCreate((TaskFunction_t)LED1_Task, "LED1", LED1_STK_SIZE, NULL, LED1_TASK_PRIO, (TaskHandle_t*)&LED1_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED2_Task, "LED2", LED2_STK_SIZE, NULL, LED2_TASK_PRIO, (TaskHandle_t*)&LED2_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED3_Task, "LED3", LED3_STK_SIZE, NULL, LED3_TASK_PRIO, (TaskHandle_t*)&LED3_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED4_Task, "LED4", LED4_STK_SIZE, NULL, LED4_TASK_PRIO, (TaskHandle_t*)&LED4_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED5_Task, "LED5", LED5_STK_SIZE, NULL, LED5_TASK_PRIO, (TaskHandle_t*)&LED5_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DO1_Task, "DO1", DO1_STK_SIZE, NULL, DO1_TASK_PRIO, (TaskHandle_t*)&DO1_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DO2_Task, "DO2", DO2_STK_SIZE, NULL, DO2_TASK_PRIO, (TaskHandle_t*)&DO2_Task_Handler);
	*/
	vTaskStartScheduler();							//��ʼ�������
	
}

/*
************************************************************
*	�������ƣ�	IWDG_Task
*
*	�������ܣ�	������Ź�
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���Ź�����
************************************************************
*/
void IWDG_Task(void *pdata)
{

	while(1)
	{
	
		Iwdg_Feed(); 		//ι��
		
		RTOS_TimeDly(50); 	//��������250ms
	
	}

}

/*
************************************************************
*	�������ƣ�	HEART_Task
*
*	�������ܣ�	�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		�������񡣷����������󲢵ȴ�������Ӧ�������趨ʱ����û����Ӧ������ƽ̨����
************************************************************
*/
void STATUS_Task(void *pdata)
{

	while(1)
	{
		
		OneNet_Status();				//״̬���
		
		RTOS_TimeDly(4000);				//��������20s
	
	}

}

/*
************************************************************
*	�������ƣ�	SEND_Task
*
*	�������ܣ�	�ϴ�����������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�������
************************************************************
*/
void SEND_Task(void *pdata)
{

	while(1)
	{
		
		oneNetInfo.sendData = SEND_TYPE_DATA;
		
		RTOS_TimeDly(3000);							//��������15s
		
	}

}

/*
************************************************************
*	�������ƣ�	USART_Task
*
*	�������ܣ�	����ƽ̨�·�������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ڽ�������������ģʽ��ʱ���ȴ�ƽ̨�·����������������
************************************************************
*/
void USART_Task(void *pdata)
{
	
	while(1)
	{
		
		OneNET_CmdHandle();
		
		RTOS_TimeDly(2);		//��������10ms
	
	}

}

/*
************************************************************
*	�������ƣ�	DATA_Task
*
*	�������ܣ�	ƽ̨�·���������ݷ���
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ݷ�����������ƽ̨�·�ָ���ķ���������͸��ģʽ��ʱ���յ�֮���������أ���͸��ģʽ��Ϊ��Ҫ����'>'���ţ�����ʹ������ķ�ʽ��������
************************************************************
*/
void DATA_Task(void *pdata)
{

	while(1)
	{
	
		switch(oneNetInfo.sendData)
		{
			case SEND_TYPE_DATA:
				oneNetInfo.sendData = OneNet_SendData(FORMAT_TYPE3, oneNetInfo.devID, oneNetInfo.apiKey, dataStream, dataStreamCnt);	//�ϴ����ݵ�ƽ̨
			break;
			
			case SEND_TYPE_REGISTER:
				oneNetInfo.sendData = OneNet_RegisterDevice("your Master-Key", "DeviceName", "auth_info", "desc", "false");
			break;
		}
		
		RTOS_TimeDly(10);					//��������50ms
	
	}

}

/*
************************************************************
*	�������ƣ�	DATALIST_Task
*
*	�������ܣ�	ѭ������������ߴ����͵����ݿ�
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void DATALIST_Task(void *pvParameter)
{

	while(1)
	{
		
		if(NET_DEVICE_CheckListHead())
		{
			NET_DEVICE_SendData(NET_DEVICE_GetListHeadBuf(), NET_DEVICE_GetListHeadLen());
			NET_DEVICE_DeleteDataSendList();
		}
	
		RTOS_TimeDly(100);					//��������500ms
	
	}

}

/*
************************************************************
*	�������ƣ�	FAULT_Task
*
*	�������ܣ�	����״̬������
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ϴ������񡣵�������������豸����ʱ�����Ƕ�Ӧ��־λ��Ȼ���н��д���
************************************************************
*/
void FAULT_Task(void *pdata)
{

	while(1)
	{
		
		if(net_fault_info.net_fault_level != NET_FAULT_LEVEL_0)					//��������־������
		{
			UsartPrintf(USART_DEBUG, "WARN:	NET Fault Process\r\n");
			
			NET_Fault_Process();												//�����������
		}
		
		RTOS_TimeDly(10);														//��������50ms
	
	}

}

/*
************************************************************
*	�������ƣ�	NET_Task
*
*	�������ܣ�	�������ӡ�ƽ̨����
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���������������񡣻������������߼����������״̬������д�����״̬��Ȼ���������������
************************************************************
*/
void NET_Task(void *pdata)
{
	
	NET_DEVICE_IO_Init();													//�����豸IO��ʼ��
	NET_DEVICE_Reset();														//�����豸��λ

	while(1)
	{
		
		if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))		//��û������ �� ����ģ���⵽ʱ
		{
			if(!NET_DEVICE_Init(oneNetInfo.protocol, oneNetInfo.ip, oneNetInfo.port))//��ʼ�������豸������������
			{
				UsartPrintf(USART_DEBUG, "Tips:	NetWork OK\r\n");
		
				oneNetInfo.netWork = 1;
				
				//if(gps.flag)
				//	dataStream[7].flag = 1;									//GPS������׼���ϴ�
			}
		}
		
		if(checkInfo.NET_DEVICE_OK == DEV_ERR) 								//�������豸δ�����
		{
			if(!NET_DEVICE_Exist())											//�����豸���
			{
				UsartPrintf(USART_DEBUG, "NET Device :Ok\r\n");
				checkInfo.NET_DEVICE_OK = DEV_OK;							//��⵽�����豸�����
			}
			else
				UsartPrintf(USART_DEBUG, "NET Device :Error\r\n");
		}
		
		RTOS_TimeDly(5);													//��������25ms
	
	}

}

/*
************************************************************
*	�������ƣ�	ALTER_Task
*
*	�������ܣ�	ͨ�����ڸ���SSID��PSWD��DEVID��APIKEY
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		���ĺ�ᱣ�浽EEPROM��
************************************************************
*/
void ALTER_Task(void *pdata)
{

    while(1)
    {
    
        memset(alterInfo.alterBuf, 0, sizeof(alterInfo.alterBuf));
		
		while(!alterInfo.rev_idle)
            RTOS_TimeDly(20);														//ÿ100ms���һ��
		
		alterInfo.rev_idle = 0;
		
        UsartPrintf(USART_DEBUG, "\r\nAlter Rev\r\n%s\r\n", alterInfo.alterBuf);
        
		if(checkInfo.EEPROM_OK == DEV_OK)											//���EEPROM����
		{
			if(Info_Alter(alterInfo.alterBuf))										//������Ϣ
			{
				oneNetInfo.netWork = 0;												//���������ƽ̨
				NET_DEVICE_ReConfig(0);
			}
		}
    
    }

}

/*
************************************************************
*	�������ƣ�	CLOCK_Task
*
*	�������ܣ�	����Уʱ��ʱ����ʾ
*
*	��ڲ�����	void���͵Ĳ���ָ��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void CLOCK_Task(void *pvParameter)
{
	
#if(NET_TIME_EN == 1)
	unsigned int second = 0, second_pre = 0, err_count = 0;				//second��ʵʱʱ�䣬second_pre��ֵ�Ƚϡ�err_count��ȡ��ʱ
	struct tm *time;
	_Bool get_net_time = 1;
#endif

	while(1)
	{
	
#if(NET_TIME_EN == 1)
		if(get_net_time)												//��Ҫ��ȡʱ��
		{
			dataStream[6].flag = 0;										//���ϴ�ʱ��
			
			if(++err_count >= 6000)										//ʮ���ӻ���ȡ���������»�ȡ
			{
				err_count = 0;
				netDeviceInfo.net_time = 0;
				oneNetInfo.netWork = 0;
				NET_DEVICE_ReConfig(0);
			}

			if(netDeviceInfo.net_time)
			{
				second = RTC_GetCounter();
				
				if(((netDeviceInfo.net_time <= second + 300) && (netDeviceInfo.net_time >= second - 300)) || (second <= 100))
				{														//����ڡ�5�����ڣ�����Ϊʱ����ȷ
					RTC_SetTime(netDeviceInfo.net_time + 4);			//����RTCʱ�䣬��4�ǲ��ϴ�ŵ�ʱ���
					
					get_net_time = 0;
					err_count = 0;
					
					dataStream[6].flag = 1;								//�ϴ�ʱ��
				}
			}
		}

		second = RTC_GetCounter();										//��ȡ��ֵ
		
		if(second > second_pre)
		{
			second_pre = second;
			time = localtime((const time_t *)&second);					//����ֵתΪtm�ṹ����ʾ��ʱ��
			
			memset(myTime, 0, sizeof(myTime));
			snprintf(myTime, sizeof(myTime), "%d-%d-%d %d:%d:%d",
							time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
							time->tm_hour, time->tm_min, time->tm_sec);
		
			if(time->tm_hour == 0 && time->tm_min == 0 && time->tm_sec == 0)//ÿ��0��ʱ������һ��ʱ��
			{
				get_net_time = 1;
				netDeviceInfo.net_time = 0;
				oneNetInfo.netWork = 0;
				NET_DEVICE_ReConfig(0);
			}
		}
#endif
		
		RTOS_TimeDly(20);													//��������100ms
	
	}

}



/*
void LED1_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led1_Set(LED_ON);
		
		//vTaskDelay(200);
		RTOS_TimeDly(400);						//��������15s
		
		Led1_Set(LED_OFF);
		
		RTOS_TimeDly(400);						//��������15s
		
	}

}

void LED2_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led2_Set(LED_ON);
		
		RTOS_TimeDly(200);						//��������1s
		
		Led2_Set(LED_OFF);
		
		RTOS_TimeDly(200);
		
	}

}

void LED3_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led3_Set(LED_ON);
		
		RTOS_TimeDly(100);
		
		Led3_Set(LED_OFF);
		
		RTOS_TimeDly(100);
		
	}

}

void LED4_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led4_Set(LED_ON);
		
		RTOS_TimeDly(2000);
		
		Led4_Set(LED_OFF);
		
		RTOS_TimeDly(2000);
		
	}

}

void LED5_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led5_Set(LED_OFF);
		
		RTOS_TimeDly(25);
		
		Led5_Set(LED_ON);
		
		RTOS_TimeDly(375);
		
	}

}

void DO1_Task(void *pvParameters)
{
	
	while(1)
	{
		
		DO1_Set(DIO_OFF);
		
		RTOS_TimeDly(2000);
		
		DO1_Set(DIO_ON);
		
		RTOS_TimeDly(2000);
		
	}

}

void DO2_Task(void *pvParameters)
{
	
	while(1)
	{
		
		DO2_Set(DIO_ON);
		
		RTOS_TimeDly(2000);
		
		DO2_Set(DIO_OFF);
		
		RTOS_TimeDly(2000);
		
	}

}
*/

static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 12MHz * 6 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_6 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	//vParTestInitialise();
}


