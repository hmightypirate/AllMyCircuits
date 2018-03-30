#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>

#include "cron.h"
#include "buzzer.h"

void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable INTERNAL_LED_PORT clock (For internal LED */
    rcc_periph_clock_enable(RCC_INTERNAL_LED);

    /* Set internal LED */
    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            INTERNAL_LED);
}

void led_blink(uint32_t ticks) {
    if ((ticks % 500) == 0) {
        gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);
    }
}

int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    cron_setup();
    buzzer_setup();
    cron_add(led_blink);

    beep_blocking(NOTE_C5,500);

    while (1) {
        play_melody_blocking();
        silence_blocking(1000);
    }
    return 0;
}
