#include <stm32f407_sdio.h>
#include <stm32f407_dma.h>

#define SDIO_FIFO_ADDRESS ((uint32)0x40012C80)

void sdio_send_cmd(union sdio_cmd cmd, uint32 arg) {
    // 写入指令参数
    SDIO->ARG = arg;
    // 写入指令
    SDIO->CMD = cmd;
}

void sdio_config_data(union sdio_dctrl dctrl, uint32 timeout, uint32 dlen) {
    SDIO->DTIMER = timeout;
    SDIO->DLEN.bits.datalen = dlen;
    SDIO->DCTRL = dctrl;
}

/*
 * sdio_init_clkcr - 初始化时钟控制寄存器
 *
 * 在STM32F407中，SDIO的驱动时钟SDIOCLK频率为48MHz，由PLL直接驱动。
 * 正常情况下，输出的SDIO_CK频率不能超过400KHz
 * SDIO_CK = SDIOCLK / (CLKDIV + 2)
 */
void sdio_init_clkcr(uint8 clkdiv, uint8 buswid) {
    SDIO->CLKCR.bits.CLKDIV = clkdiv;
    SDIO->CLKCR.bits.NEGEDGE = FALSE;
    SDIO->CLKCR.bits.BYPASS = FALSE;
    SDIO->CLKCR.bits.PWRSAV = FALSE;
    SDIO->CLKCR.bits.WIDBUS = buswid;
    SDIO->CLKCR.bits.HWFC_EN = FALSE;
}

void sdio_config_dma(uint32 *dbuf, uint32 bufsize) {
    // 清除中断标识
    DMA2->LIFCR.bits.FEIF3 = 1;
    DMA2->LIFCR.bits.DMEIF3 = 1;
    DMA2->LIFCR.bits.TEIF3 = 1;
    DMA2->LIFCR.bits.HTIF3 = 1;
    DMA2->LIFCR.bits.TCIF3 = 1;
    DMA2_Stream3->CR.bits.EN = 0;
    DMA_ResetStream(DMA2_Stream3);
    // 
    DMA2_Stream3->CR.bits.CHSEL = 4;            // 通道选择
    DMA2_Stream3->CR.bits.DIR = DMA_DIR_P2M;    // 传输方向
    DMA2_Stream3->CR.bits.CIRC = 0;             // 关闭循环模式
    DMA2_Stream3->CR.bits.PL = DMA_Priority_Very_High;// 优先级
    DMA2_Stream3->CR.bits.PINC = 0;             // 外设增长
    DMA2_Stream3->CR.bits.PSIZE = DMA_PSIZE_32Bits; // 外设数据宽度
    DMA2_Stream3->CR.bits.MINC = 1;             // 内存增长
    DMA2_Stream3->CR.bits.MSIZE = DMA_PSIZE_32Bits;
    DMA2_Stream3->CR.bits.MBURST = DMA_Burst_4;
    DMA2_Stream3->CR.bits.PBURST = DMA_Burst_4;
    //
    DMA2_Stream3->FCR.bits.DMDIS = 1;           // 保持Direct Mode
    DMA2_Stream3->FCR.bits.FTH = DMA_FIFO_4;
    //
    DMA2_Stream3->PAR = SDIO_FIFO_ADDRESS;
    DMA2_Stream3->M0AR = (uint32)dbuf;
    DMA2_Stream3->NDTR.all = bufsize;
    // 中断,流控
    DMA2_Stream3->CR.bits.TCIE = 1;
    DMA2_Stream3->CR.bits.PFCTRL = 1;
    // 使能
    DMA2_Stream3->CR.bits.EN = 1;
}


