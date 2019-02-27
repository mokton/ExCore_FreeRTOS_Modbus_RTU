#ifndef _LED_H_
#define _LED_H_



typedef struct
{

	_Bool Led1Sta;
	_Bool Led2Sta;

} LED_STATUS;

extern LED_STATUS ledStatus;

typedef enum
{

	LED_OFF = 0,
	LED_ON

} LED_ENUM;




void Led_Init(void);

void Led1_Set(LED_ENUM status);

void Led2_Set(LED_ENUM status);


#endif
