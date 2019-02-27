
//单片机头文件
#include "stm32f10x.h"

//LED头文件
#include "dio.h"



DIO_STATUS dioStatus;

//	# warning "FAN SELECT"
const unsigned char DO_Pins = 2;
const unsigned char DI_Pins = 2;
DIO_PINS DO_Pin[DO_Pins] = {{GPIOA, GPIO_Pin_5}, \
							{GPIOA, GPIO_Pin_6}};
DIO_PINS DI_Pin[DI_Pins] = {{GPIOA, GPIO_Pin_7}, \
							{GPIOB, GPIO_Pin_0}};


/*
************************************************************
*	函数名称：	DIO_Init
*
*	函数功能：	LED初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		DO1-P5	DO2-PA6	DI1-PA7	DI2-PB0
				高电平闭合		低电平断开
************************************************************
*/
void DIO_Init(void)
{
	
	GPIO_InitTypeDef gpioInitStrcut;
	unsigned char i = 0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	// DO Pins
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	for(i = 0; i < DO_Pins; i++)
	{
		gpioInitStrcut.GPIO_Pin = DO_Pin[i].Pin;
		GPIO_Init(DO_Pin[i].GPIOx, &gpioInitStrcut);
	}
	
	// DI Pins
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_IPU;
	for(i = 0; i < DI_Pins; i++)
	{
		gpioInitStrcut.GPIO_Pin = DI_Pin[i].Pin;
		GPIO_Init(DI_Pin[i].GPIOx, &gpioInitStrcut);
	}
	
	/*
	// DO Pins
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &gpioInitStrcut);
	
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &gpioInitStrcut);
	
	// DI Pins
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_IPU;
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpioInitStrcut);
	*/
	
}

void DO_Set(unsigned char pin, DIO_ENUM status)
{
	if(pin >= DO_Pins) return;
	
	GPIO_WriteBit(DO_Pin[pin].GPIOx, DO_Pin[pin].Pin, status != DIO_OFF ? Bit_SET : Bit_RESET);
}

unsigned short DI_Read(unsigned char pin)
{
	unsigned short result = 0;
	
	if(pin >= DI_Pins) return result;
	
	result = GPIO_ReadInputDataBit(DI_Pin[pin].GPIOx, DI_Pin[pin].Pin);
	
	return result;
}

