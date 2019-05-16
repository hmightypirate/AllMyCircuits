#ifndef UTILS_H
#define UTILS_H

#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>

int _write(int file, char *ptr, int len);

#endif // UTILS_H
