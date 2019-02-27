//单片机头文件
#include "stm32f10x.h"

//OS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

//Hardware
#include "delay.h"
#include "iwdg.h"
#include "led.h"
#include "dio.h"
#include "flash.h"
#include "rtc.h"

//Modbus
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"
#include "portfunction.h"
//#include "ModbusMaster.h"

//User
#include "systime.h"
#include "device.h"




#define FreeModbus_TASK_PRIO    ( tskIDLE_PRIORITY + 2 )
static void vMBTask(void *pvParameters);


//void assert_failed(uint8_t* file, uint32_t line);

/* Private macro -------------------------------------------------------------*/
/*
#define REG_INPUT_START 0000
#define REG_INPUT_NREGS 8

#define REG_HOLDING_START 0x4700  // 513
#define REG_HOLDING_NREGS 8

#define REG_COILS_START 2000
#define REG_COILS_NREGS 8

#define REG_DISCRETE_START 3000
#define REG_DISCRETE_NREGS 8
*/
/* Private variables ---------------------------------------------------------*/
uint16_t usRegInputStart = REG_INPUT_START;
uint16_t usRegInputBuf[REG_INPUT_NREGS] = \
{0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0 \
};
uint16_t usRegHoldingStart = REG_HOLDING_START;
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = \
{0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0, \
0,0,0,0,0,0,0,0 \
};



/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
//static void prvSetupHardware( void );

unsigned char usart1Buf[MAXLENGTH];
unsigned char usart1Len;
//unsigned char usart2Buf[MAXLENGTH];
//unsigned char usart2Len;
//unsigned char usart3Buf[MAXLENGTH];
//unsigned char usart3Len;
//unsigned char esp8266_buf[MAXLENGTH];
//unsigned short esp8266_cnt, esp8266_cntPre;


// flash address & data
const uint16_t flash_len = 10; // bytes
uint16_t flash_value[flash_len] = {0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff};
uint32_t flash_addr = 0x08000000 + SECTOR_SIZE * 96;


//任务

//看门狗任务
#define IWDG_TASK_PRIO		( tskIDLE_PRIORITY + 12 )
#define IWDG_STK_SIZE		64
TaskHandle_t IWDG_Task_Handler;
void IWDG_Task(void *pvParameters);

#define TIME_TASK_PRIO		( tskIDLE_PRIORITY + 11 )
#define TIME_STK_SIZE		128
TaskHandle_t TIME_Task_Handler;
void TIME_Task(void *pvParameters);

#define LED1_TASK_PRIO		( tskIDLE_PRIORITY + 5 )
#define LED1_STK_SIZE		64
TaskHandle_t LED1_Task_Handler;
void LED1_Task(void *pvParameters);

#define LED2_TASK_PRIO		( tskIDLE_PRIORITY + 4 )
#define LED2_STK_SIZE		64
TaskHandle_t LED2_Task_Handler;
void LED2_Task(void *pvParameters);

#define DIO_TASK_PRIO		( tskIDLE_PRIORITY + 3 )
#define DIO_STK_SIZE		256
TaskHandle_t DIO_Task_Handler;
void DIO_Task(void *pvParameters);

#define USART_TASK_PRIO		( tskIDLE_PRIORITY + 2 )
#define USART_STK_SIZE		256
TaskHandle_t USART_Task_Handler;
void USART_Task(void *pvParameters);

#define MASTER_TASK_PRIO	( tskIDLE_PRIORITY + 1 )
#define MASTER_STK_SIZE		256
TaskHandle_t MASTER_Task_Handler;
void MASTER_Task(void *pvParameters);



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
void Hardware_Init(void)
{
//	prvSetupHardware();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);								//中断控制器分组设置

	Delay_Init();																//Timer4初始化
	
	Led_Init();																//LED初始化
	
	DIO_Init();
	
	RTC_Init();																	//初始化RTC
	
	Usart1_Init(115200); 														//初始化串口   115200bps
	
	//Usart2_Init(9600);
	
	//Usart3_Init(9600);
	
//	UsartPrintf(USART1, "%x", 0);
//	UsartPrintf(USART2, "%x", 0);
//	UsartPrintf(USART3, "%x", 0);
	//UsartPrintf(USART2, "\nUsart2 System start\r\n");
	//RS485Printf("\nRS485 System start\r\n");
	
	UsartPrintf(USART_DEBUG, "\nHardware init okay\r\n");							//提示初始化完成
	UsartPrintf(USART_DEBUG, "System Clock HZ %d\r\n", configCPU_CLOCK_HZ);
	UsartPrintf(USART_DEBUG, "System start\r\n");
	//Iwdg_Init(4, 3125);
	Iwdg_Init(4, 1250); 											//64分频，每秒625次，重载1250次，2s
}



int main(void)
{	
	Hardware_Init();
	
	xTaskCreate((TaskFunction_t)IWDG_Task, "IWDG", IWDG_STK_SIZE, NULL, IWDG_TASK_PRIO, (TaskHandle_t*)&IWDG_Task_Handler);
	
	//xTaskCreate((TaskFunction_t)TIME_Task, "TIME", TIME_STK_SIZE, NULL, TIME_TASK_PRIO, (TaskHandle_t*)&TIME_Task_Handler);
	
	xTaskCreate(vMBTask, "FreeModbus", configMINIMAL_STACK_SIZE, NULL, FreeModbus_TASK_PRIO, NULL);

	xTaskCreate((TaskFunction_t)LED1_Task, "LED1", LED1_STK_SIZE, NULL, LED1_TASK_PRIO, (TaskHandle_t*)&LED1_Task_Handler);

	xTaskCreate((TaskFunction_t)LED2_Task, "LED2", LED2_STK_SIZE, NULL, LED2_TASK_PRIO, (TaskHandle_t*)&LED2_Task_Handler);

	//xTaskCreate((TaskFunction_t)USART_Task, "USART", USART_STK_SIZE, NULL, USART_TASK_PRIO, (TaskHandle_t*)&USART_Task_Handler);
	
	//xTaskCreate((TaskFunction_t)DIO_Task, "DIO", DIO_STK_SIZE, NULL, DIO_TASK_PRIO, (TaskHandle_t*)&DIO_Task_Handler);
	
	vTaskStartScheduler();							//开始任务调度
	
	//while(++i)
	//	UsartPrintf(USART_DEBUG, "--- %d ---\n", i);
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
void IWDG_Task(void *pvParameters)
{

	while(1)
	{
	
		Iwdg_Feed(); 		//喂狗
		
		RTOS_TimeDly(50); 	//挂起任务250ms
	
	}

}


void TIME_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200; // 200为1s，但经测试不准，此处改为134大概为1s
	uint32_t temp = 0;
	
	uint32_t secondCmp = 0;  // 秒比较值
	
	sys_minute = FLASH_ReadWord(flash_addr);
	sys_hour = FLASH_ReadWord(flash_addr + 4);
	
	if(sys_hour == 0xffffffff || sys_minute == 0xffffffff)
	{
		FLASH_WriteMoreData(flash_addr, flash_value, flash_addr);
		sys_minute = 0;
		sys_hour = 0;
	}
	
	// Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();//获取当前 tick

	while(1)
	{
	
//		sys_second++; 					// tick计算秒值
		secondCmp = RTC_GetCounter();  // RTC 获取秒值
		
		if(secondCmp > sys_second)
		{
			sys_second = secondCmp;
		
			if(sys_second % 60 == 0)
			{
				sys_minute++;
				
				if(sys_minute % 60 == 0) sys_hour++;
				
				flash_value[0] = sys_minute & 0x0000FFFF;  // 系统运行 分钟
				flash_value[1] = sys_minute >> 16;
				flash_value[2] = sys_hour & 0x0000FFFF;  // 系统运行 小时
				flash_value[3] = sys_hour >> 16;
				FLASH_WriteMoreData(flash_addr, flash_value, flash_len);
				// 保存所有寄存器数据到Flash
				FLASH_WriteMoreData(flash_addr + SECTOR_SIZE, usRegHoldingBuf, REG_HOLDING_NREGS);
			}
			
			UsartPrintf(USART_DEBUG, "[H:%d] [M:%d] [S:%d] %d, %d\r\n", sys_hour, sys_minute, sys_second, usRegHoldingBuf[22], usRegHoldingBuf[1]);
			
		}
		// 0X08000000
//		temp = FLASH_ReadWord(addr);
//		UsartPrintf(USART_DEBUG, "0 Flash: %08x\r\n", temp);
//		
//		tvalue[0] = sys_second & 0x0000FFFF;
//		tvalue[1] = sys_second >> 16;
//		FLASH_WriteMoreData(addr, tvalue, 6);
//		temp = FLASH_ReadWord(addr);
//		UsartPrintf(USART_DEBUG, "1 Flash: %08x\r\n", temp);
		
		
		UsartPrintf(USART_DEBUG, "Second count: %d\r\n", temp++);
		
		vTaskDelayUntil(&xLastWakeTime, xFrequency); 	//挂起任务1s
	
	}

}


static void vMBTask( void *pvParameters )
{
    /* Select either ASCII or RTU Mode. */
    eMBInit( MB_RTU, SALVE_ID, 0, 9600, MB_PAR_NONE );
    /* Enable the Modbus Protocol Stack. */
    eMBEnable();
    for( ;; )
    {
        /* Call the main polling loop of the Modbus protocol stack. Internally
         * the polling loop waits for a new event by calling the port
         * dependent function xMBPortEventGet(  ). In the FreeRTOS port the
         * event layer is built with queues.
         */
        ( void )eMBPoll();
		
        RTOS_TimeDly(2);
    }
}


void USART_Task(void *pvParameters)
{
//	unsigned char i = 0;
	while(1)
	{
		if(usartInfo.rev_idle)//usartInfo.count > 0
		{
			UsartPrintf(USART_DEBUG, "RS485: Received (%d)\r\n", usartInfo.count);
			usartInfo.rev_idle = 0;
			if(usartInfo.count > 0)
			{
				UsartPrintf(USART_DEBUG, "Data: %s\r\n", usartInfo.buf);
				//Modbus_Handler(usartInfo.buf, usartInfo.count);

				//if(mbInfo.s_id == 1)
				//	usRegHoldingBuf[30] = mbInfo.r_value[0];
				//else if(mbInfo.s_id == 2)
				//	usRegHoldingBuf[31] = mbInfo.r_value[0];

				usartInfo.count = 0;
				//MODBUS_FreeBuffer(mbInfo.r_value);
			}
		}
        RTOS_TimeDly(10);
	}
}


void LED1_Task(void *pvParameters)
{
	
	while(1)
	{
		
		if(ledStatus.Led1Sta == LED_OFF)
			Led1_Set(LED_ON);
		else
			Led1_Set(LED_OFF);
		
		RTOS_TimeDly(200);
		
	}

}

void LED2_Task(void *pvParameters)
{
	
	while(1)
	{
		
		if(ledStatus.Led2Sta == LED_OFF)
			Led2_Set(LED_ON);
		else
			Led2_Set(LED_OFF);
		
		RTOS_TimeDly(100);
		
	}

}


void DIO_Task(void *pvParameters)
{
	unsigned short i = 0;
	
	RTOS_TimeDly(200); // 等候1s初始化后，启动DI/DO逻辑
	
	for(i = 0; i < REG_HOLDING_NREGS; i++)
	{
		usRegHoldingBuf[i] = FLASH_ReadHalfWord(flash_addr + SECTOR_SIZE + i * 2);
		if(usRegHoldingBuf[i] == 0xffff) usRegHoldingBuf[i] = 0;
	}
	
	
	while(1)
	{   // xTaskGetTickCount()
//		if(run_cmd_time > sys_second) run_cmd_time = 0; // 计时溢出处理
//		if(stop_cmd_time > sys_second) stop_cmd_time = 0;
//		if(run_cmd_time2 > sys_second) run_cmd_time2 = 0;
//		if(stop_cmd_time2 > sys_second) stop_cmd_time2 = 0;
		
		// DI
		for(i = 0; i < DI_Pins; i++)
		{
			usRegHoldingBuf[i] = DI_Read(i) ? 0 : 1;
		}

		//DO
		for(i = 0; i < DO_Pins; i++)
		{
			DO_Set(i, usRegHoldingBuf[i + DI_Pins] & 0x0001 ? DIO_ON : DIO_OFF);
//			DO_Set(i, 1);
		}

		//LED
		Led1_Set(usRegHoldingBuf[DI_Pins + DO_Pins] & 0x0001 ? LED_ON : LED_OFF);
		Led2_Set(usRegHoldingBuf[DI_Pins + DO_Pins + 1] & 0x0001 ? LED_ON : LED_OFF);

		RTOS_TimeDly(10);  // 50ms 处理一次 DI/DO
	}
}




