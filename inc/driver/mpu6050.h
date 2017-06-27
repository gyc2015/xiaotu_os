#pragma once

#include <fakei2c.h>

struct mpu_uint8 {
    uint8 ax;
    uint8 ay;
    uint8 az;
    uint8 gx;
    uint8 gy;
    uint8 gz;
};

struct mpu_int16 {
    int16 ax;
    int16 ay;
    int16 az;
    int16 gx;
    int16 gy;
    int16 gz;
};

struct mpu_float {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
};

struct mpu_double {
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
};

#define MPU6050_ERROR_NONE              0
#define MPU6050_ERROR_CONNECT_FAILED    1
#define MPU6050_ERROR_SELF_TEST_FAILED  2

struct mpu6050 {
    uint8 addr;
    uint8 error;
    struct mpu_int16 ivalue;
    struct mpu_double fvalue;
    struct mpu_double bias;
    double gres; // �����Ƿֱ���
    double ares; // ���ٶȼƷֱ���
    double q[4]; // ��Ԫ��
    i2c_dev *i2c;
};
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

void mpu6050_readValue(struct mpu6050 *mpu);
