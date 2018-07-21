#ifndef UTILS_H
#define UTILS_H

#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define MY_USART USART1
#define MY_LED_PORT INTERNAL_LED_PORT
#define MY_LED INTERNAL_LED


// Shows a prompt and waits for a digit ('0' to '9') that is returned
char prompt();

int _write(int file, char *ptr, int len);

// Writes the string until it reaches a '\0' or the string size
void usart_send_string_stop_on_zero(uint8_t *string, uint16_t str_size);
// Writes the string until it reaches the string size (it also writes '\0's)
void usart_send_string(uint8_t *string, uint16_t str_size);

void usart_get_string(uint8_t *out_string, uint16_t str_max_size);

// Calculates the Hex representation of value and stores it in string
void local_ltoa_hex(uint32_t value, uint8_t *out_string);

#endif
