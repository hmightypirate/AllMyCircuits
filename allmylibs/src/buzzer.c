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

#include "../include/cron.h"
#include "../include/buzzer.h"

uint32_t beep_remaining = 0;
int cron_handler = -1;

// Enable buzzer in pin B4
void buzzer_setup() {
    rcc_periph_clock_enable(RCC_AFIO);
    /* Enable the alternate GPIO for output*/
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST
            , AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);
    /* Enable the GPIO */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4);
    /* Enable timer 3 */
    rcc_periph_clock_enable(RCC_TIM3);
    /* Set timer 3 mode to no divisor (72MHz), Edge-aligned, up-counting */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Set divider to 3 */
    timer_set_prescaler(TIM3, PRESCALER_VALUE);
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
     * period of 1 uS*/

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

void cron_function(uint32_t ticks) {
    (void) ticks;
    beep_remaining--;
    if(beep_remaining == 0) {
        stop();
    }
}

// Continuous beep with tone and duration parameters
// tone - period of the note from notes.h
// duration - duration of the beep in ms
void beep(int tone, int duration) {
    if(tone == NOTE_PAUSE) {
        silence(duration);
        return;
    }

    beep_remaining = duration;
    cron_handler = cron_add(cron_function);

    timer_set_period(TIM3, tone);
    timer_set_oc_value(TIM3, TIM_OC1, tone/2);
}

void stop() {
    if(cron_handler >= 0) cron_delete(cron_handler);
    cron_handler = -1;
    timer_set_oc_value(TIM3, TIM_OC1, 0);
}

void silence(int duration) {
    beep_remaining = duration;
    cron_handler = cron_add(cron_function);
    timer_set_oc_value(TIM3, TIM_OC1, 0);
}

uint32_t get_beep_remaining(){
    return beep_remaining;
}

void silence_blocking(int duration) {
    silence(duration);
    while(get_beep_remaining() > 0){
                  __asm__("nop");
    }
}

void beep_blocking(int tone, int duration) {
    beep(tone,duration);
    while(get_beep_remaining() > 0){
                  __asm__("nop");
    }
}

void play_melody_blocking() {
    
}

void play_melody() {

}
