#include <stm32f407.h>
#include <xtos.h>

#define KEY (GPIOH->IDR.bits.pin15)
#define LED_R (GPIOI->ODR.bits.pin5)
#define LED_G (GPIOI->ODR.bits.pin6)
#define LED_B (GPIOI->ODR.bits.pin7)
#define ON 0
#define OFF 1

void init_led() {
    RCC->AHB1ENR.bits.gpioi = 1;

    GPIOI->MODER.bits.pin5 = GPIO_Mode_Out;
    GPIOI->MODER.bits.pin6 = GPIO_Mode_Out;
    GPIOI->MODER.bits.pin7 = GPIO_Mode_Out;

    GPIOI->OTYPER.bits.pin5 = GPIO_OType_PP;
    GPIOI->OTYPER.bits.pin6 = GPIO_OType_PP;
    GPIOI->OTYPER.bits.pin7 = GPIO_OType_PP;

    GPIOI->PUPDR.bits.pin5 = GPIO_Pull_Up;
    GPIOI->PUPDR.bits.pin6 = GPIO_Pull_Up;
    GPIOI->PUPDR.bits.pin7 = GPIO_Pull_Up;

    GPIOI->OSPEEDR.bits.pin5 = GPIO_OSpeed_Very_High;
    GPIOI->OSPEEDR.bits.pin6 = GPIO_OSpeed_Very_High;
    GPIOI->OSPEEDR.bits.pin7 = GPIO_OSpeed_Very_High;
}

void delay(int c) {
    for (int i = 0; i < c; i++);
}

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024

static struct xtos_task_pcb taskA;
static struct xtos_task_pcb taskB;
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

void task_switch() {
    if (gp_xtos_cur_task == &taskA)
        gp_xtos_next_task = &taskB;
    else
        gp_xtos_next_task = &taskA;

    XTOS_Ctx_Sw();
}

void taska() {
    while (1) {
        LED_R = ON;
        LED_G = OFF;
        LED_B = OFF;
        for (int i = 0; i < 1000; i++)
            delay(10000);
        task_switch();
    }
}

void taskb() {
    while (1) {
        LED_R = OFF;
        LED_G = ON;
        LED_B = OFF;
        for (int i = 0; i < 1000; i++)
            delay(10000);
        task_switch();
    }
}

void task_end() {
    LED_R = OFF;
    LED_G = OFF;
    LED_B = ON;

    while (1) {

    }
}

void task_create(struct xtos_task_pcb *tcb, xtos_task task, uint32 *stk) {
    uint32  *pstk;
    pstk = stk;
    pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);

    *(--pstk) = (uint32)0x00000000L; //No Name Register  
    *(--pstk) = (uint32)0x00001000L; //FPSCR
    *(--pstk) = (uint32)0x00000015L; //s15
    *(--pstk) = (uint32)0x00000014L; //s14
    *(--pstk) = (uint32)0x00000013L; //s13
    *(--pstk) = (uint32)0x00000012L; //s12
    *(--pstk) = (uint32)0x00000011L; //s11
    *(--pstk) = (uint32)0x00000010L; //s10
    *(--pstk) = (uint32)0x00000009L; //s9
    *(--pstk) = (uint32)0x00000008L; //s8
    *(--pstk) = (uint32)0x00000007L; //s7
    *(--pstk) = (uint32)0x00000006L; //s6
    *(--pstk) = (uint32)0x00000005L; //s5
    *(--pstk) = (uint32)0x00000004L; //s4
    *(--pstk) = (uint32)0x00000003L; //s3
    *(--pstk) = (uint32)0x00000002L; //s2
    *(--pstk) = (uint32)0x00000001L; //s1
    *(--pstk) = (uint32)0x00000000L; //s0

    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)task_end;     // R14 (LR)
    *(--pstk) = (uint32)0x12121212uL; // R12
    *(--pstk) = (uint32)0x03030303uL; // R3
    *(--pstk) = (uint32)0x02020202uL; // R2
    *(--pstk) = (uint32)0x01010101uL; // R1
    *(--pstk) = (uint32)0x00000000u;  // R0

    *(--pstk) = (uint32)0x00000031L; //s31
    *(--pstk) = (uint32)0x00000030L; //s30
    *(--pstk) = (uint32)0x00000029L; //s29
    *(--pstk) = (uint32)0x00000028L; //s28
    *(--pstk) = (uint32)0x00000027L; //s27
    *(--pstk) = (uint32)0x00000026L; //s26	
    *(--pstk) = (uint32)0x00000025L; //s25
    *(--pstk) = (uint32)0x00000024L; //s24
    *(--pstk) = (uint32)0x00000023L; //s23
    *(--pstk) = (uint32)0x00000022L; //s22
    *(--pstk) = (uint32)0x00000021L; //s21
    *(--pstk) = (uint32)0x00000020L; //s20
    *(--pstk) = (uint32)0x00000019L; //s19
    *(--pstk) = (uint32)0x00000018L; //s18
    *(--pstk) = (uint32)0x00000017L; //s17
    *(--pstk) = (uint32)0x00000016L; //s16

    *(--pstk) = (uint32)0x11111111uL; // R11
    *(--pstk) = (uint32)0x10101010uL; // R10
    *(--pstk) = (uint32)0x09090909uL; // R9
    *(--pstk) = (uint32)0x08080808uL; // R8
    *(--pstk) = (uint32)0x07070707uL; // R7
    *(--pstk) = (uint32)0x06060606uL; // R6
    *(--pstk) = (uint32)0x05050505uL; // R5
    *(--pstk) = (uint32)0x04040404uL; // R4

    tcb->pTopOfStack = pstk;
}


int main(void) {
    init_led();

    GPIOI->ODR.bits.pin5 = 1;
    GPIOI->ODR.bits.pin6 = 1;
    GPIOI->ODR.bits.pin7 = 1;
    
    task_create(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    task_create(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    XTOS_Start_ASM();

    while(1) {

    }
}
