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

#include "libjukebox.h"
#include "commons.h"

//// Global timing, system tick related code
volatile uint32_t milisecs_since_start = 0;

void sys_tick_handler(void) {
	milisecs_since_start++;
}


void pwm_setup() {

    int use_alternate_gpio = 1;
    if (use_alternate_gpio == 1){
        rcc_periph_clock_enable(RCC_AFIO);
        /* Enable the alternate GPIO for output*/
        gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST
                , AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);
        /* Enable the GPIO */
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4);
    } else {
        /* Enable output GPIOs */
        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
    }

    /* Enable timer 3 */
    rcc_periph_clock_enable(RCC_TIM3);
    /* Set timer 3 mode to no divisor (72MHz), Edge-aligned, up-counting */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Set divider to 3 */
    timer_set_prescaler(TIM3, 3);
    /* A timer update event is generated only after the specified number of
     * repeat count cycles have been completed. */
    timer_set_repetition_counter(TIM3, 0);
    /* Enable Auto-Reload Buffering. */
    timer_enable_preload(TIM3);
    /* Enable the Timer to Run Continuously. */
    timer_continuous_mode(TIM3);
    /* Specify the timer period in the auto-reload register. */
    timer_set_period(TIM3, 0);

    /* The freq is 72 MHz / ((1+3)*(1+0)*(1+1024)) = 17560,975609756 Hz ->
     * period of 56.9 uS*/

    /* Timer Set Output Compare Mode.

     Specifies how the comparator output will respond to a compare match. The
     mode can be:

     Frozen - the output does not respond to a match.
     Active - the output assumes the active state on the first match.
     Inactive - the output assumes the inactive state on the first match.
     Toggle - The output switches between active and inactive states on each
     match.
     Force inactive. The output is forced low regardless of the compare
     state.
     Force active. The output is forced high regardless of the compare
     state.
     PWM1 - The output is active when the counter is less than the compare
     register contents and inactive otherwise.
     PWM2 - The output is inactive when the counter is less than the compare
     register contents and active otherwise. */
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);

    timer_set_oc_value(TIM3, TIM_OC1, 0);

    timer_enable_oc_output(TIM3, TIM_OC1);

    timer_enable_counter(TIM3);
}

/* 
 * @brief setups pwm for music play
 */
void music_setup(){
    pwm_setup();
}


// configure the system tick to interrupt each 1 msec
void systick_setup(){
   /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - one interrupt every 1ms*/
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(9000 -1);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}
//// End of global timing, system tick related code


int _write(int file, char *ptr, int len) {
    int i;

    if (file == 1) {
        for (i = 0; i < len; i++)
            usart_send_blocking(USART1, ptr[i]);
        return i;
    }

    errno = EIO;
    return -1;
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
    jukebox_setup();
    usart_setup();
    systick_setup();

    jukebox_play_current();

    int state_led = STATE_LED_TOGGLE;
    uint32_t last_milisec = milisecs_since_start;

    while (1) {
        if ((milisecs_since_start % 1000) == 0) {
            if (state_led == STATE_LED_TOGGLE){
                gpio_toggle(GPIOC, GPIO13);
                printf("%d\n", (int)milisecs_since_start);
                state_led = STATE_LED_ALREADY_TOGGLED;
            }
        }
        if ((milisecs_since_start % 1000) == 1){
            state_led = STATE_LED_TOGGLE;
        }
        if (is_jukebox_playing() == 0) {
            jukebox_play_next();
        }
        if (last_milisec < milisecs_since_start) {
            play_music_loop();
            last_milisec = milisecs_since_start;
        }
    }
    return 0;
}
