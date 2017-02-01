/***********************************************************
 *
 * led.h - 三色灯驱动
 *
 ************************************** 高乙超.2017.0201 ***/
#ifndef LED_H
#define LED_H

#include <stm32f407_gpio.h>

#define LED_R (GPIOI->ODR.bits.pin5)
#define LED_G (GPIOI->ODR.bits.pin6)
#define LED_B (GPIOI->ODR.bits.pin7)

#define LED_ON 0
#define LED_OFF 1

/*
 * init_led - 初始化三色灯
 */
void led_init(void);

#endif // !LED_H
