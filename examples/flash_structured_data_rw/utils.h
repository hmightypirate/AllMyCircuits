
#ifndef UTILS_H
#define UTILS_H

#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define MY_USART USART1
#define MY_LED_PORT INTERNAL_LED_PORT
#define MY_LED INTERNAL_LED

char prompt();

int _write(int file, char *ptr, int len);

void usart_send_string(uint8_t *string, uint16_t str_size);

void usart_get_string(uint8_t *out_string, uint16_t str_max_size);

void local_ltoa_hex(uint32_t value, uint8_t *out_string);

#endif
