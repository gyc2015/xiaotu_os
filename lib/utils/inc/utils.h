#ifndef UTILS_H
#define UTILS_H

#include <types.h>

/*
 * 切换大小端数据
 */
uint32 change_ending_32b(uint32 data);
uint16 change_ending_16b(uint16 data);
uint16 combine_bytes(uint8 h, uint8 l);

void swap_double(double *a, double *b);

void delay(int n);


#endif
