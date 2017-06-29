#pragma once

#include <fakei2c.h>
#include <imu.h>

#define MPU6050_ERROR_NONE              0
#define MPU6050_ERROR_CONNECT_FAILED    1
#define MPU6050_ERROR_SELF_TEST_FAILED  2

struct mpu6050 {
    uint8 addr;                 // 设备地址0xD0或者0xD2
    uint8 error;                // MPU6050状态
    struct imu_6x_int16 ivalue;    // MPU6050原始ADC读数
    struct imu_6x_double fvalue;   // 加速度(g), 角速度(deg/s)
    struct imu_6x_double bias;     // 零偏
    double gres;                // 陀螺仪分辨率
    double ares;                // 加速度计分辨率
    i2c_dev *i2c;               // I2C端口
};

uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);
/*
 * mpu6050_init - 初始化
 *
 * @mpu: 目标传感器
 */
uint8 mpu6050_init(struct mpu6050 *mpu);
/*
 * mpu6050_calibrate - 校准
 *
 * @mpu: 目标传感器
 */
void mpu6050_calibrate(struct mpu6050 *mpu);
/*
 * mpu6050_self_test - 上电自检
 *
 * @mpu: 目标传感器
 */
bool mpu6050_self_test(struct mpu6050 *mpu);
/*
 * mpu6050_set_gyro_scale - 设置陀螺仪量程
 *
 * @mpu: 目标传感器
 * @fsmacro: 量程宏定义
 */
double mpu6050_set_gyro_scale(struct mpu6050 *mpu, uint8 fsmacro);
/*
 * mpu6050_set_acc_scale - 设置加速度计量程
 *
 * @mpu: 目标传感器
 * @fsmacro: 量程宏定义
 */
double mpu6050_set_acc_scale(struct mpu6050 *mpu, uint8 fsmacro);

void mpu6050_read_value(struct mpu6050 *mpu, struct imu_6x_int16 *ivalue);
void mpu6050_update(struct mpu6050 *mpu);


