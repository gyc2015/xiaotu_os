#include <stm32f407_sdio.h>
#include <stm32f407_dma.h>
#include <stm32f407_gpio.h>
#include <stm32f407_rcc.h>

#define SDIO_FIFO_ADDRESS ((uint32)0x40012C80)

void sdio_send_cmd(union sdio_cmd cmd, uint32 arg) {
    // д��ָ�����
    SDIO->ARG = arg;
    // д��ָ��
    SDIO->CMD = cmd;
}

void sdio_config_data(union sdio_dctrl dctrl, uint32 timeout, uint32 dlen) {
    SDIO->DTIMER = timeout;
    SDIO->DLEN.bits.datalen = dlen;
    SDIO->DCTRL = dctrl;
}

/*
 * sdio_init_hw - ��ʼ��SDIO��Ӳ������
 *
 * ����PC8, PC9, PC10, PC11������SDIO_D[0:3], ����, ����
 * ����PD2������SDIO_CMD, ����, ����
 * ����PC12������SDIO_CK, ����, ����
 * ����APB2�����ϵ�SDIO����ʱ��
 * ����DMA2������ʱ��
 */
void sdio_init_hw(void) {
    // ʱ��
    RCC->AHB1ENR.bits.gpioc = 1;
    RCC->AHB1ENR.bits.gpiod = 1;
    RCC->APB2ENR.bits.sdio = 1;
    RCC->AHB1ENR.bits.dma2 = 1;
    // ����ѡ��
    GPIOC->AFR.bits.pin8 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin9 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin10 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin11 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin12 = GPIO_AF_SDIO;
    GPIOD->AFR.bits.pin2 = GPIO_AF_SDIO;

    struct gpio_pin_conf pincof;
    // PC8, PC9, PC10, PC11 ==> D0, D1, D2, D3
    pincof.mode = GPIO_Mode_Af;
    pincof.otype = GPIO_OType_PP;
    pincof.speed = GPIO_OSpeed_Medium;
    pincof.pull = GPIO_Pull_Up;
    gpio_init(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, &pincof);
    // PD2 ==> CMD
    gpio_init(GPIOD, GPIO_Pin_2, &pincof);
    // PC12 ==> CLK
    pincof.pull = GPIO_Pull_No;
    gpio_init(GPIOC, GPIO_Pin_12, &pincof);
}

/*
 * sdio_init_clkcr - ��ʼ��ʱ�ӿ��ƼĴ���
 *
 * ��STM32F407�У�SDIO������ʱ��SDIOCLKƵ��Ϊ48MHz����PLLֱ��������
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

void sdio_config_dma_rx(uint32 *dbuf, uint32 bufsize) {
    // ����жϱ�ʶ
    DMA2->LIFCR.bits.FEIF3 = 1;
    DMA2->LIFCR.bits.DMEIF3 = 1;
    DMA2->LIFCR.bits.TEIF3 = 1;
    DMA2->LIFCR.bits.HTIF3 = 1;
    DMA2->LIFCR.bits.TCIF3 = 1;
    DMA2_Stream3->CR.bits.EN = 0;
    DMA_ResetStream(DMA2_Stream3);
    // 
    DMA2_Stream3->CR.bits.CHSEL = 4;            // ͨ��ѡ��
    DMA2_Stream3->CR.bits.DIR = DMA_DIR_P2M;    // ���䷽��
    DMA2_Stream3->CR.bits.CIRC = 0;             // �ر�ѭ��ģʽ
    DMA2_Stream3->CR.bits.PL = DMA_Priority_Very_High;// ���ȼ�
    DMA2_Stream3->CR.bits.PINC = 0;             // ��������
    DMA2_Stream3->CR.bits.PSIZE = DMA_PSIZE_32Bits; // �������ݿ��
    DMA2_Stream3->CR.bits.MINC = 1;             // �ڴ�����
    DMA2_Stream3->CR.bits.MSIZE = DMA_PSIZE_32Bits;
    DMA2_Stream3->CR.bits.MBURST = DMA_Burst_4;
    DMA2_Stream3->CR.bits.PBURST = DMA_Burst_4;
    //
    DMA2_Stream3->FCR.bits.DMDIS = 1;           // ����Direct Mode
    DMA2_Stream3->FCR.bits.FTH = DMA_FIFO_4;
    //
    DMA2_Stream3->PAR = SDIO_FIFO_ADDRESS;
    DMA2_Stream3->M0AR = (uint32)dbuf;
    DMA2_Stream3->NDTR.all = bufsize;
    // �ж�,����
    DMA2_Stream3->CR.bits.TCIE = 1;
    DMA2_Stream3->CR.bits.PFCTRL = 1;
    // ʹ��
    DMA2_Stream3->CR.bits.EN = 1;
}

void sdio_config_dma_tx(const uint32 *dbuf, uint32 bufsize) {
    // ����жϱ�ʶ
    DMA2->LIFCR.bits.FEIF3 = 1;
    DMA2->LIFCR.bits.DMEIF3 = 1;
    DMA2->LIFCR.bits.TEIF3 = 1;
    DMA2->LIFCR.bits.HTIF3 = 1;
    DMA2->LIFCR.bits.TCIF3 = 1;
    DMA2_Stream3->CR.bits.EN = 0;
    DMA_ResetStream(DMA2_Stream3);
    // 
    DMA2_Stream3->CR.bits.CHSEL = 4;            // ͨ��ѡ��
    DMA2_Stream3->CR.bits.DIR = DMA_DIR_M2P;    // ���䷽��
    DMA2_Stream3->CR.bits.CIRC = 0;             // �ر�ѭ��ģʽ
    DMA2_Stream3->CR.bits.PL = DMA_Priority_Very_High;// ���ȼ�
    DMA2_Stream3->CR.bits.PINC = 0;             // ��������
    DMA2_Stream3->CR.bits.PSIZE = DMA_PSIZE_32Bits; // �������ݿ��
    DMA2_Stream3->CR.bits.MINC = 1;             // �ڴ�����
    DMA2_Stream3->CR.bits.MSIZE = DMA_PSIZE_32Bits;
    DMA2_Stream3->CR.bits.MBURST = DMA_Burst_4;
    DMA2_Stream3->CR.bits.PBURST = DMA_Burst_4;
    //
    DMA2_Stream3->FCR.bits.DMDIS = 1;           // ����FIFO Mode
    DMA2_Stream3->FCR.bits.FTH = DMA_FIFO_4;
    //
    DMA2_Stream3->PAR = SDIO_FIFO_ADDRESS;
    DMA2_Stream3->M0AR = (uint32)dbuf;
    DMA2_Stream3->NDTR.all = bufsize;
    // �ж�,����
    DMA2_Stream3->CR.bits.TCIE = 1;
    DMA2_Stream3->CR.bits.PFCTRL = 1;
    // ʹ��
    DMA2_Stream3->CR.bits.EN = 1;
}

void sdio_enable_interrupts(void) {
    SDIO->MASK.bits.dcrcfaile = 1;
    SDIO->MASK.bits.dtimeoute = 1;
    SDIO->MASK.bits.dataende = 1;
    SDIO->MASK.bits.rxoverre = 1;
    SDIO->MASK.bits.stbiterre = 1;
}

void sdio_disable_interrupts(void) {
    SDIO->MASK.bits.dcrcfaile = 0;
    SDIO->MASK.bits.dtimeoute = 0;
    SDIO->MASK.bits.dataende = 0;
    SDIO->MASK.bits.rxoverre = 0;
    SDIO->MASK.bits.stbiterre = 0;
}

