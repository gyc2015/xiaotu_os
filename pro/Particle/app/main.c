#include <stm32f407.h>
#include <xtos.h>
#include <led.h>

void config_interruts(void);

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_descriptor taskA;
static struct xtos_task_descriptor taskB;

void taska() {
    while (1) {
        LED_1 = LED_ON;
        xtos_delay_ms(1000);
        LED_1 = LED_OFF;
        xtos_delay_ms(1000);
    }
}
void taskb() {
    while (1) {
        LED_0 = LED_ON;
        xtos_delay_ms(1000);
        LED_0 = LED_OFF;
        xtos_delay_ms(1000);
    }
}
/******************************************************************/

int main(void) {
    sys_init();
    led_init();
    config_interruts();

    xtos_init();
    xtos_init_task_descriptor(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_descriptor(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();
    
    while(1) {

    }
}
