#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM and control pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Control GPIOs configuration for left motor */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO12 | GPIO13);

    /* Enable GPIOC clock (For internal LED */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set internal LED */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);
}

void pwm_setup() {
    /* The speed control pin accepts a PWM input with a frequency of up to
     * 100 kHz */

    /* Enable timer 4 */
    rcc_periph_clock_enable(RCC_TIM4);
    /* Set timer 4 mode to no divisor (72MHz), Edge-aligned, up-counting */
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Set divider to 3 */
    timer_set_prescaler(TIM4, 3);
    /* A timer update event is generated only after the specified number of
     * repeat count cycles have been completed. */
    timer_set_repetition_counter(TIM4, 0);
    /* Enable Auto-Reload Buffering. */
    timer_enable_preload(TIM4);
    /* Enable the Timer to Run Continuously. */
    timer_continuous_mode(TIM4);
    /* Specify the timer period in the auto-reload register. */
    timer_set_period(TIM4, 1024);

    /* The freq is 72 MHz / ((1+3)*(1+0)*(1+1024)) = 17560,975609756 Hz ->
     * period of 56.9 uS*/

    /* Enable output GPIOs */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_TIM4_CH3 | GPIO_TIM4_CH4);

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
    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM2); // so it is in contra phase
    /* This is a convenience function to set the OC preload register value for
     * loading to the compare register. */
    timer_set_oc_value(TIM4, TIM_OC3, 0);
    timer_set_oc_value(TIM4, TIM_OC4, 0);

    timer_enable_oc_output(TIM4, TIM_OC3);
    timer_enable_oc_output(TIM4, TIM_OC4);

    timer_enable_counter(TIM4);
}

int main(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    gpio_setup();
    pwm_setup();

    /* Configure motor for forward */
    gpio_set(GPIOB, GPIO12 | GPIO13);

    timer_set_oc_value(TIM4, TIM_OC3, 51); // 5% duty
    timer_set_oc_value(TIM4, TIM_OC4, 51);

    while (1) {
        gpio_set(GPIOC, GPIO13);
        for (int i = 0; i < 1000000; ++i)
            __asm__("nop");
        gpio_clear(GPIOC, GPIO13);
        for (int i = 0; i < 1000000; ++i)
            __asm__("nop");
    }

    return 0;
}
