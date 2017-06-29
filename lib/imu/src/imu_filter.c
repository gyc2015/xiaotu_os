
/*
* imu_reset_quat - 四元素初始对准

void imu_reset_quat(struct quaternion *quat, double kp, double ki) {
    quat->q[0] = 1;
    quat->q[1] = 0;
    quat->q[2] = 0;
    quat->q[3] = 0;
    quat->ex = 0;
    quat->ey = 0;
    quat->ez = 0;
    quat->kp = kp;
    quat->ki = ki;
}

#define Q(i) (quat->q[i])
#define EX  (quat->ex)
#define EY  (quat->ey)
#define EZ  (quat->ez)
#define KP  (quat->kp)
#define KI  (quat->ki)
void imu_update_quat(const struct imu_6x_double *ag, struct quaternion *quat, double hdt) {
    double ax = ag->ax;
    double ay = ag->ay;
    double az = ag->az;
    double gx = ag->gx;
    double gy = ag->gy;
    double gz = ag->gz;

    double norm = 1.0 / sqrt(ax*ax + ay*ay + az*az);
    ax *= norm;
    ay *= norm;
    az *= norm;

    double vx = 2 * (Q(1)*Q(3) - Q(0)*Q(2));
    double vy = 2 * (Q(0)*Q(1) + Q(2)*Q(3));
    double vz = Q(0)*Q(0) - Q(1)*Q(1) - Q(2)*Q(2) + Q(3)*Q(3);

    double ex = ay * vz - az * vy;
    double ey = az * vx - ax * vz;
    double ez = ax * vy - ay * vx;

    EX += ex * KI;
    EY += ey * KI;
    EZ += ez * KI;

    gx += ex * KP + EX;
    gy += ey * KP + EY;
    gz += ez * KP + EZ;

    Q(0) += (-Q(1)*gx - Q(2)*gy - Q(3)*gz)*hdt;
    Q(1) += (Q(0)*gx + Q(2)*gz - Q(3)*gy)*hdt;
    Q(2) += (Q(0)*gy - Q(1)*gz + Q(3)*gx)*hdt;
    Q(3) += (Q(0)*gz + Q(1)*gy - Q(2)*gx)*hdt;

    norm = 1 / sqrt(Q(0)*Q(0) + Q(1)*Q(1) + Q(2)*Q(2) + Q(3)*Q(3));
    Q(0) *= norm;
    Q(1) *= norm;
    Q(2) *= norm;
    Q(3) *= norm;
}

void imu_get_euler_angle(const struct quaternion *quat, struct eular_angle *rpy) {
    rpy->roll = atan2(2 * (Q(0)*Q(1) + Q(2)*Q(3)), 1 - 2 * (Q(1)*Q(1) + Q(2)*Q(2))) * 180 / 3.1415926;
    rpy->pitch = asin(2 * (Q(1)*Q(3) - Q(0)*Q(2))) * 180 / 3.1415926;
    rpy->yaw = atan2(2 * (Q(1)*Q(2) + Q(0)*Q(3)), Q(0)*Q(0) + Q(1)*Q(1) - Q(2)*Q(2) - Q(3)*Q(3)) * 180 / 3.1415926;
}

*/
