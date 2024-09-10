#include "io_i2c.h"
#include "bitband.h"
#include "delay.h"

#define		SCL		PBout(8)
#define		SDA_W	PBout(9)
#define		SDA_R	PBin(9)

void i2c_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SDA_IO_ModeConfig(GPIOMode_TypeDef IO_Mode)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = IO_Mode;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void i2c_start(void)
{
	SDA_IO_ModeConfig(GPIO_Mode_OUT);
	SDA_W = 1;
	SCL = 1;
	delay_us(4);
	SDA_W = 0;
	delay_us(4);
}

void i2c_stop(void)
{
	SDA_IO_ModeConfig(GPIO_Mode_OUT);
	SDA_W = 0;
	SCL = 1;
	delay_us(4);
	SDA_W = 1;
	delay_us(4);
}

//Master Send 
void i2c_send_byte(uint8_t c)
{
	SDA_IO_ModeConfig(GPIO_Mode_OUT);
	for(uint8_t i=0;i<8;i++)
	{
		SCL = 0;
		delay_us(4);
		SDA_W = !!(c&1<<(7-i));
		delay_us(4);
		SCL = 1;//锁存数据，接收方此时读取数据
		delay_us(4);
	}
	SCL = 0;
	SDA_W = 1;//释放总线，等待对方回复ACK
}

//Master Recv 
uint8_t i2c_recv_byte(void)
{
	SDA_IO_ModeConfig(GPIO_Mode_IN);
	uint8_t value=0;
	for(uint8_t i=0;i<8;i++)
	{
		SCL = 1;//锁存数据
		delay_us(4);
		value |= SDA_R<<(7-i);
		SCL = 0;//把SCL拉低，让从设备发数据
		delay_us(4);
	}
	return value;
}

void i2c_send_ack(uint8_t ack)
{
	SDA_IO_ModeConfig(GPIO_Mode_OUT);
	
	SDA_W = ack;
	delay_us(4);
	SCL = 1;//锁存数据，让从设备读ACK
	delay_us(4);
	SCL = 0;
	delay_us(4);
	SDA_W = 1;//释放总线
}

uint8_t i2c_wait_ack(void)
{
	SDA_IO_ModeConfig(GPIO_Mode_IN);
	uint8_t re;
	
	SDA_W = 1;//释放总线，让从设备回复ACK
	delay_us(4);
	SCL = 1;//锁存数据 
	delay_us(4);
	re = SDA_R;
	SCL = 0;//对方回来ACK后，可能会继续给我们发数据
	delay_us(4);
	
	return re;
}


