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

#include "./libmusic.h"



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

void sys_tick_handler(void) {
temp32++;

    /* We call this handler every 0.1ms so 10000ms = 1s on/off. */
    if (temp32 >= 10000) {
        gpio_toggle(GPIOC, GPIO13);
        temp32 = 0;
    }

    play_music_loop();
}


void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable GPIOC clock (For internal LED */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set internal LED */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);
    /* For USART (and buzzer) */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clock for USART3. */
    rcc_periph_clock_enable(RCC_USART1);
}

// 0.1 msec per tick
void systick_setup(){
   /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/900 = 10000 overflows per second - every 0.1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(900 -1);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}


void usart_setup(void) {

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_PULL_UPDOWN,
            GPIO_USART1_TX);

    /* Parameters for compatibility with the Bluetooth module*/
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 9);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_parity(USART1, USART_PARITY_EVEN);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART1);
}


int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    music_setup();
    usart_setup();
    systick_setup();

#define LENGTH 105
    int notes_pitch[LENGTH] = {C4, D4, E4, F4, G4, A4, B4, C5,  R //9
            , C4,E4,G4, C4,E4,G4, C4,E4,G4, C4,E4,G4              //+12 = 21
            , D4,F4,A4, D4,F4,A4, D4,F4,A4, D4,F4,A4              //+12 = 33
            , E4,G4,B4, E4,G4,B4, E4,G4,B4, E4,G4,B4              //+12 = 45
            , F4,A4,C5, F4,A4,C5, F4,A4,C5, F4,A4,C5              //+12 = 57
            , G4,B4,D5, G4,B4,D5, G4,B4,D5, G4,B4,D5              //+12 = 69
            , A4,C5,E5, A4,C5,E5, A4,C5,E5, A4,C5,E5              //+12 = 81
            , B4,D5,F5, B4,D5,F5, B4,D5,F5, B4,D5,F5              //+12 = 93
            , C5,E5,G5, C5,E5,G5, C5,E5,G5, C5,E5,G5};            //+12 = 105
    int notes_duration[LENGTH] = {32,32,32,32, 32, 32, 32, 32, 32
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8
            ,  8, 8, 8,  8, 8, 8,  8, 8, 8,  8, 8, 8};
    play_music(60, LENGTH, notes_pitch, notes_duration);

    char welcome[20];
    sprintf(welcome, "%d\n", 42);

    for (unsigned int i = 0; i < strlen(welcome); i++) {
        usart_send_blocking(USART1, welcome[i]);
    }

    while (1) {
        for (int i = 0; i < 1000000; i++) __asm__("nop");
        if (is_music_playing() == 0) play_music(60, LENGTH, notes_pitch, notes_duration);
    }
    return 0;
}
