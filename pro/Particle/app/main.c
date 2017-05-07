#include <stm32f407.h>
#include <xtos.h>
#include <led.h>
#include <uart4.h>
#include <iwdg.h>
#include <wwdg.h>
#include <sdio.h>
#include <stm32f407_dma.h>

#define KEY (GPIOH->IDR.bits.pin15)

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
        LED_R = LED_ON;
        LED_G = LED_OFF;
        LED_B = LED_OFF;
    }
}

void taskb() {
    while (1) {
        LED_R = LED_OFF;
        LED_G = LED_ON;
        LED_B = LED_OFF;
    }
}

#define SysTicks_Irq_n 15

void systick_init(uint32 ticks) {
    SYSTICK->LOAD.bits.cnt = ticks;         // 重装载数据
    SYSTICK->VAL.bits.cnt = 0;              // 清除当前计数

    SCB->SHP[SysTicks_Irq_n - 4] = 0x00;    // 暂时赋予最高优先级

    SYSTICK->CTRL.bits.clksource = 1;       // 使用CPU时钟168MHz
    SYSTICK->CTRL.bits.tickint = 1;         // 开启计数溢出中断
    SYSTICK->CTRL.bits.en = 0;              // 暂不开启计时器
}

void SysTick_Handler(void) {
    static int counter = 0;

    if ((counter++ % 1000) == 0) {
        task_switch();
    }
}

uint32 sdsize = 0;
int main(void) {
    struct sd_card card;

    led_init();
    systick_init(168000);
    uart4_init(921600);
    sdio_init(&card);
    sdsize = card.capacity >> 20;

    LED_R = LED_OFF;
    LED_G = LED_OFF;
    LED_B = LED_ON;

    uart4_send_str("G.Y.C");

    xtos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    xtos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    SYSTICK->CTRL.bits.en = 1;
    xtos_start();

    while(1) {}
}
