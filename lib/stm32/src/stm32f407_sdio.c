#include <stm32f407_sdio.h>
#include <stm32f407_dma.h>

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
 * sdio_init_clkcr - ��ʼ��ʱ�ӿ��ƼĴ���
 *
 * ��STM32F407�У�SDIO������ʱ��SDIOCLKƵ��Ϊ48MHz����PLLֱ��������
 * ��������£������SDIO_CKƵ�ʲ��ܳ���400KHz
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


