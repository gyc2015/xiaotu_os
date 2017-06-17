/***********************************************************
 *
 * led.h - 三色灯驱动
 *
 ************************************** 高乙超.2017.0201 ***/
#ifndef LED_H
#define LED_H

#include <stm32f407_gpio.h>

#define LED_R PIout(5)
#define LED_G PIout(6)
#define LED_B PIout(7)

#define LED_ON 0
#define LED_OFF 1

/*
 * init_led - 初始化三色灯
 */
void led_init(void);

void led_pwm_init(uint8 r, uint8 g, uint8 b);

/*
* led_set_color - 设置灯光颜色
*/
void led_set_color(uint8 r, uint8 g, uint8 b);

#endif // !LED_H
