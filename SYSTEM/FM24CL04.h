#ifndef  __FM24CL04_H__
#define	 __FM24CL04_H__
#include "stm32f4xx.h"
void IIC_Init(void);
void IIC_AT24C02_BufferRead(uint8_t *pBuffer,uint8_t ReadAddr,uint8_t NumByte);
void IIC_Write_AT24C02_OneByte(uint8_t addr,uint8_t data);
void IIC_AT24C02_BufferWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite );
#endif 

