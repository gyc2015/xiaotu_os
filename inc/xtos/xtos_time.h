#pragma once

#include <types.h>

extern uint32 xtos_ms;

void xtos_get_time(struct xtos_time *time);
void xtos_time_diff(const struct xtos_time *src, const struct xtos_time *dst, struct xtos_duration *duration);
int32 xtos_time_diff_us(const struct xtos_time *src, const struct xtos_time *dst);
double xtos_time_diff_fms(const struct xtos_time *src, const struct xtos_time *dst);
void xtos_time_dis(const struct xtos_time *origin, struct xtos_duration *duration);
int32 xtos_time_dis_us(const struct xtos_time *origin);
double xtos_time_dis_fms(const struct xtos_time *origin);
void xtos_delay_ms(uint32 ms);
void xtos_delay_us(uint32 us);
void xtos_delay_fms(double fms);


