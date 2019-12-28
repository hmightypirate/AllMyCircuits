#include "setup.h"
#include "utils.h"

#define MILLISEC_SLICES 9000

volatile uint32_t millis = 0;
#define FALLING 0
#define RISING 1

uint16_t ch1_direction = FALLING;
uint16_t ch2_direction = FALLING;

volatile uint32_t falling_edge_ch1 = 0;
volatile uint32_t falling_edge_ch2 = 0;
volatile uint32_t rising_edge_ch1 = 0;
volatile uint32_t rising_edge_ch2 = 0;
volatile uint32_t counter = 0;

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

	/* Activate clock for AFIO, if used */
	if (USE_ALTERNATE_FUNCTIONS) {
		rcc_periph_clock_enable(RCC_AFIO);
	}
}

static void exti_setup(void)
{

	/* Enable EXTI6 and EXTI7 interrupt. */
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 14);
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);

	/* Set GPIO6 and GPIO7 (in GPIO port B) to 'input open-drain'. */
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6 | GPIO7);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI6 | EXTI7, GPIOB);
	ch1_direction = FALLING;
	ch2_direction = FALLING;
	exti_set_trigger(EXTI6 | EXTI7, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI6 | EXTI7);

}


void ch1_isr(void)
{
	if (ch1_direction == FALLING) {
		if (ch2_direction == RISING) {
			counter++;
		} else {
			counter--;
		}
        falling_edge_ch1++;
		ch1_direction = RISING;
		exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
	} else {
		if (ch2_direction == RISING) {
			counter--;
		} else {
			counter++;
		}
		rising_edge_ch1++;
		ch1_direction = FALLING;
		exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
	}
}

void ch2_isr(void)
{
	exti_reset_request(EXTI3);

	if (ch2_direction == FALLING) {
		if (ch1_direction == RISING) {
			counter--;
		} else {
			counter++;
		}
        falling_edge_ch2++;
		ch2_direction = RISING;
		exti_set_trigger(EXTI7, EXTI_TRIGGER_RISING);
	} else {
		if (ch1_direction == RISING) {
			counter++;
		} else {
			counter--;
		}
		rising_edge_ch2++;
		ch2_direction = FALLING;
		exti_set_trigger(EXTI7, EXTI_TRIGGER_FALLING);
	}
}

void exti9_5_isr(void)
{
	if (exti_get_flag_status(EXTI6)) {
		ch1_isr();
		exti_reset_request(EXTI6);
	} else if (exti_get_flag_status(EXTI7)) {
		ch2_isr();
		exti_reset_request(EXTI7);
	}
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
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_USART1_RX);

	/* Setup USART PARAMETERS */
	usart_set_baudrate(USART1, USART_BAUDRATE);
	usart_set_databits(USART1, USART_DATABITS);
	usart_set_stopbits(USART1, USART_STOPBITS);
	usart_set_mode(USART1, USART_MODE);
	usart_set_parity(USART1, USART_PARITY);
	usart_set_flow_control(USART1, USART_FLOWCONTROL);

	/* Enable RX interruptions to usart1_isr() function */
	usart_enable_rx_interrupt(USART1);
	/* Enable USART */
	usart_enable(USART1);
}

void encoder_setup()
{

}

/*
 * @brief systick setup
 *
 * @note configure the system tick to interrupt each 1 msec
 */
void systick_setup()
{
	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - one interrupt every 1ms*/
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(MILLISEC_SLICES - 1);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

void sys_tick_handler(void)
{
	// Increase systick calls
	millis++;
}

void gpio_setup(void)
{
	/* Set internal LED */
	gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);

}

/*
 * @brief setup of microcontroller functionality
 */
void setup_microcontroller(void)
{
	/* Change interrupt vector table location to avoid
	 *  conflict with serial bootloader interrupt vectors
	 */

	SCB_VTOR = (uint32_t)0x08000000;
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	clock_setup();
	gpio_setup();
	usart_setup();
	/* left encoder */
	encoder_setup();
	/* Line sensor setup */
	systick_setup();
    exti_setup();

}

/*
 * @brief main function
 *
 */
int main(void)
{

	setup_microcontroller();

	uint32_t last_loop_ms = 0;
	uint32_t current_loop_ms = 0;

	while (1) {
		current_loop_ms = millis;

		if ((current_loop_ms - last_loop_ms) >= 1000) {
			gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);
			printf("Time: %lu F1: %lu R1: %lu F2: %lu R2: %lu ABS: %lu\n", millis / 1000, falling_edge_ch1, rising_edge_ch1, falling_edge_ch2, rising_edge_ch2, counter);
			last_loop_ms = current_loop_ms;
		}
	}

	return 0;
}
