#include <xtos.h>
#include <xtos_time.h>
#include <stm32f407.h>



void xtos_delay_ms(uint32 ms) {
    uint32 origin = sys_ms;
    uint32 delta = sys_ms - origin;

    while (delta < ms) {
        xtos_schedule();
        delta = sys_ms - origin;
    }
}

void xtos_delay_us(uint32 us) {
    struct sys_time origin;
    sys_get_time(&origin);

    double delta = sys_time_dis_us(&origin);
    while (delta < us) {
        xtos_schedule();
        delta = sys_time_dis_us(&origin);
    }

}

void xtos_delay_fms(double fms) {
    struct sys_time origin;
    sys_get_time(&origin);

    double delta = sys_time_dis_fms(&origin);
    while (delta < fms) {
        xtos_schedule();
        delta = sys_time_dis_fms(&origin);
    }
}
