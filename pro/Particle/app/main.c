#include <stm32f407.h>
#include <xtos.h>
#include <led.h>
#include <uart.h>
#include <fakei2c.h>
#include <eeprom.h>
#include <mpu6050.h>

void config_interruts(void);

/******************************************************************/
#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_descriptor taskA;
static struct xtos_task_descriptor taskB;

void taska() {
    while (1) {
        LED_1 = LED_ON;
        LED_2 = LED_OFF;
    }
}

void taskb() {
    while (1) {
        LED_1 = LED_OFF;
        LED_2 = LED_ON;
    }
}


/******************************************************************/
uint8 txBuf[10] = { 'A', 'b', 'C', 'd', 'E', 'f', 'G', 'h', 'I', 'j' };
uint8 rxBuf[10] = { 0,1,2,3,4,5,6,7,8,9 };

int main(void) {
    led_init();
    usart3_init(115200);
    eeprom_init();
    mpu6050_init();

    config_interruts();

    LED_0 = LED_ON;

    uart_send_str(USART3, "\r\n====================\r\n");

    //eeprom_write_bytes(txBuf, 10, 0);
    eeprom_read_bytes(rxBuf, 10, 0);
    uart_send_bytes(USART3, rxBuf, 10);

    xtos_init(168000);
    xtos_init_task_struct(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_struct(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();


    while(1) {

    }
}
