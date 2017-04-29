#ifndef IWDG_H
#define IWDG_H

#include <stm32f407_iwdg.h>

void iwdg_init(uint32 div, uint32 rl);
void iwdg_feed(void);

#endif
