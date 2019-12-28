#include "motors.h"
#include "setup.h"
#include "utils.h"

#define MILLISEC_SLICES 72000

#define CH1 0
#define CH2 1

#define NUM_SAMPLES 2

volatile uint32_t millis = 0;
volatile uint32_t agg = 0;
volatile uint32_t last_interrupt = CH1;
volatile uint32_t edge_ch1_clk_time = 0;
volatile uint32_t edge_ch2_clk_time = 0;
volatile uint32_t edge_ch1_ms_time = 0;
volatile uint32_t edge_ch2_ms_time = 0;
volatile uint32_t diff_time[NUM_SAMPLES];
volatile uint8_t diff_index = 0;

void init_rpm_arrays(void)
{
	for (int i = 0; i < NUM_SAMPLES; i++) {
		diff_time[i] = 0;
	}
}

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

	/* Enable TIMER for PWM engine */
	rcc_periph_clock_enable(RCC_PWM_MOTORS);

	/* Activate clock for AFIO, if used */
	if (USE_ALTERNATE_FUNCTIONS) {
		rcc_periph_clock_enable(RCC_AFIO);
	}
}

/*
 * @brief Setup usart
 */
void usart_setup(void)
{
	nvic_set_priority(NVIC_USART1_IRQ, 16);
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

void encoder_setup(void)
{

	init_rpm_arrays();

	/* Enable EXTI15 interrupt. */
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 14);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);

	/* Set GPIO15 (in GPIO port A) to 'input open-drain'. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO15);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI15, GPIOA);
	exti_set_trigger(EXTI15, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI15);

	/* This let us use PB3 as standard GPIO */
	gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_EXTI3);

	/* Enable EXTI3 interrupt. */
	nvic_set_priority(NVIC_EXTI3_IRQ, 13);
	nvic_enable_irq(NVIC_EXTI3_IRQ);

	/* Set GPIO3 (in GPIO port B) to 'input open-drain'. */
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO3);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI3, GPIOB);
	exti_set_trigger(EXTI3, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI3);
}

void exti15_10_isr(void)
{
	edge_ch1_clk_time = systick_get_value();
	edge_ch1_ms_time = millis;

	if (last_interrupt == CH2) {
		diff_time[diff_index] =
		    (MILLISEC_SLICES * (edge_ch1_ms_time - edge_ch2_ms_time)) +
		    edge_ch2_clk_time - edge_ch1_clk_time;

		diff_index = (diff_index + 1) % NUM_SAMPLES;
	}
	last_interrupt = CH1;

	agg++;

	exti_reset_request(EXTI15);
}

void exti3_isr(void)
{

	edge_ch2_clk_time = systick_get_value();
	edge_ch2_ms_time = millis;

	if (last_interrupt == CH1) {
		diff_time[diff_index] =
		    (MILLISEC_SLICES * (edge_ch2_ms_time - edge_ch1_ms_time)) +
		    edge_ch1_clk_time - edge_ch2_clk_time;

		diff_index = (diff_index + 1) % NUM_SAMPLES;
	}
	last_interrupt = CH2;

	agg++;

	exti_reset_request(EXTI3);
}

/*
 * @brief pwm engine setup
 */
void motor_pwm_setup(void)
{
	/* The speed control pin accepts a PWM input with a frequency of up to
	 * 100 kHz */

	/* Set timer 4 mode to no divisor (72MHz), Edge-aligned, up-counting */
	timer_set_mode(PWM_MOTOR_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
		       TIM_CR1_DIR_UP);
	/* Set divider to 7 */
	timer_set_prescaler(PWM_MOTOR_TIMER, 7);
	/* A timer update event is generated only after the specified number of
	 * repeat count cycles have been completed. - In bluepill is useless */
	timer_set_repetition_counter(PWM_MOTOR_TIMER, 0);
	/* Enable Auto-Reload Buffering. */
	timer_enable_preload(PWM_MOTOR_TIMER);
	/* Enable the Timer to Run Continuously. */
	timer_continuous_mode(PWM_MOTOR_TIMER);
	/* Specify the timer period in the auto-reload register. */
	timer_set_period(PWM_MOTOR_TIMER, MAX_PWM_VALUE);

	/* Enable Main output bit as Timer 1 is and andvanced timer */
	if (PWM_MOTOR_TIMER == TIM1) {
		timer_enable_break_main_output(PWM_MOTOR_TIMER);
	}

	/* The freq is 72 MHz / ((1+7)*(1+0)*(1+999)) = 9000 Hz ->
	 * period of 111.1 uS*/

	/* Enable output GPIOs */
	gpio_set_mode(LEFT_MOTOR_PWM_PORT, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, LEFT_MOTOR_PWM_PIN);

	/* Timer Set Output Compare Mode.

	   Specifies how the comparator output will respond to a compare match.
	   The mode can be:

	   Frozen - the output does not respond to a match.
	   Active - the output assumes the active state on the first match.
	   Inactive - the output assumes the inactive state on the first match.
	   Toggle - The output switches between active and inactive states on
	   each match. Force inactive. The output is forced low regardless of
	   the compare state. Force active. The output is forced high regardless
	   of the compare state. PWM1 - The output is active when the counter is
	   less than the compare register contents and inactive otherwise. PWM2
	   - The output is inactive when the counter is less than the compare
	   register contents and active otherwise. */
	timer_set_oc_mode(PWM_MOTOR_TIMER, LEFT_MOTOR_OUTPUT_CHANNEL,
			  LEFT_MOTOR_OUTPUT_PWM);
	/* This is a convenience function to set the OC preload register value
	 * for loading to the compare register. */
	timer_set_oc_value(PWM_MOTOR_TIMER, LEFT_MOTOR_OUTPUT_CHANNEL, 0);
	timer_enable_oc_output(PWM_MOTOR_TIMER, LEFT_MOTOR_OUTPUT_CHANNEL);
	timer_enable_counter(PWM_MOTOR_TIMER);
}

/*
 * @brief systick setup
 *
 * @note configure the system tick to interrupt each 1 msec
 */
void systick_setup()
{
	/* 72MHz => 72000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

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

uint32_t get_motor_last_measured_rpm(void)
{
	uint32_t diff_count_time = 0;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		diff_count_time += diff_time[i];
	}
	diff_count_time /= NUM_SAMPLES;

	//return 45000000 / diff_count_time;
	return 360000000 / diff_count_time;
}

void setup_irq_timers(void)
{

	/* Without this the timer interrupt routine will never be called. */
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);

	/* Update interrupt enable. */
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

void gpio_setup(void)
{
	/* Set internal LED */
	gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);

	/* Left motor control AIN2: PB5 */
	gpio_set_mode(LEFT_MOTOR_IN2_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, LEFT_MOTOR_IN2_PIN);

	/* Control GPIOs configuration for left motor */
	gpio_set_mode(LEFT_MOTOR_IN1_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, LEFT_MOTOR_IN1_PIN);
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
	motor_pwm_setup();
	/* left encoder */
	encoder_setup();

	/* Line sensor setup */
	systick_setup();

	/* Timer interruptions setup */
	setup_irq_timers();
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
	uint32_t pwm_value = 0;

	while (1) {
		current_loop_ms = millis;

		if ((current_loop_ms - last_loop_ms) >= 500) {
			gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);

			if ((current_loop_ms % 2000) == 0) {
				pwm_value = (pwm_value + 50) % 1000;
				set_left_motor_velocity(pwm_value);
			}

			printf("Time: %lu PWM: %lu RPM: %lu AGG: %lu\n",
			       millis / 500, pwm_value,
			       get_motor_last_measured_rpm(), agg);
			last_loop_ms = current_loop_ms;
			agg = 0;
		}
	}

	return 0;
}
