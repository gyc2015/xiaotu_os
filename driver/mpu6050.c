#include <mpu6050.h>
#include <system.h>
#include <utils.h>
#include <stm32f407_rcc.h>
#include <math.h>


#define MPU6050_SELF_TEST_X         ((uint8)0x0D)
#define MPU6050_SELF_TEST_Y         ((uint8)0x0E) 
#define MPU6050_SELF_TEST_Z         ((uint8)0x0F)
#define MPU6050_SELF_TEST_A         ((uint8)0x10)

#define MPU6050_SMPLRT_DIV          ((uint8)0x19) /* R25 */
#define MPU6050_CONFIG              ((uint8)0x1A) /* R26 */
#define MPU6050_GYRO_CONFIG         ((uint8)0x1B) /* R27 */
#define MPU6050_ACCEL_CONFIG        ((uint8)0x1C) /* R28 */

#define MPU6050_FIFO_EN             ((uint8)0x23) /* R35 */
#define MPU6050_I2C_MST_CTRL        ((uint8)0x24) /* R36 */

#define MPU6050_INT_PIN_CFG         ((uint8)0x37) /* R55 */
#define MPU6050_INT_ENABLE          ((uint8)0x38) /* R56 */
#define MPU6050_INT_STATUS          ((uint8)0x3A) /* R58 */

#define MPU6050_ACCEL_XOUT_H        ((uint8)0x3B) /* R59 */
#define MPU6050_ACCEL_XOUT_L        ((uint8)0x3C)
#define MPU6050_ACCEL_YOUT_H        ((uint8)0x3D)
#define MPU6050_ACCEL_YOUT_L        ((uint8)0x3E)
#define MPU6050_ACCEL_ZOUT_H        ((uint8)0x3F)
#define MPU6050_ACCEL_ZOUT_L        ((uint8)0x40)
#define MPU6050_TEMP_OUT_H          ((uint8)0x41)
#define MPU6050_TEMP_OUT_L          ((uint8)0x42)
#define MPU6050_GYRO_XOUT_H         ((uint8)0x43)
#define MPU6050_GYRO_XOUT_L         ((uint8)0x44)
#define MPU6050_GYRO_YOUT_H         ((uint8)0x45)
#define MPU6050_GYRO_YOUT_L         ((uint8)0x46)
#define MPU6050_GYRO_ZOUT_H         ((uint8)0x47)
#define MPU6050_GYRO_ZOUT_L         ((uint8)0x48)

#define MPU6050_USER_CTRL           ((uint8)0x6A) /* R106 */
#define MPU6050_PWR_MGMT_1          ((uint8)0x6B) /* R107 */
#define MPU6050_PWR_MGMT_2          ((uint8)0x6C) /* R108 */

#define MPU6050_FIFO_COUNT_H        ((uint8)0x72) /* R114 */
#define MPU6050_FIFO_COUNT_L        ((uint8)0x73) /* R115 */
#define MPU6050_FIFO_COUNT          MPU6050_FIFO_COUNT_H
#define MPU6050_FIFO_R_W            ((uint8)0x74) /* R116 */
#define MPU6050_WHO_AM_I            ((uint8)0x75) /* R117 */

/*
* Configuration, R26, 0x1A
*/
#define MPU6050_GYRO_DLPF_256       ((uint8)0x00) /* Gyroscope 250带宽 */
#define MPU6050_GYRO_DLPF_188       ((uint8)0x01) /* Gyroscope 184带宽 */
#define MPU6050_GYRO_DLPF_98        ((uint8)0x02) /* Gyroscope 92带宽 */
#define MPU6050_GYRO_DLPF_42        ((uint8)0x03) /* Gyroscope 41带宽 */
#define MPU6050_GYRO_DLPF_20        ((uint8)0x04) /* Gyroscope 20带宽 */
#define MPU6050_GYRO_DLPF_10        ((uint8)0x05) /* Gyroscope 10带宽 */
#define MPU6050_GYRO_DLPF_5         ((uint8)0x06) /* Gyroscope 5带宽 */
/*
* 陀螺仪量程, R27, 0x1B
*/
#define MPU6050_FSR_250DPS  ((uint8)0x00)
#define MPU6050_FSR_500DPS  ((uint8)0x08)
#define MPU6050_FSR_1000DPS  ((uint8)0x10)
#define MPU6050_FSR_2000DPS  ((uint8)0x18)
/*
* 加速度计量程, R28, 0x1C
*/
#define MPU6050_FSR_2G  ((uint8)0x00)
#define MPU6050_FSR_4G  ((uint8)0x08)
#define MPU6050_FSR_8G  ((uint8)0x10)
#define MPU6050_FSR_16G  ((uint8)0x18)
/*
* Power Management 1, R107, 0x6B
*/
#define MPU6050_CLK_INT8M   ((uint8)0x00)
#define MPU6050_CLK_GX      ((uint8)0x01)
#define MPU6050_CLK_GY      ((uint8)0x02)
#define MPU6050_CLK_GZ      ((uint8)0x03)
#define MPU6050_CLK_EXT32K  ((uint8)0x04)
#define MPU6050_CLK_EXT19M  ((uint8)0x05)
#define MPU6050_CLK_STOP    ((uint8)0x07)
#define MPU6050_RESET       ((uint8)0x80)

#define MPU6050_FIFO_RESET  ((uint8)0x04)


uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg) {
    uint8 data;
    I2C_ReceiveDatas(mpu->i2c, &data, 1, mpu->addr, reg);
    return data;
}

/*
 * mpu6050_init - 初始化
 *
 * @mpu: 目标传感器
 */
uint8 mpu6050_init(struct mpu6050 *mpu) {
    uint8 data;
    Init_I2C(mpu->i2c);
    // 检测连接
    I2C_ReceiveDatas(mpu->i2c, &data, 1, mpu->addr, MPU6050_WHO_AM_I);
    if (data != (mpu->addr >> 1))
        return MPU6050_ERROR_CONNECT_FAILED;
    // 上电自检
    if (!mpu6050_self_test(mpu))
        return MPU6050_ERROR_SELF_TEST_FAILED;

    // 电源时钟管理，用陀螺仪的X轴作为时钟源
    I2C_SendByte(mpu->i2c, MPU6050_CLK_GX, mpu->addr, MPU6050_PWR_MGMT_1);
    I2C_SendByte(mpu->i2c, 0x00, mpu->addr, MPU6050_PWR_MGMT_2);
    delay(6000);
    // 采样频率
    // Sample Rate = Gyroscope Output Rate / 1 + SMPLRT_DIV
    // Gyroscope Output Rate = (CONFIG.DLPF == 0) ? 8kHz : 1kHz
    I2C_SendByte(mpu->i2c, MPU6050_GYRO_DLPF_42, mpu->addr, MPU6050_CONFIG);
    I2C_SendByte(mpu->i2c, 0x04, mpu->addr, MPU6050_SMPLRT_DIV);
    // 量程 Gyroscope: ±250 deg/s, Accelerometer: ±4g
    mpu6050_set_gyro_scale(mpu, MPU6050_FSR_250DPS);
    mpu6050_set_acc_scale(mpu, MPU6050_FSR_4G);
    // 中断引脚上拉、推挽、通过读INT_STATUS清除中断, 使能数据准备好中断
    I2C_SendByte(mpu->i2c, 0x22, mpu->addr, MPU6050_INT_PIN_CFG);
    I2C_SendByte(mpu->i2c, 0x01, mpu->addr, MPU6050_INT_ENABLE);

    mpu->q[0] = 1.0f;
    mpu->q[1] = 0.0f;
    mpu->q[2] = 0.0f;
    mpu->q[3] = 0.0f;

    // 校准
    mpu6050_calibrate(mpu);

    return MPU6050_ERROR_NONE;
}
/*
 * mpu6050_self_test - 上电自检
 *
 * @mpu: 目标传感器
 */
bool mpu6050_self_test(struct mpu6050 *mpu) {
    /* 加表值域±8,陀螺值域±250deg/s,开启自检 */
    I2C_SendByte(mpu->i2c, 0xF0, mpu->addr, MPU6050_ACCEL_CONFIG);
    I2C_SendByte(mpu->i2c, 0xE0, mpu->addr, MPU6050_GYRO_CONFIG);
    delay(6000);

    uint8 rawData[4] = { 0, 0, 0, 0 };
    struct mpu_uint8 st;
    I2C_ReceiveDatas(mpu->i2c, &rawData[0], 4, mpu->addr, MPU6050_SELF_TEST_X);
    st.ax = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4;
    st.ay = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2;
    st.az = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 0;
    st.gx = rawData[0] & 0x1F;
    st.gy = rawData[1] & 0x1F;
    st.gz = rawData[2] & 0x1F;
    struct mpu_float ft;
    ft.ax = (4096.0f*0.34f)*(pow((0.92f / 0.34f), ((st.ax - 1.0f) / 30.0f)));
    ft.ay = (4096.0f*0.34f)*(pow((0.92f / 0.34f), ((st.ay - 1.0f) / 30.0f)));
    ft.az = (4096.0f*0.34f)*(pow((0.92f / 0.34f), ((st.az - 1.0f) / 30.0f)));
    ft.gx = (25.0f*131.0f)*(pow(1.046f, (st.gx - 1.0f)));
    ft.gy = (-25.0f*131.0f)*(pow(1.046f, (st.gy - 1.0f)));
    ft.gz = (25.0f*131.0f)*(pow(1.046f, (st.gz - 1.0f)));
    float *pft = &(ft.ax);
    uint8 *pst = &(st.ax);
    for (int i = 0; i < 6; i++) {
        if ((100.0f + 100.0f*(pst[i] - pft[i]) / pft[i]) > 1.0f)
            return FALSE;
    }

    return TRUE;
}
/*
 * mpu6050_calibrate - 校准
 *
 * @mpu: 目标传感器
 */
#define CALIBTIMES 100
uint8 testdata, testdata1;
void mpu6050_calibrate(struct mpu6050 *mpu) {
    double *pbias = &(mpu->bias.ax);
    for (uint16 i = 0; i < 6; i++)
        pbias[i] = 0.0;

}

/*
 * mpu6050_set_gyro_scale - 设置陀螺仪量程
 *
 * @mpu: 目标传感器
 * @fsmacro: 量程宏定义
 */
double mpu6050_set_gyro_scale(struct mpu6050 *mpu, uint8 fsmacro) {
    I2C_SendByte(mpu->i2c, fsmacro, mpu->addr, MPU6050_GYRO_CONFIG);
    switch (fsmacro) {
    case MPU6050_FSR_250DPS:
        mpu->gres = 250.0 / 32768.0;
        break;
    case MPU6050_FSR_500DPS:
        mpu->gres = 500.0 / 32768.0;
        break;
    case MPU6050_FSR_1000DPS:
        mpu->gres = 1000.0 / 32768.0;
        break;
    case MPU6050_FSR_2000DPS:
        mpu->gres = 2000.0 / 32768.0;
        break;
    default:
        mpu->gres = 0;
        break;
    }
    return mpu->gres;
}
/*
 * mpu6050_set_acc_scale - 设置加速度计量程
 *
 * @mpu: 目标传感器
 * @fsmacro: 量程宏定义
 */
double mpu6050_set_acc_scale(struct mpu6050 *mpu, uint8 fsmacro) {
    I2C_SendByte(mpu->i2c, fsmacro, mpu->addr, MPU6050_ACCEL_CONFIG);
    switch (fsmacro) {
    case MPU6050_FSR_2G:
        mpu->ares = 2.0 / 32768.0;
        break;
    case MPU6050_FSR_4G:
        mpu->ares = 4.0 / 32768.0;
        break;
    case MPU6050_FSR_8G:
        mpu->ares = 8.0 / 32768.0;
        break;
    case MPU6050_FSR_16G:
        mpu->ares = 16.0 / 32768.0;
        break;
    default:
        mpu->ares = 0;
        break;
    }
    return mpu->ares;
}

void mpu6050_readValue(struct mpu6050 *mpu) {
    uint8 tmp[14];
    I2C_ReceiveDatas(mpu->i2c, tmp, 14, mpu->addr, MPU6050_ACCEL_XOUT_H);

    mpu->ivalue.ax = (int16)(((int16)tmp[0] << 8) | tmp[1]);
    mpu->ivalue.ay = (int16)(((int16)tmp[2] << 8) | tmp[3]);
    mpu->ivalue.az = (int16)(((int16)tmp[4] << 8) | tmp[5]);
    mpu->ivalue.gx = (int16)(((int16)tmp[8] << 8) | tmp[9]);
    mpu->ivalue.gy = (int16)(((int16)tmp[10] << 8) | tmp[11]);
    mpu->ivalue.gz = (int16)(((int16)tmp[12] << 8) | tmp[13]);

    mpu->fvalue.ax = mpu->ares * (float)mpu->ivalue.ax - mpu->bias.ax;
    mpu->fvalue.ay = mpu->ares * (float)mpu->ivalue.ay - mpu->bias.ay;
    mpu->fvalue.az = mpu->ares * (float)mpu->ivalue.az - mpu->bias.az;
    mpu->fvalue.gx = mpu->gres * (float)mpu->ivalue.gx - mpu->bias.gx;
    mpu->fvalue.gy = mpu->gres * (float)mpu->ivalue.gy - mpu->bias.gy;
    mpu->fvalue.gz = mpu->gres * (float)mpu->ivalue.gz - mpu->bias.gz;
}

double beta = 0.0;
double deltat = 1;
void mpu6050_quaternion(struct mpu6050 *mpu) {
    double q1 = mpu->q[0];
    double q2 = mpu->q[1];
    double q3 = mpu->q[2];
    double q4 = mpu->q[3];

    double _halfq1 = 0.5 * q1;
    double _halfq2 = 0.5 * q2;
    double _halfq3 = 0.5 * q3;
    double _halfq4 = 0.5 * q4;

    double _2q1 = 2.0 * q1;
    double _2q2 = 2.0 * q2;
    double _2q3 = 2.0 * q3;
    double _2q4 = 2.0 * q4;

    double norm = sqrt(mpu->fvalue.ax * mpu->fvalue.ax + mpu->fvalue.ay * mpu->fvalue.ay + mpu->fvalue.az * mpu->fvalue.az);
    double ax = mpu->fvalue.ax / norm;
    double ay = mpu->fvalue.ay / norm;
    double az = mpu->fvalue.az / norm;

    double f1 = _2q2 * q4 - _2q1 * q3 - ax;
    double f2 = _2q1 * q2 + _2q3 * q4 - ay;
    double f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;
    // Compute the objective function and Jacobian
    double J_11or24 = _2q3;
    double J_12or23 = _2q4;
    double J_13or22 = _2q1;
    double J_14or21 = _2q2;
    double J_32 = 2.0f * J_14or21;
    double J_33 = 2.0f * J_11or24;
    // Compute the gradient (matrix multiplication)
    double hatDot1 = J_14or21 * f2 - J_11or24 * f1;
    double hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
    double hatDot3 = J_12or23 * f2 - J_33 *f3 - J_13or22 * f1;
    double hatDot4 = J_14or21 * f1 + J_11or24 * f2;
    // Normalize the gradient
    norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
    hatDot1 /= norm;
    hatDot2 /= norm;
    hatDot3 /= norm;
    hatDot4 /= norm;
    // Compute the quaternion derivative
    double qDot1 = -_halfq2 * mpu->fvalue.gx - _halfq3 * mpu->fvalue.gy - _halfq4 * mpu->fvalue.gz;
    double qDot2 = _halfq1 * mpu->fvalue.gx + _halfq3 * mpu->fvalue.gz - _halfq4 * mpu->fvalue.gy;
    double qDot3 = _halfq1 * mpu->fvalue.gy - _halfq2 * mpu->fvalue.gz + _halfq4 * mpu->fvalue.gx;
    double qDot4 = _halfq1 * mpu->fvalue.gz + _halfq2 * mpu->fvalue.gy - _halfq3 * mpu->fvalue.gx;
    q1 += (qDot1 - (beta * hatDot1)) * deltat;
    q2 += (qDot2 - (beta * hatDot2)) * deltat;
    q3 += (qDot3 - (beta * hatDot3)) * deltat;
    q4 += (qDot4 - (beta * hatDot4)) * deltat;
    // normalise quaternion
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
    norm = 1.0f / norm;
    mpu->q[0] = q1 * norm;
    mpu->q[1] = q2 * norm;
    mpu->q[2] = q3 * norm;
    mpu->q[3] = q4 * norm;
}



