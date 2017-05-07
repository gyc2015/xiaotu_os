#ifndef UART4_H
#define UART4_h

#include <stm32f407_usart.h>

void uart4_init(uint32 baudrate);
void uart4_send_byte(uint8 value);
void uart4_send_bytes(const uint8 *buf, uint32 len);
void uart4_send_str(const uint8 *str);

#endif
