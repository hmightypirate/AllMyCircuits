#include "setup.h"

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

  /* Enable TIMER for encoder (right encoder) */
  rcc_periph_clock_enable(RCC_RIGHT_ENCODER);

  /* Enable TIMER for PWM engine */
  rcc_periph_clock_enable(RCC_PWM_ENGINE);

  /* Activate clock for AFIO, if used */
  if (USE_ALTERNATE_FUNCTIONS)
    {
      rcc_periph_clock_enable(RCC_AFIO);
    }
}

void setup(void)
{
  /* Change interrupt vector table location to avoid 
   *  conflict with serial bootloader interrupt vectors
   */

  SCB_VTOR = (uint32_t)0x08000000;
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
  
  clock_setup();
}

