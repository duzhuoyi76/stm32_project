#ifndef  __UART_H__
#define  __UART_H__
#include "stm32f4xx.h"
#include "bitband.h"
void uart1_init( uint32_t BaudRate);
void usart2_init(uint32_t BaudRate);
void usart3_init(uint32_t BaudRate);
void USART_SendDatas(USART_TypeDef * USARTx, uint8_t *buf, uint8_t len);

#define  RS485_TX_EN()		{PGout(8)=1;}   //485模式控制，1发送，0接收
#define	 RS485_RX_EN()	  {PGout(8)=0;}
void rs485_init(uint32_t BaudRate);
#endif 

