#ifndef WWDG_H
#define WWDG_H

#include <stm32f407_wwdg.h>

void wwdg_init(uint8 div, uint8 w, uint8 s);
void wwdg_feed(void);

#endif
