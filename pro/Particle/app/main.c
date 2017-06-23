#include <stm32f407.h>
#include <xtos.h>
#include <led.h>

void config_interruts(void);

int main(void) {
    led_init();
    usart3_init(115200);

    config_interruts();

    LED_0 = LED_ON;

    uart_send_str(USART3, "\r\n====================\r\n");


    while(1) {

    }
}
