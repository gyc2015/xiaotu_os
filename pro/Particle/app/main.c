#include <stm32f407.h>
#include <led.h>

int main(void) {
    led_init();

    LED_0 = LED_OFF;
    LED_1 = LED_OFF;
    LED_2 = LED_OFF;

    LED_0 = LED_ON;
    LED_1 = LED_ON;
    LED_2 = LED_ON;


    while(1) {}
}
