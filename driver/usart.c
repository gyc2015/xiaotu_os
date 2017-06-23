#include <stm32f407.h>
#include <uart.h>

/*
 * usart3_init - 初始化串口
 */
void usart3_init(uint32 baudrate) {
    RCC->AHB1ENR.bits.gpiob = 1;
    RCC->APB1ENR.bits.usart3 = 1;// 串口时钟使能

    // 功能选择
    // remark: 对于引脚的功能选择应当放在具体的配置之前
    // 否则初始化后，会通过串口发送一个0xFF的数到上位机
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


void USART3_IRQHandler(void) {
    if (0 != USART3->SR.bits.RXNE) {
        uint8 data = USART3->DR.bits.byte;
        uart_send_byte(USART3, data);
    }
}


