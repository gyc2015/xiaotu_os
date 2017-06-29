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
    double q[4];    // ��Ԫ��
    double R[9];    // ������ϵ���ο�����ϵ����ת����
    double a[3];    // �ο�����ϵ�¼��ٶ�
    double v[3];    // �ο�����ϵ���ٶ�
    double r[3];    // �ο�����ϵ��λ��
};
struct eular_angle {
    double roll;
    double pitch;
    double yaw;
};

/*
* imu_reset_quat - ��ʼ��׼
*
* @param: ���в���
*/
void imu_init_param(struct flight_parameters *param);
/*
 * imu_update_quat - ������Ԫ��
 *
 * @ag: imu����
 * @param: ���в���
 * @dt: ʱ����
 */
void imu_update_quat(struct imu_6x_double *ag, struct flight_parameters *param, double dt);
/*
* imu_update_xyz - ������ת����,λ�û���
*
* @ag: imu����
* @param: ���в���
* @dt: ʱ����
*/
void imu_update_xyz(const struct imu_6x_double *ag, struct flight_parameters *param, double dt);
/*
 * imu_get_rotate_matrix - ������ת����
 *
 * @param: ���в���
 * @m: ��ת����
 */
void imu_get_rotate_matrix(const struct flight_parameters *param, double *m);
/*
 * imu_get_euler_angle - ����ŷ����,���ڸ�������ת����֮�����
 *
 * @param: ���в���
 * @m: ŷ����
 */
void imu_get_euler_angle(const struct flight_parameters *param, struct eular_angle *rpy);
