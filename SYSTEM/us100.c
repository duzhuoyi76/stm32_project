#include "stdio.h"
#include "us100.h"
#include "uart.h"
#include "delay.h"
#include <stdio.h>

uint8_t Serial_RxFlag = 0;
uint8_t US100_Date[2] = {0};

#define BUFFER_SIZE 64
volatile uint8_t rxBuffer[BUFFER_SIZE];
volatile uint8_t rxHead = 0;
volatile uint8_t rxTail = 0;
volatile uint8_t isTemperatureRequest = 0;
volatile uint8_t temperatureData = 0;

void us100_init(uint32_t BaudRate)//PA2,PA3
{
	usart2_init(BaudRate);
	//printf("us100_init\n");
}

void US100_Cmd(uint16_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	//printf("us100_cmd\n");
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        uint8_t data = USART_ReceiveData(USART2);

        if (isTemperatureRequest)
        {
            temperatureData = data;
            isTemperatureRequest = 0;
        }
        else
        {
            uint8_t nextHead = (rxHead + 1) % BUFFER_SIZE;
            if (nextHead != rxTail) 
						{
                rxBuffer[rxHead] = data;
                rxHead = nextHead;
            }
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

uint16_t US100_GetDate(void)
{
    US100_Cmd(0x55);

    while ((rxHead - rxTail + BUFFER_SIZE) % BUFFER_SIZE < 2);

    uint8_t highByte = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % BUFFER_SIZE;
    uint8_t lowByte = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % BUFFER_SIZE;

    uint16_t distance = (highByte << 8) | lowByte;
    return distance;
}

uint8_t Get_Temperature(void)
{
    isTemperatureRequest = 1;
    USART_SendData(USART2, 0x50);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    while(isTemperatureRequest); // Wait until temperature is received
    
    int actualTemperature = temperatureData - 45;

    return actualTemperature;
}

