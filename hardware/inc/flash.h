#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define FLASH_SIZE 512          //MCU��FLASH��С(��λK)

#if FLASH_SIZE < 256
	#define SECTOR_SIZE           1024    //������С���ֽ�
#else 
	#define SECTOR_SIZE           2048    //�ֽ�
#endif



void FLASH_ReadMoreData(uint32_t startAddress, uint16_t *readData, uint16_t countToRead);

uint16_t FLASH_ReadHalfWord(uint32_t address);

uint32_t FLASH_ReadWord(uint32_t address);

void FLASH_WriteMoreData(uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite);

#endif
