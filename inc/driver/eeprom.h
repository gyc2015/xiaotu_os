#pragma once

#include <fakei2c.h>

void eeprom_init(void);
void eeprom_write_bytes(const uint8 *buf, uint8 len, uint8 reg);
void eeprom_read_bytes(uint8 *buf, uint8 len, uint8 reg);
