#pragma once

#include <mpu6050.h>

struct imu_6x_uint8 {
    uint8 ax;
    uint8 ay;
    uint8 az;
    uint8 gx;
    uint8 gy;
    uint8 gz;
};
struct imu_6x_int16 {
    int16 ax;
    int16 ay;
    int16 az;
    int16 gx;
    int16 gy;
    int16 gz;
};
struct imu_6x_int32 {
    int32 ax;
    int32 ay;
    int32 az;
    int32 gx;
    int32 gy;
    int32 gz;
};
struct imu_6x_float {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
};
struct imu_6x_double {
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
};

struct flight_parameters {
    double q[4];    // 四元素
    double R[9];    // 体坐标系到参考坐标系的旋转矩阵
    double a[3];    // 参考坐标系下加速度
    double v[3];    // 参考坐标系下速度
    double r[3];    // 参考坐标系下位置
};
struct eular_angle {
    double roll;
    double pitch;
    double yaw;
};

/*
* imu_reset_quat - 初始对准
*
* @param: 飞行参数
*/
void imu_init_param(struct flight_parameters *param);
/*
 * imu_update_quat - 更新四元数
 *
 * @ag: imu数据
 * @param: 飞行参数
 * @dt: 时间间隔
 */
void imu_update_quat(struct imu_6x_double *ag, struct flight_parameters *param, double dt);
/*
* imu_update_xyz - 计算旋转矩阵,位置积分
*
* @ag: imu数据
* @param: 飞行参数
* @dt: 时间间隔
*/
void imu_update_xyz(const struct imu_6x_double *ag, struct flight_parameters *param, double dt);
/*
 * imu_get_rotate_matrix - 计算旋转矩阵
 *
 * @param: 飞行参数
 * @m: 旋转矩阵
 */
void imu_get_rotate_matrix(const struct flight_parameters *param, double *m);
/*
 * imu_get_euler_angle - 计算欧拉角,需在更新了旋转矩阵之后调用
 *
 * @param: 飞行参数
 * @m: 欧拉角
 */
void imu_get_euler_angle(const struct flight_parameters *param, struct eular_angle *rpy);
