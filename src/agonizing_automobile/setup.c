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

void sensor_setup(void)
{
    int i;

    /* Make sure the ADC doesn't run during config. */
    adc_power_off(SENSOR_ADC);

    /* We configure everything for one single conversion. */
    adc_disable_scan_mode(SENSOR_ADC);
    adc_set_single_conversion_mode(SENSOR_ADC);
    adc_disable_external_trigger_regular(SENSOR_ADC);
    adc_set_right_aligned(ADC1);
    /* We want to read the temperature sensor, so we have to enable it. */

    adc_set_sample_time_on_all_channels(SENSOR_ADC,
                                        ADC_SMPR_SMP_28DOT5CYC);

    adc_power_on(SENSOR_ADC);

    /* Wait for ADC starting up. */
    for (i = 0; i < 800000; i++)    /* Wait a bit. */
        __asm__("nop");

    adc_reset_calibration(SENSOR_ADC);
    adc_calibrate(SENSOR_ADC);
}

void gpio_setup(void)
{
  /* Sensors */
  gpio_set_mode(SENSOR_0_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_0_PORT);
  gpio_set_mode(SENSOR_1_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_1_PORT);
  gpio_set_mode(SENSOR_2_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_2_PORT);
  gpio_set_mode(SENSOR_3_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_3_PORT);
  gpio_set_mode(SENSOR_4_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_4_PORT);
  gpio_set_mode(SENSOR_5_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_5_PORT);
  gpio_set_mode(SENSOR_6_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_6_PORT);
  gpio_set_mode(SENSOR_7_GPIO, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, SENSOR_7_PORT);

  /* Set internal LED */
  gpio_set_mode(LED_GPIO, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                LED_PORT);
  
  /* Set motor control ports */
  /* Control GPIOs configuration for right motor */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO10 | GPIO11);

  /* Left motor control AIN2: PB5 */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO5);
  
  /* Control GPIOs configuration for left motor */
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO12);
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
  gpio_setup();
  usart_setup();
  motor_pwm_setup();
  /* left encoder */
  encoder_setup(LEFT_ENCODER_TIMER,
                LEFT_ENCODER_AFIO,
                LEFT_ENCODER_CHANNEL1,
                LEFT_ENCODER_CHANNEL2,
                LEFT_ENCODER_CHANNEL1_TI,
                LEFT_ENCODER_CHANNEL2_TI);

  /* Right encoder */
  encoder_setup(RIGHT_ENCODER_TIMER,
                RIGHT_ENCODER_AFIO,
                RIGHT_ENCODER_CHANNEL1,
                RIGHT_ENCODER_CHANNEL2,
                RIGHT_ENCODER_CHANNEL1_TI,
                RIGHT_ENCODER_CHANNEL2_TI);

  /* Line sensor setup */
  sensor_setup();

}

