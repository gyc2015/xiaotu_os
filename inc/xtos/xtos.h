#ifndef XTOS_H
#define XTOS_H

#include <types.h>

// xtos的栈空间单元
typedef uint32 xtos_stack_cell;
// xtos任务入口
typedef void(*xtos_task)(void);

struct xtos_task_pcb {
    xtos_stack_cell *pTopOfStack;   /* 栈顶地址 */
};

extern struct xtos_task_pcb *gp_xtos_cur_task;
extern struct xtos_task_pcb *gp_xtos_next_task;

void XTOS_Start_ASM(void);
void XTOS_Ctx_Sw(void);
void XTOS_PendSV_Handler(void);

#endif // !XTOS_H
