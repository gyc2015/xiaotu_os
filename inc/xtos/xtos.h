#ifndef XTOS_H
#define XTOS_H

#include <types.h>
#include <list.h>

// xtos任务入口
typedef void(*xtos_task)(void);

/*
 * xtos_task_struct - 任务描述符
 */
struct xtos_task_descriptor {
    uint32 *pTopOfStack;    /* 栈顶地址，该位段不可以更改 */
    uint32 *pBottomOfStack;   /* 栈底地址 */
    uint16 pid;
    struct list_head list;
};

void xtos_init(uint32 ticks);
void xtos_start(void);
void xtos_schedule(void);

/*
 * xtos_init_task_struct - 创建一个任务，初始化任务栈空间
 *
 * @tcb: 任务描述符
 * @task: 任务入口函数
 * @stk_bottom: 任务栈底
 * @pid: 任务id
 */
void xtos_init_task_struct(struct xtos_task_descriptor *tcb, xtos_task task, uint32 *stk_bottom, uint16 pid);
void xtos_distroy_task(void);

uint32 xtos_get_ms(void);

#endif // !XTOS_H
