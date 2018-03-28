#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define BLACKPILL

#ifdef BLACKPILL
	#define RCC_GPIOX RCC_GPIOB
        #define GPIOX GPIOB
        #define GPION GPIO12
#else
	#define RCC_GPIOX RCC_GPIOC
	#define GPIOX GPIOC
	#define GPION GPIO13
#endif

int main(void) {
    rcc_periph_clock_enable(RCC_GPIOX);
    gpio_set_mode(GPIOX, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPION);

    while(1) {
        gpio_set(GPIOX, GPION);
        for (int i = 0; i < 100000; ++i) __asm__("nop");
        gpio_clear(GPIOX, GPION);
        for (int i = 0; i <  100000; ++i) __asm__("nop");
    }
}
