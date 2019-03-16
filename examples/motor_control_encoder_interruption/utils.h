#ifndef UTILS_H
#define UTILS_H

#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define MY_USART USART1
#define MY_LED_PORT INTERNAL_LED_PORT
#define MY_LED INTERNAL_LED

/* Weird encoder measures */
#define WEIRD_ENCODER_MEAS UINT16_MAX / 2


char prompt();

int _write(int file, char *ptr, int len);

#endif // UTILS_H
