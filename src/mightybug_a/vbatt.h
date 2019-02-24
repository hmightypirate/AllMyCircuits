#ifndef __VBATT_H
#define __VBATT_H

#include "setup.h"

/* Battery limit in millivolts (2 cells) */
#define BATTERY_LIMIT_MV 6400
#define RESISTOR_DIVISOR 37

/* Time between battery readings: 1000 ms => 1s */
#define VBATT_TIME_BETWEEN_READS 5000

/* Length of array of past measurements */
#define VBATT_LAST_MEASUREMENTS_ARRAY_LEN 100

void vbatt_setup(void);
uint16_t read_vbatt(void);
uint16_t read_vbatt_mean(void);
uint8_t is_vbatt_drained(void);

#endif /* __VBATT_H */
