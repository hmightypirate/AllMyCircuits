#ifndef __SENSORS_H
#define __SENSORS_H

#include "setup.h"
#include <stdio.h>

/* Number of sensors */
#define NUM_SENSORS 8
#define K_SENSOR 1023.0

#define LEFT_DRIFT 0
#define RIGHT_DRIFT 1

/* 1 for black line
   0 for white line */
#define FOLLOW_BLACK_LINE 1

#define BLACK_MEASURE 2000
#define WHITE_MEASURE 0

void calibrate_sensors(void);
void hard_reset_sensors(void);

void read_line_sensors(uint16_t* sensor_value);
int get_line_position(uint16_t* value);

#endif /* __SENSORS_H */
