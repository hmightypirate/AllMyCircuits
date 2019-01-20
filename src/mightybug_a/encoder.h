#ifndef __ENCODER_H
#define __ENCODER_H

#include <inttypes.h>
#include <stdio.h>
#include "commons.h"
#include "setup.h"

/* wheel perimeter in mm * 0.01 */
#define WHEEL_PERIMETER 1050

/* to obtain measure in mm/s */
#define WHEEL_RATIO 100

/* encoder history measurements lenght */
#define ENCODER_BUFFER_LEN 20

/* encoder acc. measurements */
#define ACC_MEAS 10

/* Weird encoder measures */
#define WEIRD_ENCODER_MEAS UINT16_MAX/2

void update_encoder_ticks(void);
void update_velocities_encoders(void);

uint32_t get_left_encoder_ticks(void);
uint32_t get_right_encoder_ticks(void);
uint32_t get_left_velocity(void);
uint32_t get_right_velocity(void);
void reset_encoder_ticks(void);
int32_t get_left_acc(void);
int32_t get_right_acc(void);
int16_t get_abs_diff_encoders(void);
uint32_t get_last_left_ticks(void);
uint32_t get_last_right_ticks(void);

#endif /* __ENCODER_H */
