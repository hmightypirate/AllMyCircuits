#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/scb.h>

void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable GPIOC clock (For internal LED */
    rcc_periph_clock_enable(RCC_INTERNAL_LED);

    /* Set internal LED */
    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            INTERNAL_LED);
}

void servo_setup(void) {
    /* Enable timer 4 */
    rcc_periph_clock_enable(RCC_TIM4);
    /* Set timer 4 mode to no divisor (72MHz), Edge-aligned, up-counting */
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Set divider to 3 */
    timer_set_prescaler(TIM4, 72);
    /* A timer update event is generated only after the specified number of
     * repeat count cycles have been completed. */
    timer_set_repetition_counter(TIM4, 0);
    /* Enable Auto-Reload Buffering. */
    timer_enable_preload(TIM4);
    /* Enable the Timer to Run Continuously. */
    timer_continuous_mode(TIM4);
    /* Specify the timer period in the auto-reload register. */
    timer_set_period(TIM4, 20000);
    /* Set GPIO mode for pin B7 = TIMER 4 CHANNEL 2 */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_TIM4_CH2);
    timer_disable_oc_output(TIM4, TIM_OC2);
    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_value(TIM4, TIM_OC2, 0);
    timer_enable_oc_output(TIM4, TIM_OC2);
    timer_enable_counter(TIM4);
}

void servo_move_to(int position) {
    if(position > 850) position = 850;
    if(position < -850) position = -850;
    timer_set_oc_value(TIM4, TIM_OC2, 1500 + position);
}

int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    gpio_setup();
    servo_setup();


    while (1) {
        for (int i = 0; i < 5000000; ++i)
            __asm__("nop");
        servo_move_to(850);

        for (int i = 0; i < 5000000; ++i)
            __asm__("nop");
        servo_move_to(-850);
    }

    return 0;
}
