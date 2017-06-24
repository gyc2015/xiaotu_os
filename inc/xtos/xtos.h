#ifndef XTOS_H
#define XTOS_H

#include <types.h>
#include <list.h>

// xtos�������
typedef void(*xtos_task)(void);

/*
 * xtos_task_struct - ����������
 */
struct xtos_task_descriptor {
    uint32 *pTopOfStack;    /* ջ����ַ����λ�β����Ը��� */
    uint32 *pBottomOfStack;   /* ջ�׵�ַ */
    uint16 pid;
    struct list_head list;
};

void xtos_init(uint32 ticks);
void xtos_start(void);
void xtos_schedule(void);

/*
 * xtos_init_task_struct - ����һ�����񣬳�ʼ������ջ�ռ�
 *
 * @tcb: ����������
 * @task: ������ں���
 * @stk_bottom: ����ջ��
 * @pid: ����id
 */
void xtos_init_task_struct(struct xtos_task_descriptor *tcb, xtos_task task, uint32 *stk_bottom, uint16 pid);
void xtos_distroy_task(void);

uint32 xtos_get_ms(void);

#endif // !XTOS_H
