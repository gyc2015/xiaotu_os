#include <stm32f407.h>
#include <xtos.h>

#define KEY (GPIOH->IDR.bits.pin15)
#define LED_R (GPIOI->ODR.bits.pin5)
#define LED_G (GPIOI->ODR.bits.pin6)
#define LED_B (GPIOI->ODR.bits.pin7)
#define ON 0
#define OFF 1

void init_led() {
    RCC->AHB1ENR.bits.gpioi = 1;

    GPIOI->MODER.bits.pin5 = GPIO_Mode_Out;
    GPIOI->MODER.bits.pin6 = GPIO_Mode_Out;
    GPIOI->MODER.bits.pin7 = GPIO_Mode_Out;

    GPIOI->OTYPER.bits.pin5 = GPIO_OType_PP;
    GPIOI->OTYPER.bits.pin6 = GPIO_OType_PP;
    GPIOI->OTYPER.bits.pin7 = GPIO_OType_PP;

    GPIOI->PUPDR.bits.pin5 = GPIO_Pull_Up;
    GPIOI->PUPDR.bits.pin6 = GPIO_Pull_Up;
    GPIOI->PUPDR.bits.pin7 = GPIO_Pull_Up;

    GPIOI->OSPEEDR.bits.pin5 = GPIO_OSpeed_Very_High;
    GPIOI->OSPEEDR.bits.pin6 = GPIO_OSpeed_Very_High;
    GPIOI->OSPEEDR.bits.pin7 = GPIO_OSpeed_Very_High;
}

void delay(int c) {
    for (int i = 0; i < c; i++);
}

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_struct taskA;
static struct xtos_task_struct taskB;

void task_switch() {
    if (gp_xtos_cur_task == &taskA)
        gp_xtos_next_task = &taskB;
    else
        gp_xtos_next_task = &taskA;

    xtos_context_switch();
}

void taska() {
    while (1) {
        LED_R = ON;
        LED_G = OFF;
        LED_B = OFF;
        for (int i = 0; i < 1000; i++)
            delay(10000);
        task_switch();
    }
}

void taskb() {
    while (1) {
        LED_R = OFF;
        LED_G = ON;
        LED_B = OFF;
        for (int i = 0; i < 1000; i++)
            delay(10000);
        task_switch();
    }
}

int main(void) {
    init_led();

    LED_R = OFF;
    LED_G = OFF;
    LED_B = OFF;
    
    xtos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    xtos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    xtos_start();

    while(1) {

    }
}
