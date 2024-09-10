#ifndef		__EEPROM_H__
#define		__EEPROM_H__
#include "stm32f4xx.h"
void eeprom_init(void);
void eeprom_write_byte(uint16_t addr, uint8_t data);
void eeprom_write_bytes(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_read_bytes(uint16_t addr, uint8_t *buf, uint8_t len);

#endif

