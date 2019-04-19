#ifndef SYSTICK_H
#define SYSTICK_H

#include <libopencm3/cm3/systick.h>

uint32_t get_millisecs_since_start();

void systick_setup();

void sleep_ms(uint8_t millisecs);

#endif
