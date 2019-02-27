#ifndef _AT24C32_H_
#define _AT24C32_H_







#define AT24C32_ADDRESS		0x50 //AT24C32µÿ÷∑




void AT24C32_WriteByte(unsigned short regAddr, unsigned char byte);

void AT24C32_WriteBytes(unsigned short regAddr, unsigned char *byte, unsigned char len);

void AT24C32_ReadByte(unsigned short regAddr, unsigned char *byte);

void AT24C32_ReadBytes(unsigned short regAddr, unsigned char *byte, unsigned char len);

_Bool AT24C32_Clear(unsigned short startAddr, unsigned char byte, unsigned short len);


#endif
