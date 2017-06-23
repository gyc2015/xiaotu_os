#include <mpu6050.h>
#include <stm32f407_rcc.h>


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

static void i2c_set_scl_out(void) {
    GPIOG->MODER.bits.pin3 = GPIO_Mode_Out;
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

    GPIOG->OSPEEDR.bits.pin3 = GPIO_OSpeed_Very_High;
    GPIOG->OSPEEDR.bits.pin4 = GPIO_OSpeed_Very_High;

    i2c_sda_h();
    i2c_scl_h();
}

static i2c_dev gI2C = {
    .scl_h = i2c_scl_h,
    .scl_l = i2c_scl_l,
    .sda_h = i2c_sda_h,
    .sda_l = i2c_sda_l,
    .sda = i2c_sda,
    .set_scl_out = i2c_set_scl_out,
    .set_sda_out = i2c_set_sda_out,
    .set_sda_in = i2c_set_sda_in,
    .init = i2c_init
};

uint8 mpudata;
void mpu6050_init(void) {
    Init_I2C(&gI2C);

    I2C_ReceiveDatas(&gI2C, &mpudata, 1, 0xD0, 0x75);


}
