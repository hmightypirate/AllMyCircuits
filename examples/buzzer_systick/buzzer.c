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

#include "buzzer.h"

int _write(int file, char *ptr, int len)
{
    int i;

    if (file == 1) {
        for (i = 0; i < len; i++)
            usart_send_blocking(USART1, ptr[i]);
        return i;
    }

    errno = EIO;
    return -1;
}

uint32_t temp32 = 0;
uint32_t beep_end = 0;

void sys_tick_handler(void) {
    temp32++;

    /* We call this handler every 1ms so 500ms = 0.5s on/off. */
    if (temp32 >= 500) {
        gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);
        temp32 = 0;
    }

    buzzer_systick();
}

void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable INTERNAL_LED_PORT clock (For internal LED */
    rcc_periph_clock_enable(RCC_INTERNAL_LED);

    /* Set internal LED */
    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            INTERNAL_LED);
}

void systick_setup(){
   /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}

int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    systick_setup();
    setup_buzzer();

    //beep(NOTE_B5,2000);

    play_melody();

    while (1) {}
    return 0;
}
