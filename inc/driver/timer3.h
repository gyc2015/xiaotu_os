#pragma once

#include <stm32f407_timer.h>

/*
* tim3_init - 初始化TIM3, 挂在慢速总线APB1上,驱动频率最高为42MHz
*
* @pres: 预分频系数
* @period: 计数周期
*/
void tim3_init(uint16 pres, uint16 period);

