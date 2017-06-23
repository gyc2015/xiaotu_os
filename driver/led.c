/***********************************************************
 *
 * led.c - ��ɫ������
 *
 ************************************** ���ҳ�.2017.0201 ***/
#include <stm32f407.h>
#include <led.h>
#include <stm32f407_timer.h>
/*
 * led_init - ��ʼ����ɫ��
 */
void led_init(void) {
    RCC->AHB1ENR.bits.gpioe = 1;
    RCC->AHB1ENR.bits.gpiog = 1;

    GPIOE->MODER.bits.pin3 = GPIO_Mode_Out;
    GPIOE->MODER.bits.pin4 = GPIO_Mode_Out;
    GPIOG->MODER.bits.pin9 = GPIO_Mode_Out;

    GPIOE->OTYPER.bits.pin3 = GPIO_OType_PP;
    GPIOE->OTYPER.bits.pin4 = GPIO_OType_PP;
    GPIOG->OTYPER.bits.pin9 = GPIO_OType_PP;

    GPIOE->PUPDR.bits.pin3 = GPIO_Pull_Up;
    GPIOE->PUPDR.bits.pin4 = GPIO_Pull_Up;
    GPIOG->PUPDR.bits.pin9 = GPIO_Pull_Up;

    GPIOE->OSPEEDR.bits.pin3 = GPIO_OSpeed_Very_High;
    GPIOE->OSPEEDR.bits.pin4 = GPIO_OSpeed_Very_High;
    GPIOG->OSPEEDR.bits.pin9 = GPIO_OSpeed_Very_High;

    LED_0 = LED_OFF;
    LED_1 = LED_OFF;
    LED_2 = LED_OFF;
}
