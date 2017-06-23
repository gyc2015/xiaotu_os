/***********************************************************
 *
 * led.h - 三色灯驱动
 *
 ************************************** 高乙超.2017.0201 ***/
#pragma once

#include <stm32f407_gpio.h>

#define LED_0 PEout(3)
#define LED_1 PEout(4)
#define LED_2 PGout(9)
#define BEEP PGout(7)

#define LED_ON 0
#define LED_OFF 1
#define BEEP_ON 1
#define BEEP_OFF 0

/*
 * led_init - 初始化三色灯
 */
void led_init(void);
void beep_init(void);

