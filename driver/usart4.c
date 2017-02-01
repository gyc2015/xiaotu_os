#include <stm32f407.h>
#include <uart4.h>

/*
 * uart4_init_gpio - ��ʼ���˿�����
 */
static void uart4_init_gpio(void) {
    RCC->AHB1ENR.bits.gpioa = 1;
    // PA0 : tx
    GPIOA->MODER.bits.pin0 = GPIO_Mode_Af;
    GPIOA->OTYPER.bits.pin0 = GPIO_OType_PP;
    GPIOA->PUPDR.bits.pin0 = GPIO_Pull_Up;
    GPIOA->OSPEEDR.bits.pin0 = GPIO_OSpeed_High;
    // PA1 : rx
    GPIOA->MODER.bits.pin1 = GPIO_Mode_Af;
    GPIOA->PUPDR.bits.pin1 = GPIO_Pull_No;
    GPIOA->OSPEEDR.bits.pin1 = GPIO_OSpeed_High;
    // ����ѡ��
    GPIOA->AFR.bits.pin0 = 0x08;
    GPIOA->AFR.bits.pin1 = 0x08;
}
/*
 * uart4_init - ��ʼ������
 */
void uart4_init(uint32 baudrate) {
    uint32 tmp, mantissa, fraction;

    uart4_init_gpio();

    RCC->APB1ENR.bits.uart4 = 1;// ����ʱ��ʹ��
    
    UART4->CR1.bits.M = 0;      // 8����λ
    UART4->CR1.bits.PCE = 0;    // ����żУ��
    UART4->CR1.bits.RE = 1;     // ��
    UART4->CR1.bits.TE = 1;     // ��
    UART4->CR2.bits.STOP = USART_STOP_1bit; // 1λֹͣλ

    UART4->CR3.bits.CTSE = 0;   // �ر�Ӳ��������
    UART4->CR3.bits.RTSE = 0;

    UART4->CR1.bits.OVER8 = 0;  // ��ʼλ16���ز����������˼��㲨����
    tmp = (25 * 42000000) / (4 * baudrate);
    mantissa = tmp / 100;
    fraction = (16 * (tmp - 100 * mantissa) + 50) / 100;
    UART4->BRR.bits.mantissa = mantissa;
    UART4->BRR.bits.fraction = fraction;

    UART4->CR1.bits.UE = 1;     // ��������
}

uint8 uart4_send_byte(uint8 value) {
    UART4->DR.bits.byte = value;
    while (!UART4->SR.bits.TXE);
    return 0;
}

