#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(void) {
    rcc_periph_clock_enable(RCC_INTERNAL_LED);
    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  INTERNAL_LED);

    while(1) {
        gpio_set(INTERNAL_LED_PORT, INTERNAL_LED);
        for (int i = 0; i < 100000; ++i) __asm__("nop");
        gpio_clear(INTERNAL_LED_PORT, INTERNAL_LED);
        for (int i = 0; i <  100000; ++i) __asm__("nop");
    }
}
