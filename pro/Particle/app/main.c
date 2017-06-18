#include <stm32f407.h>
#include <xtos.h>
#include <power.h>
#include <led.h>
#include <uart4.h>
#include <iwdg.h>
#include <wwdg.h>
#include <sdio.h>
#include <timer3.h>
#include <stm32f407_dma.h>

//uint8 writebuf[512];
uint8 readbuf[1024];
float fvcc = 0;
float fcurrent = 0;
float fd1v2 = 0;
float fd3v3 = 0;
float fa2v5 = 0;

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
        led_set_color(100, 50, 0);
    }
}

void taskb() {
    while (1) {
        led_set_color(0, 50, 100);
        fvcc = power_get_vcc();
        fcurrent = power_get_current();
        fd1v2 = power_get_D1V2();
        fd3v3 = power_get_D3V3();
        fa2v5 = power_get_A2V5();
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

void config_interruts(void);


int main(void) {
    struct sd_card card;

    //led_init();
    power_init();
    led_pwm_init();
    systick_init(168000);
    uart4_init(115200);
    sdio_init(&card);
    //tim3_init(41999, 999);
    config_interruts();

    fvcc = power_get_vcc();
    fcurrent = power_get_current();
    fd1v2 = power_get_D1V2();
    fd3v3 = power_get_D3V3();
    fa2v5 = power_get_A2V5();

    led_set_color(0, 0, 50);
    uart4_send_str("G.Y.C");
/*
    for (int i = 0; i < 1024; i++)
        readbuf[i] = 'C';

    if (SDE_OK != sdio_write_multiblock(&card, 512, readbuf, 2))
        uart4_send_str("\r\nhehe\r\n");
    if (SDE_OK != sdio_write_finished(&card))
        uart4_send_str("\r\nhaha\r\n");
    while (SDE_OK != sdio_expect_card_state(&card, SD_CS_TRAN))
        uart4_send_str("\r\nxixi\r\n");
*/
    uart4_send_str("\r\nG.Y.C\r\n");

    for (int i = 0; i < 1024; i++)
        readbuf[i] = '0';

    if (SDE_OK != sdio_read_multiblock(&card, 0, readbuf, 2))
        uart4_send_str("\r\nhehe\r\n");
    if (SDE_OK != sdio_read_finished(&card))
        uart4_send_str("\r\nhaha\r\n");
    while (SDE_OK != sdio_expect_card_state(&card, SD_CS_TRAN))
        uart4_send_str("\r\nxixi\r\n");

    uart4_send_str("\r\n====\r\n");

    for (int i = 0; i < 1024; i++)
        uart4_send_byte(readbuf[i]);

    xtos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    xtos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    SYSTICK->CTRL.bits.en = 1;
    xtos_start();


    while(1) {}
}
