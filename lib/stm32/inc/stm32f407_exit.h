/***********************************************************
 *
 * stm32f407_exit - �ⲿ�ж�
 *
 * 0x4001 3C00 - 0x4001 3FFF, APB2
 *
 ************************************** ���ҳ�.2017.0111 ***/
#ifndef STM32F407_EXIT_H
#define STM32F407_EXIT_H

#include <types.h>

struct exit_register_1bit {
    uint32 tr0 : 1;
    uint32 tr1 : 1;
    uint32 tr2 : 1;
    uint32 tr3 : 1;
    uint32 tr4 : 1;
    uint32 tr5 : 1;
    uint32 tr6 : 1;
    uint32 tr7 : 1;
    uint32 tr8 : 1;
    uint32 tr9 : 1;
    uint32 tr10 : 1;
    uint32 tr11 : 1;
    uint32 tr12 : 1;
    uint32 tr13 : 1;
    uint32 tr14 : 1;
    uint32 tr15 : 1;
    uint32 tr16 : 1;
    uint32 tr17 : 1;
    uint32 tr18 : 1;
    uint32 tr19 : 1;
    uint32 tr20 : 1;
    uint32 tr21 : 1;
    uint32 tr22 : 1;
    uint32 r0 : 9;
};

union exit_register {
    struct exit_register_1bit bits;
    uint32 all;
};

typedef struct exit_regs {
    volatile union exit_register IMR;    /* EXTI�ж�ʹ�ܼĴ���, offset: 0x00 */
    volatile union exit_register EMR;    /* EXTI�¼�ʹ�ܼĴ���, offset: 0x04 */
    volatile union exit_register RTSR;   /* EXTI�����ش���ѡ��Ĵ���, offset: 0x08 */
    volatile union exit_register FTSR;   /* EXTI�½��ش���ѡ��Ĵ���, offset: 0x0C */
    volatile union exit_register SWIER;  /* EXTI��������ж�, offset: 0x10 */
    volatile union exit_register PR;     /* EXTI Pending�Ĵ���,д1���, offset: 0x14 */
} exit_regs_t;

#define EXIT_BASE 0x40013C00
#define EXIT ((exit_regs_t *) EXIT_BASE)

#endif // !STM32F407_EXIT_H

