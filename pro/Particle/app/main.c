#include <stm32f407.h>
#include <xtos.h>
#include <led.h>

#include <uart.h>
#include <sdio.h>
#include <fakei2c.h>
#include <eeprom.h>
#include <mpu6050.h>

#include "ff.h"
#include "diskio.h"


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

uint8 txBuf[10] = { 'A', 'b', 'C', 'd', 'E', 'f', 'G', 'h', 'I', 'j' };
uint8 rxBuf[10] = { 0,1,2,3,4,5,6,7,8,9 };

struct sd_card gCard;
uint8 readbuf[1024];

int main(void) {
    sys_init();
    led_init();

    usart3_init(115200);
    eeprom_init();
    sdio_init(&gCard);

    config_interruts();

    LED_2 = LED_ON;


    if (MPU6050_ERROR_NONE == readbuf[0])
        uart_send_str(USART3, "\r\n====================\r\n");

    for (int i = 0; i < 1024; i++)
        readbuf[i] = '0';

    if (SDE_OK != sdio_read_multiblock(&gCard, 0, readbuf, 2))
        uart_send_str(USART3,"\r\nhehe\r\n");
    if (SDE_OK != sdio_read_finished(&gCard))
        uart_send_str(USART3,"\r\nhaha\r\n");
    while (SDE_OK != sdio_expect_card_state(&gCard, SD_CS_TRAN))
        uart_send_str(USART3,"\r\nxixi\r\n");

    uart_send_str(USART3,"\r\n====\r\n");

    for (int i = 0; i < 1024; i++) {
        uart_send_byte(USART3, readbuf[i]);
        readbuf[i] = 'B';
    }

    FATFS fatfs;
    FIL file;
    FRESULT res = f_mount(&fatfs, "0:", 1);
    unsigned int counter;
    res = f_open(&file, "0:/test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    res = f_lseek(&file, 0);
    res = f_write(&file, readbuf, 512, &counter);
    res = f_lseek(&file, 0);
    res = f_read(&file, readbuf, 512, &counter);
    f_close(&file);


    xtos_init();
    xtos_init_task_descriptor(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_descriptor(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();
    
    while(1) {

    }
}
