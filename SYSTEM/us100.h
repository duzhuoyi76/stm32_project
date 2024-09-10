#ifndef __US100_H__
#define __US100_H__
#include "stm32f4xx.h"
#include "bitband.h"

void us100_init(uint32_t BaudRate);
uint8_t Serial_GetRxFlag(void);
void US100_Cmd(uint16_t Byte);
uint16_t US100_GetDate(void);
uint8_t Get_Temperature(void);
#endif
