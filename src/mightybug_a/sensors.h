#ifndef __SENSORS_H
#define __SENSORS_H

#include "commons.h"
#include "setup.h"
#include "fsm.h"
#include <stdio.h>

/* Number of sensors */
#define NUM_SENSORS 8
#define K_SENSOR 4095.0

#define LEFT_DRIFT 0
#define RIGHT_DRIFT 1

/* 1 for black line
   0 for white line */
#define FOLLOW_BLACK_LINE 1
#define FOLLOW_WHITE_LINE !FOLLOW_BLACK_LINE

#define BLACK_MEASURE_HARDRESET 4096
#define BLACK_MEASURE 2000
#define WHITE_MEASURE 0

#define SOFT_CALIBRATION 1
#define THRESHOLD_CALIBRATION 200  // 500
/* Number of sensors that could not be callibrated */
#define MAX_NUM_NOT_CALLIBRATED_SENSORS 1

/* if 1 the car stops after finding an horizontal line -> all sensors detect line */
#define FINISH_ALL_INLINE 0

/* the line follower position will be calculated between
   (-(NUM_SENSORS-1)/2 * SEP_SENSORS , (NUM_SENSORS-1)/2 * SEP_SENSORS)

   This value only has to be high enough in order to have an accurate position value
   (100 is more than enough)

 */
#define SEP_SENSORS 100

/* Callibration functions */
uint8_t get_calibrated_sensors_count(void);
void reset_calibration_values(void);
void calibrate_sensors(uint16_t* values);
void hard_reset_sensors(void);

void enable_line_sensors(void);
void disable_line_sensors(void);

void read_line_sensors(uint16_t* sensor_value);
int get_line_position(uint16_t* value);
int is_out_of_line();

uint16_t* get_whites(void);
uint16_t* get_blacks(void);
uint16_t* get_thresholds(void);
uint8_t get_all_inline(void);
void reset_all_inline(void);

int get_position(void);

#endif /* __SENSORS_H */
