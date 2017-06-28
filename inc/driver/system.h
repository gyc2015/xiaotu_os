#pragma once

#include <types.h>

extern uint32 sys_ms;

/* Ã¿ºÁÃë¼ÆÊý */
#define SYS_TICK_PMS   168000

struct sys_time {
    uint32 ms;
    uint32 us;
};

struct sys_duration {
    int32 ms;
    int32 us;
};

void sys_init(void);
void sys_get_time(struct sys_time *time);
void sys_time_diff(const struct sys_time *src, const struct sys_time *dst, struct sys_duration *duration);
int32 sys_time_diff_us(const struct sys_time *src, const struct sys_time *dst);
double sys_time_diff_fms(const struct sys_time *src, const struct sys_time *dst);
void sys_time_dis(const struct sys_time *origin, struct sys_duration *duration);
int32 sys_time_dis_us(const struct sys_time *origin);
double sys_time_dis_fms(const struct sys_time *origin);
void sys_delay_ms(uint32 ms);
void sys_delay_us(uint32 us);
void sys_delay_fms(double fms);

