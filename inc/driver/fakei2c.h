#pragma once

#include <stm32f407_gpio.h>

typedef struct {
    void(*scl_h)(void);
    void(*scl_l)(void);
    void(*sda_h)(void);
    void(*sda_l)(void);
    bool(*sda)(void);
    void(*set_scl_out)(void);
    void(*set_sda_out)(void);
    void(*set_sda_in)(void);
    void(*init)(void);
} i2c_dev;


void Init_I2C(i2c_dev *dev);
void I2C_SendByte(i2c_dev *dev, uint8 data, uint8 add, uint8 reg);
void I2C_SendDatas(i2c_dev *dev, const uint8 *buf, int len, uint8 add, uint8 reg);
void I2C_ReceiveDatas(i2c_dev *dev, uint8 *buf, int len, uint8 add, uint8 reg);

