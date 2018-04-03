#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

static void clock_setup(void);
static void usart_setup(void);
static void gpio_setup(void);

void usart1_isr(void);
void send_message(char *message);
void send_usart(char *message);
void command_stop(void);
void command_unknown(char *command);
void check_command_car(void);
void check_command_led(void);
void check_module(char *module);
void clear_head_tail(void);
void clear_command_line(void);
void set_head_tail(char *origin);


char command_line[80];
int command_line_size = 0;
char head[20];
char tail[80];
char message[80];
bool led_on;
bool led_off;
bool fast;



int main(void) {
    /* Change interrupt vector table location to avoid conflict with */
    /* serial bootloader interrupt vectors */
    SCB_VTOR = (uint32_t)0x08000000;
    
    clock_setup();
    gpio_setup();
    usart_setup();

    send_message("Agonizing Automobile CLI by Mighty Escuderida\n");
    send_message("Version v0.1\n");

    clear_command_line();

    led_on = false;
    led_off = false;
    fast = false;

    while (1) {
        if (led_on)
            gpio_clear(GPIOC, GPIO13);
        else if (led_off)
            gpio_set(GPIOC, GPIO13);
        else
            gpio_toggle(GPIOC, GPIO13); /* LED on/off */
        for (int i = 0; i < 10000000; ++i) __asm__("nop");
    }
};

static void clock_setup(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */
    rcc_periph_clock_enable(RCC_USART1);

};

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

    /* Enable RX interruptions to usart1_isr() function */
    usart_enable_rx_interrupt(USART1);
    /* Finally enable the USART. */
    usart_enable(USART1);
};

static void gpio_setup(void) {
    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            GPIO13);
};


void send_message(char *message) {
    send_usart(message);
};

void send_usart(char *message) {
    for (int i = 0; i < (int)strlen(message); i++){
        usart_send_blocking(USART1, message[i]);
    }
};


void command_stop() {
    send_message("STOPPING\n");
};

void command_unknown(char *command) {
    sprintf(message, "Unknown command: %s\n", command);
    send_message(message);
};

void check_command_car() {
    set_head_tail(tail);
    if (strcmp(head, "PLAY") == 0) {
        send_message("Go!\n");
    } else if (strcmp(head, "STOP") == 0) {
        command_stop();
    } else {
        command_unknown(head);
    }
};

void check_command_led() {
    set_head_tail(tail);
    if (strcmp(head, "ON") == 0) {
        send_message("LED ON\n");
        led_on = true;
        led_off = false;
    } else if (strcmp(head, "OFF") == 0) {
        send_message("LED OFF\n");
        led_on = false;
        led_off = true;
    } else {
        send_message("LED BLINK\n");
        led_on = false;
        led_off = false;
    }
};

void clear_head_tail(void) {
    head[0] = '\0';
    tail[0] = '\0';
};


void check_module(char *module) {
    if (strcmp(module, "STOP") == 0) command_stop();
    else if (strcmp(module, "CAR") == 0) check_command_car();
//     else if (strcmp(module, "MTR") == 0) check_command_motor();
//     else if (strcmp(module, "ENC") == 0) check_command_encoder();
//     else if (strcmp(module, "LIN") == 0) check_command_line();
//     else if (strcmp(module, "BUZ") == 0) check_command_buzzer();
//     else if (strcmp(module, "RST") == 0) command_reset();
    else if (strcmp(module, "LED") == 0) check_command_led();
    else command_unknown(module);
    clear_head_tail();
};


void usart1_isr(void) {
    if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
        uint8_t data = usart_recv(USART1);
        if (data != '\n') {
            command_line[command_line_size++] = data;
        } else {
            command_line[command_line_size] = '\0';
            set_head_tail(command_line);
            check_module(head);
            clear_command_line();
        }
    }
}

void clear_command_line(void) {
    command_line[0] = '\0';
    command_line_size = 0;
}



void set_head_tail(char *origin) {
    char *tail_ptr = NULL;
    int blank_pos = strlen(origin);

    tail_ptr = strchr(origin, ' ');
    if (tail_ptr != NULL)
        blank_pos = (int)(tail_ptr - origin);

    /* Set head */
    memcpy(head, origin, blank_pos);
    head[blank_pos] = '\0';

    /* Set tail */
    if (tail_ptr != NULL) {
        memcpy(tail, tail_ptr + 1, strlen(tail_ptr + 1));
        tail[strlen(tail_ptr + 1)] = '\0';
    } else
        tail[0] = '\0';
};