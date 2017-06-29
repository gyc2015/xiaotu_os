#pragma once

#include <fakei2c.h>
#include <imu.h>

#define MPU6050_ERROR_NONE              0
#define MPU6050_ERROR_CONNECT_FAILED    1
#define MPU6050_ERROR_SELF_TEST_FAILED  2

struct mpu6050 {
    uint8 addr;                 // �豸��ַ0xD0����0xD2
    uint8 error;                // MPU6050״̬
    struct imu_6x_int16 ivalue;    // MPU6050ԭʼADC����
    struct imu_6x_double fvalue;   // ���ٶ�(g), ���ٶ�(deg/s)
    struct imu_6x_double bias;     // ��ƫ
    double gres;                // �����Ƿֱ���
    double ares;                // ���ٶȼƷֱ���
    i2c_dev *i2c;               // I2C�˿�
};

uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);
/*
 * mpu6050_init - ��ʼ��
 *
 * @mpu: Ŀ�괫����
 */
uint8 mpu6050_init(struct mpu6050 *mpu);
/*
 * mpu6050_calibrate - У׼
 *
 * @mpu: Ŀ�괫����
 */
void mpu6050_calibrate(struct mpu6050 *mpu);
/*
 * mpu6050_self_test - �ϵ��Լ�
 *
 * @mpu: Ŀ�괫����
 */
bool mpu6050_self_test(struct mpu6050 *mpu);
/*
 * mpu6050_set_gyro_scale - ��������������
 *
 * @mpu: Ŀ�괫����
 * @fsmacro: ���̺궨��
 */
double mpu6050_set_gyro_scale(struct mpu6050 *mpu, uint8 fsmacro);
/*
 * mpu6050_set_acc_scale - ���ü��ٶȼ�����
 *
 * @mpu: Ŀ�괫����
 * @fsmacro: ���̺궨��
 */
double mpu6050_set_acc_scale(struct mpu6050 *mpu, uint8 fsmacro);

void mpu6050_read_value(struct mpu6050 *mpu, struct imu_6x_int16 *ivalue);
void mpu6050_update(struct mpu6050 *mpu);


