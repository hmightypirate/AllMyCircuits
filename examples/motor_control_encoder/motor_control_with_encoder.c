#include "utils.h"
#include "setup.h"

/*
 * @brief set gpio modes, enable gpio clocks
 */
void gpio_setup(void) {

  /* Enable GPIOC clock (For internal LED */
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable USART */
  rcc_periph_clock_enable(RCC_USART1);

  /* Enable GPIO for encoder */
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_clock_enable(RCC_AFIO);
  
  /* Set internal LED */
  gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                INTERNAL_LED);

  
}


/*
 * @brief Setup usart
 */
void usart_setup(void)
{

  	nvic_set_priority(NVIC_USART1_IRQ, 15);
	nvic_enable_irq(NVIC_USART1_IRQ);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART1_TX);


	/*
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_USART1_RX);

	*/
	/* Setup USART PARAMETERS */
	usart_set_baudrate(USART1, USART_BAUDRATE);
	usart_set_databits(USART1, USART_DATABITS);
	usart_set_stopbits(USART1, USART_STOPBITS);
	usart_set_mode(USART1, USART_MODE);
	usart_set_parity(USART1, USART_PARITY);
	usart_set_flow_control(USART1, USART_FLOWCONTROL);

	/* Enable RX interruptions to usart1_isr() function */
	//usart_enable_rx_interrupt(USART1);
	/* Enable USART */
	usart_enable(USART1);

}

/*
 * @brief setup encoder using Timer 2
 */
void encoder_setup()
{


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
 * @brief main function
 *
 */
int main(void) {
  SCB_VTOR = (uint32_t) 0x08000000; 
  
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */                                                  

    
    /* Initial setup */
    gpio_setup();
    usart_setup();
    encoder_setup();

    uint16_t read_count = 0;
    uint16_t old_read_count = 0;

    while (1) {
      /* set the LED */
      gpio_set(INTERNAL_LED_PORT,INTERNAL_LED);
      for (int i = 0; i < 100000; ++i)
        __asm__("nop");

      /* clear the LED */
      gpio_clear(INTERNAL_LED_PORT, INTERNAL_LED);
      for (int i = 0; i < 100000; ++i)
        __asm__("nop");

      /* read timer 2: left motor encoder information */
      read_count = (uint16_t)timer_get_counter(TIM2);

      
      /* Obtain the difference between the former and new measure */
      uint16_t next_measure;

      if (old_read_count >= read_count)
	{
	  next_measure = old_read_count - read_count;
	}
      else
	{
	  next_measure = read_count - old_read_count;
	}
      
      printf("Next meas: %u\n", next_measure);
      
      old_read_count = read_count;
      
    }

    return 0;
}
