#ifndef __VBATT_H
#define __VBATT_H

#include "setup.h"

/* Battery limit in millivolts (2 cells) */
#define BATTERY_LIMIT_MV 6400
#define RESISTOR_DIVISOR 37

/* Time between battery readings: 1000 ms => 1s */
#define SYS_BETWEEN_READS 1000

void vbatt_setup();
uint16_t read_vbatt();
uint8_t has_batt_drained(void);
uint16_t get_last_batt_meas(void);

#endif /* __VBATT_H */
