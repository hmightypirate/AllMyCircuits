#include "setup.h"


/*
 * Setup clocks of internal connections
 */

void clock_setup(void)
{
  /* Enable GPIOB clocks (for PWM and motor control pins) */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIOA */
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable GPIO clock C (internal LED */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable USART */
  rcc_periph_clock_enable(RCC_USART1);

  /* Enable TIMER for encoder (left encoder) */
  rcc_periph_clock_enable(RCC_LEFT_ENCODER);
  timer_reset(RCC_LEFT_ENCODER);

  /* Enable TIMER for encoder (right encoder) */
  rcc_periph_clock_enable(RCC_RIGHT_ENCODER);
  timer_reset(RCC_RIGHT_ENCODER);

  /* Enable TIMER for PWM engine */
  rcc_periph_clock_enable(RCC_PWM_ENGINE);

  /* Enable ADC clock (sensors) */
  rcc_periph_clock_enable(RCC_ADC_SENSORS);
  
  /* Activate clock for AFIO, if used */
  if (USE_ALTERNATE_FUNCTIONS)
    {
      rcc_periph_clock_enable(RCC_AFIO);
    }
}

/* 
 * @brief Setup usart 
 */
void usart_setup(void)
{
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_PULL_UPDOWN,
                GPIO_USART1_TX);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                GPIO_USART1_RX);

  /* Setup USART PARAMETERS */
  usart_set_baudrate(USART1, USART_BAUDRATE);
  usart_set_databits(USART1, USART_DATABITS);
  usart_set_stopbits(USART1, USART_STOPBITS);
  usart_set_mode(USART1, USART_MODE);
  usart_set_parity(USART1, USART_PARITY);
  usart_set_flow_control(USART1, USART_FLOWCONTROL);
  
  /* Enable USART */
  usart_enable(USART1);
}

void encoder_setup(uint32_t timer,
                   int afio_function,
                   int channel1, int channel2,
                   int channel1_ti, int channel2_ti)
{
  if (afio_function)
    {
      gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
                         afio_function);
    }

  /* No reset clock: full period */
  timer_set_period(timer, 0xFFFF);

  /* Encoders in quadrature */
  timer_slave_set_mode(timer, TIM_SMCR_SMS_EM3);

  /* Set input channels */
  timer_ic_set_input(timer, channel1, channel1_ti);
  timer_ic_set_input(timer, channel2, channel2_ti);
  
  timer_enable_counter(timer);
}


/* 
 * @brief pwm engine setup
 */ 
void motor_pwm_setup(void)
{
  /* The speed control pin accepts a PWM input with a frequency of up to
   * 100 kHz */

  
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
  timer_set_period(TIM4, MAX_VEL_MOTOR);
  
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


/* 
 * @brief setup of microcontroller functionality
 */
void setup(void)
{
  /* Change interrupt vector table location to avoid 
   *  conflict with serial bootloader interrupt vectors
   */

  SCB_VTOR = (uint32_t)0x08000000;
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
  
  clock_setup();
  usart_setup();
  motor_pwm_setup();
  /* left encoder */
  encoder_setup(LEFT_ENCODER_TIMER,
                LEFT_ENCODER_AFIO,
                LEFT_ENCODER_CHANNEL1,
                LEFT_ENCODER_CHANNEL2,
                LEFT_ENCODER_CHANNEL1_TI,
                LEFT_ENCODER_CHANNEL2_TI);

  encoder_setup(RIGHT_ENCODER_TIMER,
                RIGHT_ENCODER_AFIO,
                RIGHT_ENCODER_CHANNEL1,
                RIGHT_ENCODER_CHANNEL2,
                RIGHT_ENCODER_CHANNEL1_TI,
                RIGHT_ENCODER_CHANNEL2_TI);

  // TODO: GPIO (sensor, motor control and LED)
}

