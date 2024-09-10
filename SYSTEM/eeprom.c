#include "eeprom.h"
#include "io_i2c.h"
#include "stdio.h"
#include "delay.h"

void eeprom_init(void)
{
	i2c_init();
}

void eeprom_write_byte(uint16_t addr, uint8_t data)
{
	i2c_start();//发送一个start信号
	i2c_send_byte(0xA0+addr/256*2);//发送一个器件写地址(0xA0,0xA2)
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom send byte error: 1\r\n");
		return;
	}	
	
	i2c_send_byte(addr%256);//发送一个字地址(希望把数据写到哪个位置)
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom send byte error: 2\r\n");
		return;
	}	
	
	i2c_send_byte(data);//发送想要写入的数据 data
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom send byte error: 3\r\n");
		return;
	}	
															
	i2c_stop();//发送一个stop信号
}

void eeprom_write_bytes(uint16_t addr, uint8_t *buf, uint8_t len)
{
	for(uint8_t i=0;i<len;i++)
	{
		eeprom_write_byte(addr+i, buf[i]);
		delay_ms(10);
	}
}

void eeprom_read_bytes(uint16_t addr, uint8_t *buf, uint8_t len)
{
	i2c_start();//发送一个start信号
	i2c_send_byte(0xA0+addr/256*2);//发送一个器件写地址(0xA0,0xA2)
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom read bytes error: 1\r\n");
		return;
	}	
	
	i2c_send_byte(addr%256);//发送一个字地址(希望从哪个位置读取数据)
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom read bytes error: 2\r\n");
		return;
	}	

	i2c_start();//发送一个start信号
	i2c_send_byte(0xA1+addr/256*2);//发送一个器件读地址(0xA1,0xA3)
	if(i2c_wait_ack())		//等待ACK
	{
		printf("eeprom read bytes error: 3\r\n");
		return;
	}	

	for(uint8_t i=0;i<len;i++)
	{
		buf[i] = i2c_recv_byte();
		if(i == len-1)
		{
			i2c_stop();
		}
		else
		{
			i2c_send_ack(0);
		}
	}
	
}

