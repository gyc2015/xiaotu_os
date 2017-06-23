#include <stm32f407.h>
#include <uart.h>

/*
 * usart3_init - ��ʼ������
 */
void usart3_init(uint32 baudrate) {
    RCC->AHB1ENR.bits.gpiob = 1;
    RCC->APB1ENR.bits.usart3 = 1;// ����ʱ��ʹ��

    // ����ѡ��
    // remark: �������ŵĹ���ѡ��Ӧ�����ھ��������֮ǰ
    // �����ʼ���󣬻�ͨ�����ڷ���һ��0xFF��������λ��
    GPIOB->AFR.bits.pin10 = GPIO_AF_USART3;
    GPIOB->AFR.bits.pin11 = GPIO_AF_USART3;
    // PB10 : tx
    GPIOB->MODER.bits.pin10 = GPIO_Mode_Af;
    GPIOB->OTYPER.bits.pin10 = GPIO_OType_PP;
    GPIOB->PUPDR.bits.pin10 = GPIO_Pull_Up;
    GPIOB->OSPEEDR.bits.pin10 = GPIO_OSpeed_High;
    // PB11 : rx
    GPIOB->MODER.bits.pin11 = GPIO_Mode_Af;
    GPIOB->OTYPER.bits.pin11 = GPIO_OType_OD;
    GPIOB->PUPDR.bits.pin11 = GPIO_Pull_No;

    uart_init(USART3, baudrate);
}

/*
void UART4_IRQHandler(void) {
    if (0 != UART4->SR.bits.RXNE) {
        uint8 data = UART4->DR.bits.byte;
        uart4_send_byte(data);
    }
}
*/

