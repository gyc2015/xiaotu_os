#include <stm32f407.h>
#include <xtos.h>
#include <led.h>
#include <uart4.h>
#include <stm32f407_dma.h>

#define KEY (GPIOH->IDR.bits.pin15)

void delay(int c) {
    for (int i = 0; i < c; i++);
}

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_struct taskA;
static struct xtos_task_struct taskB;

void task_switch() {
    if (gp_xtos_cur_task == &taskA)
        gp_xtos_next_task = &taskB;
    else
        gp_xtos_next_task = &taskA;

    xtos_context_switch();
}

void taska() {
    while (1) {
        LED_R = LED_ON;
        LED_G = LED_OFF;
        LED_B = LED_OFF;
        uart4_send_byte('A');
    }
}

void taskb() {
    while (1) {
        LED_R = LED_OFF;
        LED_G = LED_ON;
        LED_B = LED_OFF;
        uart4_send_byte('B');
    }
}

uint32 src_buffer[32] = {
    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80 };

uint32 dst_buffer[32] = { 0 };


void dma_init() {
    RCC->AHB1ENR.bits.dma2 = 1;
    DMA_ResetStream(DMA2_Stream0);

    DMA2_Stream0->CR.bits.CHSEL = 0;            // 通道选择
    DMA2_Stream0->CR.bits.DIR = DMA_DIR_M2M;    // 传输方向
    DMA2_Stream0->CR.bits.CIRC = 0;             // 关闭循环模式
    DMA2_Stream0->CR.bits.PL = DMA_Priority_High;// 优先级
    DMA2_Stream0->CR.bits.PINC = 1;             // 外设增长
    DMA2_Stream0->CR.bits.PSIZE = DMA_PSIZE_32Bits; // 外设数据宽度
    DMA2_Stream0->CR.bits.MINC = 1;             // 内存增长
    DMA2_Stream0->CR.bits.MSIZE = DMA_PSIZE_32Bits;
    DMA2_Stream0->CR.bits.MBURST = DMA_Burst_0;
    DMA2_Stream0->CR.bits.PBURST = DMA_Burst_0;

    DMA2_Stream0->FCR.bits.DMDIS = 0;           // 保持Direct Mode
    DMA2_Stream0->FCR.bits.FTH = DMA_FIFO_4;

    DMA2_Stream0->PAR = (uint32)src_buffer;
    DMA2_Stream0->M0AR = (uint32)dst_buffer;
    DMA2_Stream0->NDTR.all = 32;

    DMA2_Stream0->CR.bits.EN = 1;
}

#define SysTicks_Irq_n 15

void systick_init(uint32 ticks) {
    SYSTICK->LOAD.bits.cnt = ticks;         // 重装载数据
    SYSTICK->VAL.bits.cnt = 0;              // 清除当前计数

    SCB->SHP[SysTicks_Irq_n - 4] = 0x00;    // 暂时赋予最高优先级

    SYSTICK->CTRL.bits.clksource = 1;       // 使用CPU时钟168MHz
    SYSTICK->CTRL.bits.tickint = 1;         // 开启计数溢出中断
    SYSTICK->CTRL.bits.en = 0;              // 暂不开启计时器
}

void SysTick_Handler(void) {
    static int counter = 0;

    if ((counter++ % 1000) == 0) {
        task_switch();
    }
}


int main(void) {
    led_init();
    uart4_init(256000);
    systick_init(168000);
    dma_init();

    while (!(0x01 && DMA2_Stream0->CR.all));

    for (int i = 0; i < 32; i++) {
        if (src_buffer[i] != dst_buffer[i])
            while(1){}
    }

    xtos_create_task(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1]);
    xtos_create_task(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1]);

    gp_xtos_next_task = &taskA;

    SYSTICK->CTRL.bits.en = 1;
    xtos_start();

    while(1) {

    }
}
