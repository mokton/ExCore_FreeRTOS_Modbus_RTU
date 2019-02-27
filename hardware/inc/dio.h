#ifndef _DIO_H_
#define _DIO_H_

#include "stm32f10x.h"
#include "device.h"

typedef struct
{
	_Bool DO1Sta;
	_Bool DO2Sta;
	_Bool DI1Sta;
	_Bool DI2Sta;

} DIO_STATUS;

extern DIO_STATUS dioStatus;

typedef struct
{
	GPIO_TypeDef* 	GPIOx;
	uint16_t 		Pin;
} DIO_PINS;

extern DIO_PINS DO_Pin[];

extern DIO_PINS DI_Pin[];

extern const unsigned char DO_Pins;

extern const unsigned char DI_Pins;

typedef enum
{

	DIO_OFF = 0,
	DIO_ON

} DIO_ENUM;

void DIO_Init(void);
void DO_Set(unsigned char pin, DIO_ENUM status);
unsigned short DI_Read(unsigned char pin);


//void DO1_Set(DIO_ENUM status);
//void DO2_Set(DIO_ENUM status);
//unsigned char DI1_Read(void);
//unsigned char DI2_Read(void);

#endif
