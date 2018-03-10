#include <string.h>
#include <stdio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>



void gpio_setup(void) {
    /* Enable GPIOB clock (for PWM and control pins) */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Control GPIOs configuration for left motor */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO12 | GPIO13);

    /* Enable GPIOC clock (For internal LED */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set internal LED */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);

    rcc_periph_clock_enable(RCC_GPIOA);
}


static void usart_setup(void) {

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_PULL_UPDOWN,
                GPIO_USART1_TX);

    /* Setup UART parameters. */
    usart_set_baudrate(USART1, 38400);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART1);


    
}


void encoder_setup()
{

  /* Enable GPIO for encoder */
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);

    timer_reset(RCC_TIM2);
    
    /* TIM2 remap for the quadrature encoder */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
                       AFIO_MAPR_TIM2_REMAP_FULL_REMAP);
    
    /* No reset clock */
    timer_set_period(TIM2, 0xFFFF);

    /* encoders in quadrature  */
    timer_slave_set_mode(TIM2, TIM_SMCR_SMS_EM3);

    /* Disable preload. */

    
    /* set input channels  */
    timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI2);
    
    /* enable counter */
    timer_enable_counter(TIM2);
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
    usart_setup();
    encoder_setup();

    /* Configure motor for forward */
    gpio_set(GPIOB, GPIO12);
    gpio_clear(GPIOB, GPIO13);

    timer_set_oc_value(TIM4, TIM_OC3, 100); // 10% duty for left motor
    timer_set_oc_value(TIM4, TIM_OC4, 0); // 0% duty for right motor (because it is not wired yet)

    uint16_t read_count = 0;
    uint16_t old_read_count = 0;
    
    while (1) {
        gpio_set(GPIOC, GPIO13);
        for (int i = 0; i < 100000; ++i)
            __asm__("nop");
        gpio_clear(GPIOC, GPIO13);
        for (int i = 0; i < 100000; ++i)
            __asm__("nop");

        /* read timer 2 */
        read_count = (uint16_t)timer_get_counter(TIM2);

        char diff_encoder_count[20];
        sprintf(diff_encoder_count, "%u\n", old_read_count - read_count);

        for (int i = 0; i < strlen(diff_encoder_count); i++)
          {
            usart_send_blocking(USART1, diff_encoder_count[i]);

          }

        old_read_count = read_count;
          
    }

    return 0;
}
