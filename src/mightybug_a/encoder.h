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

/* systick measurements */
#define SYSTICK_MEAS 1


void update_encoder_ticks(void);
void update_velocities_encoders(void);

uint32_t get_left_encoder_ticks(void);
uint32_t get_right_encoder_ticks(void);
uint32_t get_left_velocity(void);
uint32_t get_right_velocity(void);

#endif /* __ENCODER_H */