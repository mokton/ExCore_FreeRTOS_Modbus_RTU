//单片机头文件
#include "stm32f10x.h"

//OS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


//硬件驱动
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

//协议
#include "onenet.h"
#include "fault.h"

//网络设备
#include "net_device.h"

//中文数据流
#include "dataStreamName.h"

//C库
#include <string.h>
#include <time.h>
#include <stdio.h>


/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );



//看门狗任务
#define IWDG_TASK_PRIO		11
#define IWDG_STK_SIZE		64
TaskHandle_t IWDG_Task_Handler;
void IWDG_Task(void *pvParameters);

//串口任务
#define USART_TASK_PRIO		10
#define USART_STK_SIZE		256
TaskHandle_t USART_Task_Handler;
void USART_Task(void *pvParameters);

//状态检查任务
#define STATUS_TASK_PRIO		9
#define STATUS_STK_SIZE		256
TaskHandle_t STATUS_Task_Handler;
void STATUS_Task(void *pvParameters);

//故障处理任务
#define FAULT_TASK_PRIO		8 //
#define FAULT_STK_SIZE		256
TaskHandle_t FAULT_Task_Handler;
void FAULT_Task(void *pvParameters);

//数据发送任务
#define SEND_TASK_PRIO		6
#define SEND_STK_SIZE		64
TaskHandle_t SEND_Task_Handler;
void SEND_Task(void *pvParameters);

//网络初始化任务
#define NET_TASK_PRIO		5 //
#define NET_STK_SIZE		512
TaskHandle_t NET_Task_Handler;
void NET_Task(void *pvParameters);

//数据反馈任务
#define DATA_TASK_PRIO		4 //
#define DATA_STK_SIZE		512
TaskHandle_t DATA_Task_Handler;
void DATA_Task(void *pvParameters);

//数据发送任务
#define DATALIST_TASK_PRIO		3 //
#define DATALIST_STK_SIZE		128
TaskHandle_t DATALIST_Task_Handler;
void DATALIST_Task(void *pvParameters);

//信息更改任务
#define ALTER_TASK_PRIO		2 //
#define ALTER_STK_SIZE		256
TaskHandle_t ALTER_Task_Handler;
void ALTER_Task(void *pvParameters);

//时钟任务
#define CLOCK_TASK_PRIO		1 //
#define CLOCK_STK_SIZE		256
TaskHandle_t CLOCK_Task_Handler;
void CLOCK_Task(void *pvParameters);


#define NET_COUNT	7			//错误计数

#define NET_TIME	60			//设定时间--单位秒

unsigned short timerCount = 0;	//时间计数--单位秒


TimerHandle_t t1_Thdl;


char myTime[24];

//数据流
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
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
/*
void Hardware_Init(void)
{
	prvSetupHardware();
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);								//中断控制器分组设置

	Delay_Init();																//Timer4初始化
	
	Led_Init();																//LED初始化
	
	DIO_Init();
	
	Usart1_Init(115200); 														//初始化串口   115200bps
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alterInfo.alterBuf, sizeof(alterInfo.alterBuf), USART_RX_TYPE);
#endif
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//提示初始化完成

}*/

void Hardware_Init(void)
{
	prvSetupHardware();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//中断控制器分组设置

	Delay_Init();																//Timer4初始化初始化
	
	Led_Init();																	//LED初始化
	
	//IIC_Init();																	//软件IIC总线初始化
	
	Usart1_Init(115200); 														//初始化串口   115200bps
	UsartPrintf(USART_DEBUG, "INFO:	Usart1 Init.\r\n");
#if(USART_DMA_RX_EN)
	USARTx_ResetMemoryBaseAddr(USART_DEBUG, (unsigned int)alterInfo.alterBuf, sizeof(alterInfo.alterBuf), USART_RX_TYPE);
#endif
	
	RTC_Init();																	//初始化RTC
	
	//Check_PowerOn(); 															//上电自检

	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//如果是看门狗复位则提示
	{
		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
		
		RCC_ClearFlag();														//清除看门狗复位标志位
		
		net_fault_info.net_fault_level = net_fault_info.net_fault_level_r
														= NET_FAULT_LEVEL_5;	//错误等级5
		
		netDeviceInfo.reboot = 1;
		
		if(!Info_Check() && checkInfo.EEPROM_OK)								//如果EEPROM里有信息
			Info_Read();
	}
	else
	{
		//先读出ssid、pswd、devid、apikey
		if(!Info_Check() && checkInfo.EEPROM_OK)								//如果EEPROM里有信息
		{
			//AT24C02_Clear(0, 255, 256);Iwdg_Feed();
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in EEPROM\r\n");
			Info_Read();
		}
		else //没有数据
		{
			UsartPrintf(USART_DEBUG, "1.ssid_pswd in ROM\r\n");
		}
		
		UsartPrintf(USART_DEBUG, "2.DEVID: %s,     APIKEY: %s\r\n"
								, oneNetInfo.devID, oneNetInfo.apiKey);
		
		netDeviceInfo.reboot = 0;
	}
	
	Iwdg_Init(4, 1250); 														//64分频，每秒625次，重载1250次，2s
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//提示初始化完成

}


/*
************************************************************
*	函数名称：	OS_TimerCallBack
*
*	函数功能：	定时检查网络状态标志位
*
*	入口参数：	软件定时器句柄
*
*	返回参数：	无
*
*	说明：		定时器任务。定时检查网络状态，若持续超过设定时间无网络连接，则进行平台重连
************************************************************
*/
void OS_TimerCallBack(TimerHandle_t xTimer)
{
	
	if(oneNetInfo.errCount >= NET_COUNT)								//如果发送错误计数达到NET_COUNT次
	{
		UsartPrintf(USART_DEBUG, "Tips:	Timer Check Err-Send\r\n");
		
		oneNetInfo.errCount = 0;
		
		net_fault_info.net_fault_level = NET_FAULT_LEVEL_1;				//错误等级1
	}
	
	if(oneNetInfo.netWork == 0)											//如果在规定时间内网络还未接入成功
	{
		if(++timerCount >= NET_TIME) 									//如果网络断开超时
		{
			UsartPrintf(USART_DEBUG, "Tips:	Timer Check Err-Init\r\n");
			
			timerCount = 0;
			
			net_fault_info.net_fault_level = NET_FAULT_LEVEL_3;			//错误等级3
		}
	}
	else
	{
		timerCount = 0;													//清除计数
	}

}

int main(void)
{	
	Hardware_Init();
	
	//创建应用任务
	
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
	
	UsartPrintf(USART_DEBUG, "4.OSStart\r\n");		//提示任务开始执行
	
	/*
	xTaskCreate((TaskFunction_t)LED1_Task, "LED1", LED1_STK_SIZE, NULL, LED1_TASK_PRIO, (TaskHandle_t*)&LED1_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED2_Task, "LED2", LED2_STK_SIZE, NULL, LED2_TASK_PRIO, (TaskHandle_t*)&LED2_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED3_Task, "LED3", LED3_STK_SIZE, NULL, LED3_TASK_PRIO, (TaskHandle_t*)&LED3_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED4_Task, "LED4", LED4_STK_SIZE, NULL, LED4_TASK_PRIO, (TaskHandle_t*)&LED4_Task_Handler);
	
	xTaskCreate((TaskFunction_t)LED5_Task, "LED5", LED5_STK_SIZE, NULL, LED5_TASK_PRIO, (TaskHandle_t*)&LED5_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DO1_Task, "DO1", DO1_STK_SIZE, NULL, DO1_TASK_PRIO, (TaskHandle_t*)&DO1_Task_Handler);
	
	xTaskCreate((TaskFunction_t)DO2_Task, "DO2", DO2_STK_SIZE, NULL, DO2_TASK_PRIO, (TaskHandle_t*)&DO2_Task_Handler);
	*/
	vTaskStartScheduler();							//开始任务调度
	
}

/*
************************************************************
*	函数名称：	IWDG_Task
*
*	函数功能：	清除看门狗
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		看门狗任务
************************************************************
*/
void IWDG_Task(void *pdata)
{

	while(1)
	{
	
		Iwdg_Feed(); 		//喂狗
		
		RTOS_TimeDly(50); 	//挂起任务250ms
	
	}

}

/*
************************************************************
*	函数名称：	HEART_Task
*
*	函数功能：	心跳检测
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		心跳任务。发送心跳请求并等待心跳响应，若在设定时间内没有响应则会进行平台重连
************************************************************
*/
void STATUS_Task(void *pdata)
{

	while(1)
	{
		
		OneNet_Status();				//状态检测
		
		RTOS_TimeDly(4000);				//挂起任务20s
	
	}

}

/*
************************************************************
*	函数名称：	SEND_Task
*
*	函数功能：	上传传感器数据
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		数据发送任务
************************************************************
*/
void SEND_Task(void *pdata)
{

	while(1)
	{
		
		oneNetInfo.sendData = SEND_TYPE_DATA;
		
		RTOS_TimeDly(3000);							//挂起任务15s
		
	}

}

/*
************************************************************
*	函数名称：	USART_Task
*
*	函数功能：	处理平台下发的命令
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		串口接收任务。在数据模式下时，等待平台下发的命令并解析、处理
************************************************************
*/
void USART_Task(void *pdata)
{
	
	while(1)
	{
		
		OneNET_CmdHandle();
		
		RTOS_TimeDly(2);		//挂起任务10ms
	
	}

}

/*
************************************************************
*	函数名称：	DATA_Task
*
*	函数功能：	平台下发命令的数据反馈
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		数据反馈任务。这是平台下发指令后的反馈函数，透传模式的时候收到之后立即返回，非透传模式因为需要检索'>'符号，所以使用任务的方式来反馈。
************************************************************
*/
void DATA_Task(void *pdata)
{

	while(1)
	{
	
		switch(oneNetInfo.sendData)
		{
			case SEND_TYPE_DATA:
				oneNetInfo.sendData = OneNet_SendData(FORMAT_TYPE3, oneNetInfo.devID, oneNetInfo.apiKey, dataStream, dataStreamCnt);	//上传数据到平台
			break;
			
			case SEND_TYPE_REGISTER:
				oneNetInfo.sendData = OneNet_RegisterDevice("your Master-Key", "DeviceName", "auth_info", "desc", "false");
			break;
		}
		
		RTOS_TimeDly(10);					//挂起任务50ms
	
	}

}

/*
************************************************************
*	函数名称：	DATALIST_Task
*
*	函数功能：	循环发送链表里边待发送的数据块
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		
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
	
		RTOS_TimeDly(100);					//挂起任务500ms
	
	}

}

/*
************************************************************
*	函数名称：	FAULT_Task
*
*	函数功能：	网络状态错误处理
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		故障处理任务。当发生网络错误、设备错误时，会标记对应标志位，然后集中进行处理
************************************************************
*/
void FAULT_Task(void *pdata)
{

	while(1)
	{
		
		if(net_fault_info.net_fault_level != NET_FAULT_LEVEL_0)					//如果错误标志被设置
		{
			UsartPrintf(USART_DEBUG, "WARN:	NET Fault Process\r\n");
			
			NET_Fault_Process();												//进入错误处理函数
		}
		
		RTOS_TimeDly(10);														//挂起任务50ms
	
	}

}

/*
************************************************************
*	函数名称：	NET_Task
*
*	函数功能：	网络连接、平台接入
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		网络连接任务任务。会在心跳检测里边检测网络连接状态，如果有错，会标记状态，然后在这里进行重连
************************************************************
*/
void NET_Task(void *pdata)
{
	
	NET_DEVICE_IO_Init();													//网络设备IO初始化
	NET_DEVICE_Reset();														//网络设备复位

	while(1)
	{
		
		if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))		//当没有网络 且 网络模块检测到时
		{
			if(!NET_DEVICE_Init(oneNetInfo.protocol, oneNetInfo.ip, oneNetInfo.port))//初始化网络设备，能连入网络
			{
				UsartPrintf(USART_DEBUG, "Tips:	NetWork OK\r\n");
		
				oneNetInfo.netWork = 1;
				
				//if(gps.flag)
				//	dataStream[7].flag = 1;									//GPS就绪，准备上传
			}
		}
		
		if(checkInfo.NET_DEVICE_OK == DEV_ERR) 								//当网络设备未做检测
		{
			if(!NET_DEVICE_Exist())											//网络设备检测
			{
				UsartPrintf(USART_DEBUG, "NET Device :Ok\r\n");
				checkInfo.NET_DEVICE_OK = DEV_OK;							//检测到网络设备，标记
			}
			else
				UsartPrintf(USART_DEBUG, "NET Device :Error\r\n");
		}
		
		RTOS_TimeDly(5);													//挂起任务25ms
	
	}

}

/*
************************************************************
*	函数名称：	ALTER_Task
*
*	函数功能：	通过串口更改SSID、PSWD、DEVID、APIKEY
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		更改后会保存到EEPROM里
************************************************************
*/
void ALTER_Task(void *pdata)
{

    while(1)
    {
    
        memset(alterInfo.alterBuf, 0, sizeof(alterInfo.alterBuf));
		
		while(!alterInfo.rev_idle)
            RTOS_TimeDly(20);														//每100ms检查一次
		
		alterInfo.rev_idle = 0;
		
        UsartPrintf(USART_DEBUG, "\r\nAlter Rev\r\n%s\r\n", alterInfo.alterBuf);
        
		if(checkInfo.EEPROM_OK == DEV_OK)											//如果EEPROM存在
		{
			if(Info_Alter(alterInfo.alterBuf))										//更改信息
			{
				oneNetInfo.netWork = 0;												//重连网络和平台
				NET_DEVICE_ReConfig(0);
			}
		}
    
    }

}

/*
************************************************************
*	函数名称：	CLOCK_Task
*
*	函数功能：	网络校时、时间显示
*
*	入口参数：	void类型的参数指针
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void CLOCK_Task(void *pvParameter)
{
	
#if(NET_TIME_EN == 1)
	unsigned int second = 0, second_pre = 0, err_count = 0;				//second是实时时间，second_pre差值比较。err_count获取计时
	struct tm *time;
	_Bool get_net_time = 1;
#endif

	while(1)
	{
	
#if(NET_TIME_EN == 1)
		if(get_net_time)												//需要获取时间
		{
			dataStream[6].flag = 0;										//不上传时间
			
			if(++err_count >= 6000)										//十分钟还获取不到则重新获取
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
				{														//如果在±5分钟内，则认为时间正确
					RTC_SetTime(netDeviceInfo.net_time + 4);			//设置RTC时间，加4是补上大概的时间差
					
					get_net_time = 0;
					err_count = 0;
					
					dataStream[6].flag = 1;								//上传时间
				}
			}
		}

		second = RTC_GetCounter();										//获取秒值
		
		if(second > second_pre)
		{
			second_pre = second;
			time = localtime((const time_t *)&second);					//将秒值转为tm结构所表示的时间
			
			memset(myTime, 0, sizeof(myTime));
			snprintf(myTime, sizeof(myTime), "%d-%d-%d %d:%d:%d",
							time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
							time->tm_hour, time->tm_min, time->tm_sec);
		
			if(time->tm_hour == 0 && time->tm_min == 0 && time->tm_sec == 0)//每天0点时，更新一次时间
			{
				get_net_time = 1;
				netDeviceInfo.net_time = 0;
				oneNetInfo.netWork = 0;
				NET_DEVICE_ReConfig(0);
			}
		}
#endif
		
		RTOS_TimeDly(20);													//挂起任务100ms
	
	}

}



/*
void LED1_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led1_Set(LED_ON);
		
		//vTaskDelay(200);
		RTOS_TimeDly(400);						//挂起任务15s
		
		Led1_Set(LED_OFF);
		
		RTOS_TimeDly(400);						//挂起任务15s
		
	}

}

void LED2_Task(void *pvParameters)
{
	
	while(1)
	{
		
		Led2_Set(LED_ON);
		
		RTOS_TimeDly(200);						//挂起任务1s
		
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


