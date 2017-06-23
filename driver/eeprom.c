#include <eeprom.h>
#include <stm32f407_rcc.h>
#include <utils.h>

static void i2c1_scl_h(void) {
    PBout(8) = 1;
}

static void i2c1_scl_l(void) {
    PBout(8) = 0;
}

static void i2c1_sda_h(void) {
    PBout(9) = 1;
}

static void i2c1_sda_l(void) {
    PBout(9) = 0;
}

static bool i2c1_sda(void) {
    return (1 == PBin(9)) ? TRUE : FALSE;
}

static void i2c1_set_scl_out(void) {
    GPIOB->MODER.bits.pin8 = GPIO_Mode_Out;
}

static void i2c1_set_sda_out(void) {
    GPIOB->MODER.bits.pin9 = GPIO_Mode_Out;
}

static void i2c1_set_sda_in(void) {
    GPIOB->MODER.bits.pin9 = GPIO_Mode_In;
}
/*
* PB8 -> SCL, PB9 -> SDA
*/
static void i2c1_init(void) {
    RCC->AHB1ENR.bits.gpiob = 1;

    GPIOB->MODER.bits.pin8 = GPIO_Mode_Out;
    GPIOB->MODER.bits.pin9 = GPIO_Mode_Out;

    GPIOB->OTYPER.bits.pin8 = GPIO_OType_PP;
    GPIOB->OTYPER.bits.pin9 = GPIO_OType_PP;

    GPIOB->PUPDR.bits.pin8 = GPIO_Pull_Up;
    GPIOB->PUPDR.bits.pin9 = GPIO_Pull_Up;

    GPIOB->OSPEEDR.bits.pin8 = GPIO_OSpeed_Very_High;
    GPIOB->OSPEEDR.bits.pin9 = GPIO_OSpeed_Very_High;

    i2c1_sda_h();
    i2c1_scl_h();
}

static i2c_dev gI2C1 = {
    .scl_h = i2c1_scl_h,
    .scl_l = i2c1_scl_l,
    .sda_h = i2c1_sda_h,
    .sda_l = i2c1_sda_l,
    .sda = i2c1_sda,
    .set_scl_out = i2c1_set_scl_out,
    .set_sda_out = i2c1_set_sda_out,
    .set_sda_in = i2c1_set_sda_in,
    .init = i2c1_init
};

#define EEPROM_ADDR 0xA0

void eeprom_init(void) {
    Init_I2C(&gI2C1);
}

void eeprom_write_bytes(const uint8 *buf, uint8 len, uint8 reg) {
    for (int i = 0; i < len; i++) {
        I2C_SendByte(&gI2C1, buf[i], EEPROM_ADDR, i);
        for (int j = 0; j < 10; j++)
            Delay(6000);
    }
}

void eeprom_read_bytes(uint8 *buf, uint8 len, uint8 reg) {
    for (int i = 0; i < len; i++) {
        I2C_ReceiveDatas(&gI2C1, &(buf[i]), 1, EEPROM_ADDR, i);
    }
}


