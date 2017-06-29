#include <imu.h>
#include <math.h>
#include <matrix.h>

/*
 * imu_reset_quat - 初始对准
 *
 * @param: 飞行参数
 */
void imu_init_param(struct flight_parameters *param) {
    param->q[0] = 1;
    param->q[1] = 0;
    param->q[2] = 0;
    param->q[3] = 0;

    param->r[0] = 0;
    param->r[1] = 0;
    param->r[2] = 0;
}
/*
 * imu_update_quat - 更新四元数
 *
 * @ag: imu数据
 * @param: 飞行参数
 * @dt: 时间间隔
 */
void imu_update_quat(const struct imu_6x_double *ag, struct flight_parameters *param, double dt) {
    double gx = ag->gx;
    double gy = ag->gy;
    double gz = ag->gz;
    double hdt = 0.5 * dt;
    double *q = param->q;

    q[0] += (-q[1] * gx - q[2] * gy - q[3] * gz) * hdt;
    q[1] += (q[0] * gx + q[2] * gz - q[3] * gy) * hdt;
    q[2] += (q[0] * gy - q[1] * gz + q[3] * gx) * hdt;
    q[3] += (q[0] * gz + q[1] * gy - q[2] * gx) * hdt;

    double norm = 1 / sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    q[0] *= norm;
    q[1] *= norm;
    q[2] *= norm;
    q[3] *= norm;
}
/*
 * imu_update_xyz - 计算旋转矩阵,位置积分
 *
 * @ag: imu数据
 * @param: 飞行参数
 * @dt: 时间间隔
 */
#define IMU_GRAVITY_COUNT   (9.8)   // m/s^2
void imu_update_xyz(const struct imu_6x_double *ag, struct flight_parameters *param, double dt) {
    double tmp[3];
    tmp[0] = ag->ax * IMU_GRAVITY_COUNT;
    tmp[1] = ag->ay * IMU_GRAVITY_COUNT;
    tmp[2] = ag->az * IMU_GRAVITY_COUNT;

    imu_get_rotate_matrix(param, param->R);
    matrix3d_lmul_vector(param->R, tmp, param->a);
    // 加速度积分,目前先不考虑科氏力和重力影响
    // 速度增量
    vector3d_mul(param->a, dt, tmp);
    vector3d_add_(param->v, tmp);
    // 位置增量
    vector3d_mul(param->v, dt, tmp);
    vector3d_add_(param->r, tmp);
}
/*
 * imu_get_rotate_matrix - 计算旋转矩阵
 *
 * @param: 飞行参数
 * @m: 旋转矩阵
 */
void imu_get_rotate_matrix(const struct flight_parameters *param, double *m) {
    double q1q1 = param->q[1] * param->q[1];
    double q2q2 = param->q[2] * param->q[2];
    double q3q3 = param->q[3] * param->q[3];

    double q0q3 = param->q[0] * param->q[3];
    double q0q2 = param->q[0] * param->q[2];
    double q0q1 = param->q[0] * param->q[1];
    double q1q2 = param->q[1] * param->q[2];
    double q1q3 = param->q[1] * param->q[3];
    double q2q3 = param->q[2] * param->q[3];

    m[0 * 3 + 0] = 1 - 2 * (q2q2 + q3q3);
    m[0 * 3 + 1] = 2 * (q1q2 - q0q3);
    m[0 * 3 + 2] = 2 * (q1q3 + q0q2);
    m[1 * 3 + 0] = 2 * (q1q2 + q0q3);
    m[1 * 3 + 1] = 1 - 2 * (q1q1 + q3q3);
    m[1 * 3 + 2] = 2 * (q2q3 - q0q1);
    m[2 * 3 + 0] = 2 * (q1q3 - q0q2);
    m[2 * 3 + 1] = 2 * (q2q3 + q0q1);
    m[2 * 3 + 2] = 1 - 2 * (q1q1 + q2q2);
}
/*
 * imu_get_euler_angle - 计算欧拉角,需在更新了旋转矩阵之后调用
 *
 * @param: 飞行参数
 * @m: 欧拉角
 */
void imu_get_euler_angle(const struct flight_parameters *param, struct eular_angle *rpy) {
    rpy->yaw = atan2(-param->R[0 * 3 + 1], param->R[0 * 3 + 0]);
    rpy->pitch = asin(param->R[0 * 3 + 2]);
    rpy->roll = atan2(-param->R[1 * 3 + 2], param->R[2 * 3 + 2]);
}


