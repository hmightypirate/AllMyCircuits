#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

static void clock_setup(void);
static void usart_setup(void);
static void gpio_setup(void);

void send_message(char *message);
void send_usart(char *message);

void usart1_isr(void) {
    if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
        send_message("usart1_isr\n");
    }
}

int main(void) {
    clock_setup();
    gpio_setup();
    usart_setup();

    send_message("Agonizing Automobile CLI by Mighty Escuderida\n");
    send_message("Version v0.1\n");

    while (1) {
        gpio_toggle(GPIOC, GPIO13); /* LED on/off */
        for (int i = 0; i < 10000000; ++i) __asm__("nop");
        //while (usart_get_flag(USART1, USART_SR_RXNE)) {
        //    usart_send_blocking(USART1, usart_recv_blocking(USART1));
        //}
        
        //usart_send_blocking(USART1, usart_recv(USART1));
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
    nvic_set_priority(NVIC_USART1_IRQ, 16);
    nvic_enable_irq(NVIC_USART1_IRQ);
    
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

    usart_enable_rx_interrupt(USART1);
    /* Finally enable the USART. */
    usart_enable(USART1);
}

static void gpio_setup(void) {
    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);
}


void send_message(char *message) {
    send_usart(message);
}

void send_usart(char *message) {
    for (int i = 0; i < strlen(message); i++){
        usart_send_blocking(USART1, message[i]);
    }
}