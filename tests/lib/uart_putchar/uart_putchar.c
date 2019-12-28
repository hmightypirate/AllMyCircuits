#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "uart_putchar.h"

static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* Enable INTERNAL_LED_PORT clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_INTERNAL_LED);

    /* Enable clock for GPIO_USART1_TX and USART1. */
    rcc_periph_clock_enable(RCC_USART1);
}

static void gpio_setup(void)
{
    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
    gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
              GPIO_CNF_OUTPUT_PUSHPULL, INTERNAL_LED);
}

static void usart_setup(void)
{
    /* Setup GPIO pin GPIO_USART1_TX (GPIO9 or PA9). */
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
}

void uart_putchar_configure(){
    clock_setup();
    gpio_setup();
    usart_setup();
}

int putchar(int a){
	usart_send_blocking(USART1, (char)a);
	return 0;
}
