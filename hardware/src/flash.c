
#include "flash.h"

//#include "usart.h"
//#include "FreeRTOS.h"
//#include "task.h"

//从指定地址开始读取多个数据
void FLASH_ReadMoreData(uint32_t startAddress, uint16_t *readData, uint16_t countToRead)
{
  uint16_t dataIndex;
  for(dataIndex = 0; dataIndex<countToRead; dataIndex++)
  {
    readData[dataIndex] = FLASH_ReadHalfWord(startAddress + dataIndex * 2);
  }
}

//读取指定地址的半字(16位数据)
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
  return *(__IO uint16_t*)address; 
}

//读取指定地址的全字(32位数据)
uint32_t FLASH_ReadWord(uint32_t address)
{
  uint32_t temp1,temp2;
  temp1 = *(__IO uint16_t*)address;
  temp2 = *(__IO uint16_t*)(address + 2); 
  return (temp2 << 16) + temp1;
}

//从指定地址开始写入多个数据
void FLASH_WriteMoreData(uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite)
{
	uint32_t offsetAddress = 0;
	uint32_t sectorPosition = 0;
	uint32_t sectorStartAddress = 0;
	uint16_t dataIndex = 0;
	
//	TickType_t xLastWakeTime;
	
	if(startAddress < FLASH_BASE || ((startAddress+countToWrite * 2) >= (FLASH_BASE + 1024 * FLASH_SIZE)))
	{
		return;  //非法地址
	}
	
	FLASH_Unlock();         //解锁写保护
	
	offsetAddress = startAddress - FLASH_BASE;               //去掉计算0X08000000后的实际偏移地址
	sectorPosition = offsetAddress / SECTOR_SIZE;            //计算扇区地址,对于STM32F103VET6为0~255

	sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE;    //对应扇区的首地址

//	xLastWakeTime = xTaskGetTickCount();
	FLASH_ErasePage(sectorStartAddress);//擦除整个扇区
//	UsartPrintf(USART_DEBUG, "Erase Flash: %08d\r\n", xTaskGetTickCount() - xLastWakeTime);
	
	for(dataIndex=0; dataIndex < countToWrite; dataIndex++)
	{
		FLASH_ProgramHalfWord(startAddress+dataIndex * 2, writeData[dataIndex]);
	}

//	FLASH_Lock();//上写保护
}
