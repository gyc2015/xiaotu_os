#include <matrix.h>

/* w = u + v */
void vector3d_add(const double *u, const double *v, double *w) {
    w[0] = u[0] + v[0];
    w[1] = u[1] + v[1];
    w[2] = u[2] + v[2];
}
/* u += v */
void vector3d_add_(double *u, const double *v) {
    u[0] += v[0];
    u[1] += v[1];
    u[2] += v[2];
}
/* w = u - v */
void vector3d_sub(const double *u, const double *v, double *w) {
    w[0] = u[0] - v[0];
    w[1] = u[1] - v[1];
    w[2] = u[2] - v[2];
}
/* u -= v*/
void vector3d_sub_(double *u, const double *v) {
    u[0] -= v[0];
    u[0] -= v[0];
    u[0] -= v[0];
}
/* u µã³Ë v */
double vector3d_dmul(const double *u, const double *v) {
    return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}
/* u ²æ³Ë v */
void vector3d_xmul(const double * u, const double *v, double *w) {
    w[0] = u[1] * v[2] - u[2] * v[1];
    w[1] = u[2] * v[0] - u[0] * v[2];
    w[2] = u[0] * v[1] - u[1] * v[0];
}
/* v = cu */
void vector3d_mul(const double *u, double c, double *v) {
    v[0] = c * u[0];
    v[1] = c * u[1];
    v[2] = c * u[2];
}
/* u *= c */
void vector3d_mul_(double *u, double c) {
    u[0] *= c;
    u[1] *= c;
    u[2] *= c;
}
/* out = mv */
void matrix3d_lmul_vector(const double *m, const double *v, double *out) {
    out[0] = vector3d_dmul(&m[0], v);
    out[1] = vector3d_dmul(&m[3], v);
    out[2] = vector3d_dmul(&m[6], v);
}
/* m = m^T */
void matrix3d_transpose(double *m) {
    swap_double(&m[1 + 0 * 3], &m[0 + 1 * 3]);
    swap_double(&m[2 + 0 * 3], &m[0 + 2 * 3]);
    swap_double(&m[2 + 1 * 3], &m[1 + 2 * 3]);
}



