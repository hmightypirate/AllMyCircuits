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
#include <libopencm3/stm32/exti.h>

#define FALLING 0
#define RISING 1

#define USART_CONSOLE USART1
#define INT_DIGITS 11		/* enough for 32 bit integer + end of string (\0) */
#define NOBOUNCE 500

char number[INT_DIGITS];

volatile uint16_t exti_direction = FALLING;
volatile uint32_t last_rising = 0;
volatile uint32_t laptime = 0;
volatile uint32_t millis;

// Convert integer to string
char *itoa(uint32_t x, char* s) {
    s += INT_DIGITS - 1;
    *--s = 0;
    if (!x) *--s = '0';
    for (; x; x/=10) *--s = '0'+x%10;
    return s;
}

// Send message throug USART1
void send_usart(char *message) {
    for (int i = 0; i < (int)strlen(message); i++){
        usart_send_blocking(USART1, message[i]);
    }
}

// Print integer
void print_int(uint32_t x) {
  send_usart(itoa(x, number));
  usart_send_blocking(USART1, '\n');
}

// Enable internal LED
static void gpio_setup(void) {
    /* Enable GPIOB clock. */
    rcc_periph_clock_enable(RCC_INTERNAL_LED);

    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            INTERNAL_LED);

    /* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
    rcc_periph_clock_enable(RCC_USART1);

    /* Setup GPIO pin GPIO_USART1_TX. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

    /* Setup UART parameters. */
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 9);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_parity(USART1, USART_PARITY_EVEN);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART1);

    /* Set B11 as digital input */
    /* Enable AFIO clock. */
	  rcc_periph_clock_enable(RCC_AFIO);

	  /* Enable EXTI0 interrupt. */

}

// Enable IR LED in pin B4
void ir_setup() {
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
    /* Set prescaler to 72 */
    timer_set_prescaler(TIM3, 72);
    /* A timer update event is generated only after the specified number of
     * repeat count cycles have been completed. */
    timer_set_repetition_counter(TIM3, 0);
    /* Enable Auto-Reload Buffering. */
    timer_enable_preload(TIM3);
    /* Enable the Timer to Run Continuously. */
    timer_continuous_mode(TIM3);
    /* Specify the timer period in the auto-reload register. */
    /* 1/28KhZ ~ 26us */
    timer_set_period(TIM3, 26);
    /* Timer Set Output Compare Mode.*/
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);

    timer_set_oc_value(TIM3, TIM_OC1, 13);

    timer_enable_oc_output(TIM3, TIM_OC1);

    timer_enable_counter(TIM3);
}

// This callback is called once every 1ms
void sys_tick_handler(void) {
  millis++;
  //if ((millis % 500) == 0) gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);
}

// Setup external interruptions
void exti_setup(void) {
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable AFIO clock. */
	rcc_periph_clock_enable(RCC_AFIO);

	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI0_IRQ);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI0, GPIOA);
	exti_direction = FALLING;
	exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI0);
}

// External interrupt service routine
void exti0_isr(void) {
	exti_reset_request(EXTI0);

	//if (exti_direction == FALLING) {
	//	gpio_set(INTERNAL_LED_PORT, INTERNAL_LED);
	//	exti_direction = RISING;
	//	exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
	//} else {
		gpio_clear(INTERNAL_LED_PORT, INTERNAL_LED);
		exti_direction = FALLING;
    if(millis - last_rising > NOBOUNCE) {
      laptime = millis - last_rising;
      last_rising = millis;
    }
		//exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
	//}
}


// Setup systick and start calling the handler
void systick_setup(){
    millis = 0;

   /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    /* Enable interrupt vector */
    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}

int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    // Enable GPIO
    gpio_setup();

    // Start 38kHz IR timer
    ir_setup();

    // Start systick counter
    systick_setup();

    // Start external interruptions
    exti_setup();

    while(1) {
        if(laptime != 0) {
          //usart_send_blocking(USART1, '0');
          print_int(laptime);
          laptime = 0;
        }
    }
}
