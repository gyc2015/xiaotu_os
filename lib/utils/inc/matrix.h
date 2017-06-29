#pragma once

#include <utils.h>


/* w = u + v */
void vector3d_add(const double *u, const double *v, double *w);
/* u += v */
void vector3d_add_(double *u, const double *v);
/* w = u - v */
void vector3d_sub(const double *u, const double *v, double *w);
/* u -= v*/
void vector3d_sub_(double *u, const double *v);
/* u µã³Ë v */
double vector3d_dmul(const double *u, const double *v);
/* u ²æ³Ë v */
void vector3d_xmul(const double * u, const double *v, double *w);
/* v = cu */
void vector3d_mul(const double *u, double c, double *v);
/* u *= c */
void vector3d_mul_(double *u, double c);
/* out = mv */
void matrix3d_lmul_vector(const double *m, const double *v, double *out);
/* m = m^T */
void matrix3d_transpose(double *m);


