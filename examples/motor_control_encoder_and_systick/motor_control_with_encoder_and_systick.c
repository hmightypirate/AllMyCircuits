#include <libopencm3/cm3/scb.h>
#include <string.h>
#include <stdio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>

/* Read encoders every x systicks */
#define ENCODER_MEASURE_TICKS 10000

uint32_t temp32 = 0;
uint16_t former_left_encoder = 0;
uint16_t new_left_encoder = 0;
uint16_t former_right_encoder = 0;
uint16_t new_right_encoder = 0;
uint16_t new_measure = 0;


void sys_tick_handler(void) {
  // Increase systick calls
  temp32++;

  // We call this handler every 1ms so 1000ms = 1s on/off.
  if (temp32 >= ENCODER_MEASURE_TICKS) {
    gpio_toggle(GPIOC, GPIO13);
    temp32 = 0;
    // Read encoders
    former_left_encoder = new_left_encoder;
    //Read timer 2: left encoder information
    new_left_encoder = (uint16_t)timer_get_counter(TIM2);
    new_right_encoder = (uint16_t)timer_get_counter(TIM1);
    new_measure = 1;
  }
}


/*
 * @brief systick setup (with by 8 division)
 *
 */
void systick_setup(void) {

  // Init counter to 0
  temp32 = 0;

  nvic_set_priority(NVIC_SYSTICK_IRQ, 16);

  // 72MHz / 8 => 9000000 counts per second
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

  // 9000000/9000 = 1000 overflows per second - every 1ms one interrupt
  // SysTick interrupt every N clock pulses: set reload to N-1
  systick_set_reload(8999);

  systick_interrupt_enable();

  // Start counting.
  systick_counter_enable();
}



/*
 * @brief set gpio modes, enable gpio clocks
 */
void gpio_setup(void) {
  /* Enable GPIOB clock (for PWM and control pins) */
  rcc_periph_clock_enable(RCC_GPIOB);

  /* Enable GPIO A */
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable GPIOC clock (For internal LED */
  rcc_periph_clock_enable(RCC_GPIOC);
  
  /* Control GPIOs configuration for right motor */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO10 | GPIO11);

  /* Left motor control AIN2: PB5 */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO5);
  
  /* Control GPIOs configuration for left motor */
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO12);
    
  /* Set internal LED */
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO13);

  
}


/*
 * @brief setup usart
 */
static void usart_setup(void) {

  /* Enable USART */
  rcc_periph_clock_enable(RCC_USART1);

  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_PULL_UPDOWN,
                GPIO_USART1_TX);

  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 9);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX);
  usart_set_parity(USART1, USART_PARITY_EVEN);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  /* Finally enable the USART. */
  usart_enable(USART1);

}

/*
 * @brief setup of left encoder using Timer 2
 */
void left_encoder_setup()
{

  /* Enable GPIO for encoder (left encoder) */
  rcc_periph_clock_enable(RCC_TIM2);
  /* Enable Auxiliary functions I/O */
  rcc_periph_clock_enable(RCC_AFIO);

  timer_reset(RCC_TIM2);

  /* TIM2 remap for the quadrature encoder */
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
                       AFIO_MAPR_TIM2_REMAP_FULL_REMAP);

  /* No reset clock: full period */
  timer_set_period(TIM2, 0xFFFF);

  /* encoders in quadrature  */
  timer_slave_set_mode(TIM2, TIM_SMCR_SMS_EM3);

  /* set input channels  */
  timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI2);

  /* enable counter */
  timer_enable_counter(TIM2);  
}

/*
 * @brief setup of right encoder using Timer 1
 */
void right_encoder_setup()
{

  /* Enable GPIO for encoder (left encoder) */
  rcc_periph_clock_enable(RCC_TIM1);
  /* Enable Auxiliary functions I/O */
  rcc_periph_clock_enable(RCC_AFIO);

  timer_reset(RCC_TIM1);

  /* No reset clock: full period */
  timer_set_period(TIM1, 0xFFFF);

  /* encoders in quadrature  */
  timer_slave_set_mode(TIM1, TIM_SMCR_SMS_EM3);

  /* set input channels  */
  timer_ic_set_input(TIM1, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM1, TIM_IC4, TIM_IC_IN_TI4);

  /* enable counter */
  timer_enable_counter(TIM1); 
}


/*
 * @brief set pwm for motor control
 *
 * It uses timer 4 for controlling two engines with the same PWM
 *
 */
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

/*
 * @brief main function
 *
 */
int main(void) {
  /* Change interrupt vector table location to avoid conflict with */
  /* serial bootloader interrupt vectors */
  SCB_VTOR = (uint32_t)0x08000000;


  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  /* Initial setup */
  gpio_setup();
  pwm_setup();
  usart_setup();
  left_encoder_setup();
  right_encoder_setup();
  /* Configure right motor for forward */
  gpio_set(GPIOB, GPIO11);
  gpio_clear(GPIOB, GPIO10);

  /* Configure left motor for forward */
  gpio_set(GPIOA, GPIO12);
  gpio_clear(GPIOB, GPIO5);
  
  /* this value is the time each engine is active : max value is 1024 */
  timer_set_oc_value(TIM4, TIM_OC3, 100); // 10% duty for left motor
  timer_set_oc_value(TIM4, TIM_OC4, 924); // 0% duty for right motor (because it is not wired yet)

  systick_setup();

  uint16_t left_count = 0;
  uint16_t right_count = 0;
  
  while (1) {

    if (new_measure)
      {
        /* Obtain the difference between the former and new measure */
        char diff_encoder_count[20];

        /* Difference in left encoder */
        if (former_left_encoder >= new_left_encoder)
          {
            left_count = former_left_encoder - new_left_encoder;
          }
        else
          {
            left_count = 65535 - new_left_encoder;
            left_count += former_left_encoder;
          }

        /* Difference in right encoder */
        if (former_right_encoder >= new_right_encoder)
          {
            right_count = former_right_encoder - new_right_encoder;
          }
        else
          {
            right_count = 65535 - new_right_encoder;
            right_count += former_right_encoder;
          }

        sprintf(diff_encoder_count, "%u-%u\n", left_count, right_count);
        
        /* Send difference through the USART */
        for (int i = 0; i < strlen(diff_encoder_count); i++)
          {
            usart_send_blocking(USART1, diff_encoder_count[i]);

          }
        
        // Finished transmission
        new_measure = 0;
      }
  }

  return 0;
}
