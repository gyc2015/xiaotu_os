#ifndef STM32F407_H
#define STM32F407_H

#include <cortex_m4.h>

#include <stm32f407_exti.h>
#include <stm32f407_flash.h>
#include <stm32f407_gpio.h>
#include <stm32f407_rcc.h>
#include <stm32f407_syscfg.h>
#include <stm32f407_usart.h>

#define HSI_VALUE    ((uint32)16000000) /* �ڲ�����RC��Ƶ�� Hz*/
#define HSE_VALUE    ((uint32)24000000) /* �ⲿʱ��Ƶ�� Hz */
// ϵͳʱ��Ƶ��Hz
#define SYSCLK_HZ ((uint32)168000000)
// AHB����ʱ��Ƶ��Hz
#define HCLK_HZ ((uint32)168000000)
// APB1(����)����ʱ��Ƶ��Hz
#define PCLK1_HZ ((uint32)84000000)
// APB2(����)����ʱ��Ƶ��Hz
#define PCLK2_HZ ((uint32)42000000)

#endif
