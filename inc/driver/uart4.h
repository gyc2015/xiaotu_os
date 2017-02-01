#ifndef UART4_H
#define UART4_h

#include <stm32f407_usart.h>

void uart4_init(uint32 baudrate);
uint8 uart4_send_byte(uint8 value);

#endif
