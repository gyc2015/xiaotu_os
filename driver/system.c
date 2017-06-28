#include <system.h>
#include <xtos.h>
#include <stm32f407.h>

uint32 sys_ms = 0;


#define SysTicks_Irq_n 15

void sys_init(void) {
    SYSTICK->LOAD.bits.cnt = SYS_TICK_PMS; // 重装载数据
    SYSTICK->VAL.bits.cnt = 0;              // 清除当前计数

    SCB->SHP[SysTicks_Irq_n - 4] = 0x00;    // 暂时赋予最高优先级

    SYSTICK->CTRL.bits.clksource = 1;       // 使用CPU时钟168MHz
    SYSTICK->CTRL.bits.tickint = 1;         // 开启计数溢出中断
    SYSTICK->CTRL.bits.en = 1;              // 暂不开启计时器
}
/*
* xtos_get_time - 获取当前操作系统运行时间
*/
void sys_get_time(struct sys_time *time) {
    time->ms = sys_ms;
    uint32 tmp = SYS_TICK_PMS - SYSTICK->VAL.bits.cnt;
    time->us = (uint32)((1000 * tmp) / SYS_TICK_PMS);
}

void sys_time_diff(const struct sys_time *src, const struct sys_time *dst, struct sys_duration *duration) {
    duration->us = dst->us - src->us;
    duration->ms = dst->ms - src->ms;
}

int32 sys_time_diff_us(const struct sys_time *src, const struct sys_time *dst) {
    struct sys_duration duration;
    sys_time_diff(src, dst, &duration);
    return (1000 * duration.ms + duration.us);
}

double sys_time_diff_fms(const struct sys_time *src, const struct sys_time *dst) {
    struct sys_duration duration;
    sys_time_diff(src, dst, &duration);
    return (0.001 * duration.us + 1.0 * duration.ms);
}

void sys_time_dis(const struct sys_time *origin, struct sys_duration *duration) {
    struct sys_time current;
    sys_get_time(&current);
    sys_time_diff(origin, &current, duration);
}

int32 sys_time_dis_us(const struct sys_time *origin) {
    struct sys_time current;
    sys_get_time(&current);
    return sys_time_diff_us(origin, &current);
}

double sys_time_dis_fms(const struct sys_time *origin) {
    struct sys_time current;
    sys_get_time(&current);
    return sys_time_diff_fms(origin, &current);
}

void sys_delay_ms(uint32 ms) {
    uint32 origin = sys_ms;
    uint32 delta = sys_ms - origin;

    while (delta < ms) {
        delta = sys_ms - origin;
    }
}

void sys_delay_us(uint32 us) {
    struct sys_time origin;
    sys_get_time(&origin);

    int32 delta = sys_time_dis_us(&origin);
    while (delta < us) {
        delta = sys_time_dis_us(&origin);
    }

}

void sys_delay_fms(double fms) {
    struct sys_time origin;
    sys_get_time(&origin);

    double delta = sys_time_dis_fms(&origin);
    while (delta < fms) {
        delta = sys_time_dis_fms(&origin);
    }
}

void SysTick_Handler(void) {
    sys_ms++;

    if (XTOS_STATED == xtos_state)
        xtos_schedule();
}



