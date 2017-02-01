#ifndef STM32F407_H
#define STM32F407_H

#include <cortex_m4.h>

#include <stm32f407_exti.h>
#include <stm32f407_flash.h>
#include <stm32f407_gpio.h>
#include <stm32f407_rcc.h>
#include <stm32f407_syscfg.h>
#include <stm32f407_usart.h>

#define HSI_VALUE    ((uint32)16000000) /* 内部高速RC震荡频率 Hz*/
#define HSE_VALUE    ((uint32)24000000) /* 外部时钟频率 Hz */
// 系统时钟频率Hz
#define SYSCLK_HZ ((uint32)168000000)
// AHB总线时钟频率Hz
#define HCLK_HZ ((uint32)168000000)
// APB1(低速)总线时钟频率Hz
#define PCLK1_HZ ((uint32)84000000)
// APB2(高速)总线时钟频率Hz
#define PCLK2_HZ ((uint32)42000000)

#endif
