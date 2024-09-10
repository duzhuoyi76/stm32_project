#ifndef		__IO_I2C_H__
#define		__IO_i2c_H__
#include "stm32f4xx.h"
void i2c_init(void);
void SDA_IO_ModeConfig(GPIOMode_TypeDef IO_Mode);
void i2c_start(void);
void i2c_stop(void);
void i2c_send_byte(uint8_t c);
uint8_t i2c_recv_byte(void);
void i2c_send_ack(uint8_t ack);
uint8_t i2c_wait_ack(void);



#endif 

