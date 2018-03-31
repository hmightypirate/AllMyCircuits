#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void);
static void usart_setup(void);
static void gpio_setup(void);

char welcome[] = "Echo test\n";
#define WELCOME_SIZE 10




int main(void) {
    clock_setup();
    gpio_setup();
    usart_setup();

    for (int i = 0; i < WELCOME_SIZE; i++){
        usart_send_blocking(USART1, welcome[i]);
    }
    while (1) {
        gpio_toggle(GPIOC, GPIO13); /* LED on/off */
        usart_send_blocking(USART1, usart_recv_blocking(USART1));
    }
}




static void clock_setup(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
    rcc_periph_clock_enable(RCC_USART1);

}

static void usart_setup(void) {
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_PULL_UPDOWN,
            GPIO_USART1_TX);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
            GPIO_USART1_RX);

    /* Setup UART parameters. */
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 9);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_EVEN);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART1);
}

static void gpio_setup(void) {
    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);
}