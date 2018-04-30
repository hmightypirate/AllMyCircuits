#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>

#include "clock.h"
#include "keypad.h"

/* Set STM32 to 72 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);
	
	/* Enable GPIOB clock. */
	rcc_periph_clock_enable(RCC_GPIOB);

	/* Set GPIO13 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

int main(void)
{
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;
    
	clock_setup();
	gpio_setup();
    system_clock_setup();
	keypad_setup();

	while (1) {
		keypad_loop();
		
		if (keypad_read_bt3()) {
			gpio_clear(INTERNAL_LED_PORT, INTERNAL_LED);
		}
		
		if (keypad_read_bt2()) {
			gpio_set(INTERNAL_LED_PORT, INTERNAL_LED);
		}
	}

	return 0;
}