#include <stm32f407.h>
#include <xtos.h>
#include <led.h>
#include <uart.h>
#include <sdio.h>
#include <fakei2c.h>
#include <eeprom.h>
#include <mpu6050.h>


void config_interruts(void);

/******************************************************************/

static void i2c_scl_h(void) {
    PGout(3) = 1;
}

static void i2c_scl_l(void) {
    PGout(3) = 0;
}

static void i2c_sda_h(void) {
    PGout(4) = 1;
}

static void i2c_sda_l(void) {
    PGout(4) = 0;
}

static bool i2c_sda(void) {
    return (1 == PGin(4)) ? TRUE : FALSE;
}

static void i2c_set_sda_out(void) {
    GPIOG->MODER.bits.pin4 = GPIO_Mode_Out;
}

static void i2c_set_sda_in(void) {
    GPIOG->MODER.bits.pin4 = GPIO_Mode_In;
}
/*
* PG3 -> SCL, PG4 -> SDA
*/
static void i2c_init(void) {
    RCC->AHB1ENR.bits.gpiog = 1;

    GPIOG->MODER.bits.pin3 = GPIO_Mode_Out;
    GPIOG->MODER.bits.pin4 = GPIO_Mode_Out;

    GPIOG->OTYPER.bits.pin3 = GPIO_OType_PP;
    GPIOG->OTYPER.bits.pin4 = GPIO_OType_PP;

    GPIOG->PUPDR.bits.pin3 = GPIO_Pull_Up;
    GPIOG->PUPDR.bits.pin4 = GPIO_Pull_Up;

    GPIOG->OSPEEDR.bits.pin3 = GPIO_OSpeed_High;
    GPIOG->OSPEEDR.bits.pin4 = GPIO_OSpeed_High;

    i2c_sda_h();
    i2c_scl_h();
}

static i2c_dev gI2C = {
    .scl_h = i2c_scl_h,
    .scl_l = i2c_scl_l,
    .sda_h = i2c_sda_h,
    .sda_l = i2c_sda_l,
    .sda = i2c_sda,
    .set_sda_out = i2c_set_sda_out,
    .set_sda_in = i2c_set_sda_in,
    .init = i2c_init,
    .delayus = 20
};

static struct mpu6050 gMpu6050 = {
    .addr = 0xD0,
    .i2c = &gI2C
};

/*******************************************************************************/
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
    uint8 data;
    while (1) {
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            mpu6050_update(&gMpu6050);
            LED_0 = LED_ON;
        } else {
            LED_0 = LED_OFF;
        }

        //xtos_delay_ms(1);
    }
}
/******************************************************************/

uint8 txBuf[10] = { 'A', 'b', 'C', 'd', 'E', 'f', 'G', 'h', 'I', 'j' };
uint8 rxBuf[10] = { 0,1,2,3,4,5,6,7,8,9 };

struct sd_card card;

int main(void) {
    sys_init();
    led_init();
    usart3_init(115200);
    config_interruts();

    LED_2 = LED_ON;
    sys_delay_ms(1000);

    if (MPU6050_ERROR_NONE != mpu6050_init(&gMpu6050)) {
        LED_2 = LED_OFF;
        uart_send_str(USART3, "MPU6050≥ı ºªØ¥ÌŒÛ!\r\n");
        while (1);
    }
    
    xtos_init();
    xtos_init_task_struct(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_struct(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();
    
    while(1) {

    }
}
