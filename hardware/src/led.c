
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//LEDͷ�ļ�
#include "led.h"




LED_STATUS ledStatus;

/*
************************************************************
*	�������ƣ�	Led_Init
*
*	�������ܣ�	LED��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		LED1-PB8	LED2-PB9
				�ߵ�ƽ����		�͵�ƽ�ص�
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
*	�������ƣ�	Ledx_Set
*
*	�������ܣ�	LEDx����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
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

