#include <xtos.h>
#include <xtos_time.h>
#include <stm32f407.h>

uint32 xtos_ms = 0;
/*
* xtos_get_time - 获取当前操作系统运行时间
*/
void xtos_get_time(struct xtos_time *time) {
    time->ms = xtos_ms;
    uint32 tmp = XTOS_TICK_PMS - SYSTICK->VAL.bits.cnt;
    time->us = (uint32)((1000 * tmp) / XTOS_TICK_PMS);
}

void xtos_time_diff(const struct xtos_time *src, const struct xtos_time *dst, struct xtos_duration *duration) {
    duration->us = dst->us - src->us;
    duration->ms = dst->ms - src->ms;
}

int32 xtos_time_diff_us(const struct xtos_time *src, const struct xtos_time *dst) {
    struct xtos_duration duration;
    xtos_time_diff(src, dst, &duration);
    return (1000 * duration.ms + duration.us);
}

double xtos_time_diff_fms(const struct xtos_time *src, const struct xtos_time *dst) {
    struct xtos_duration duration;
    xtos_time_diff(src, dst, &duration);
    return (0.001 * duration.us + 1.0 * duration.ms);
}

void xtos_time_dis(const struct xtos_time *origin, struct xtos_duration *duration) {
    struct xtos_time current;
    xtos_get_time(&current);
    xtos_time_diff(origin, &current, duration);
}

int32 xtos_time_dis_us(const struct xtos_time *origin) {
    struct xtos_time current;
    xtos_get_time(&current);
    return xtos_time_diff_us(origin, &current);
}

double xtos_time_dis_fms(const struct xtos_time *origin) {
    struct xtos_time current;
    xtos_get_time(&current);
    return xtos_time_diff_fms(origin, &current);
}

void xtos_delay_ms(uint32 ms) {
    uint32 origin = xtos_ms;
    uint32 delta = xtos_ms - origin;

    while (delta < ms) {
        delta = xtos_ms - origin;
    }
}

void xtos_delay_us(uint32 us) {
    struct xtos_time origin;
    xtos_get_time(&origin);

    double delta = xtos_time_dis_us(&origin);
    while (delta < us) {
        delta = xtos_time_dis_us(&origin);
    }

}

void xtos_delay_fms(double fms) {
    struct xtos_time origin;
    xtos_get_time(&origin);

    double delta = xtos_time_dis_fms(&origin);
    while (delta < fms) {
        delta = xtos_time_dis_fms(&origin);
    }
}
