
//单片机头文件
#include "stm32f10x.h"

//LED头文件
#include "led.h"




LED_STATUS ledStatus;

/*
************************************************************
*	函数名称：	Led_Init
*
*	函数功能：	LED初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		LED1-PB8	LED2-PB9
				高电平开灯		低电平关灯
************************************************************
*/
void Led_Init(void)
{
	
	GPIO_InitTypeDef gpioInitStrcut;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	
//	gpioInitStrcut.GPIO_Pin = GPIO_Pin_5;
//	GPIO_Init(GPIOA, &gpioInitStrcut);
	
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &gpioInitStrcut);
    
    Led1_Set(LED_OFF);
    Led2_Set(LED_OFF);

}

/*
************************************************************
*	函数名称：	Ledx_Set
*
*	函数功能：	LEDx控制
*
*	入口参数：	status：LED_ON-开灯	LED_OFF-关灯
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/

void Led1_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_8, status != LED_OFF ? Bit_SET : Bit_RESET);
	ledStatus.Led1Sta = status;

}

void Led2_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOB, GPIO_Pin_9, status != LED_OFF ? Bit_SET : Bit_RESET);
	ledStatus.Led2Sta = status;

}

