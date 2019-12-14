#ifndef __CRON_H
#define __CRON_H

#include "encoder.h"
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/dwt.h>

uint32_t get_millisecs_since_start();
void delay_microseconds(uint32_t microseconds);

#endif /* __CRON_H */
