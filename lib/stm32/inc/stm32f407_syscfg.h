/***********************************************************
 *
 * stm32f407_syscfg - ϵͳ������
 *
 * 0x4001 3800 - 0x4001 3BFF, APB2
 *
 ************************************** ���ҳ�.2017.0111 ***/
#ifndef STM32F407_SYSCFG_H
#define STM32F407_SYSCFG_H

#include <types.h>

enum syscfg_exti_port {
    Exti_PortSource_A = 0,
    Exti_PortSource_B = 1,
    Exti_PortSource_C = 2,
    Exti_PortSource_D = 3,
    Exti_PortSource_E = 4,
    Exti_PortSource_F = 5,
    Exti_PortSource_G = 6,
    Exti_PortSource_H = 7,
    Exti_PortSource_I = 8
};

struct syscfg_exticr_bits {
    uint32 exit0 : 4;
    uint32 exit1 : 4;
    uint32 exit2 : 4;
    uint32 exit3 : 4;
    uint32 r0 : 16;
    uint32 exit4 : 4;
    uint32 exit5 : 4;
    uint32 exit6 : 4;
    uint32 exit7 : 4;
    uint32 r1 : 16;
    uint32 exit8 : 4;
    uint32 exit9 : 4;
    uint32 exit10 : 4;
    uint32 exit11 : 4;
    uint32 r2 : 16;
    uint32 exit12 : 4;
    uint32 exit13 : 4;
    uint32 exit14 : 4;
    uint32 exit15 : 4;
    uint32 r3 : 16;
};

union syscfg_exticr {
    struct syscfg_exticr_bits bits;
    uint32 words[4];
};

typedef struct syscfg_regs {
    volatile uint32 MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
    volatile uint32 PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
    volatile union syscfg_exticr EXTICR;    /* �ⲿ�ж����üĴ���, offset: 0x08-0x14 */
    uint32 RESERVED[2];           /*!< Reserved, 0x18-0x1C                                                          */
    volatile uint32 CMPCR;        /*!< SYSCFG Compensation cell control register,         Address offset: 0x20      */
} syscfg_regs_t;


#define SYSCFG_BASE 0x40013800
#define SYSCFG ((syscfg_regs_t *) SYSCFG_BASE)



#endif // !STM32F407_SYSCFG_H
