#ifndef __MOTORS_H
#define __MOTORS_H

#include "setup.h"
#include <stdint.h>

/**
 * Maximum PWM period (max velocity)
 */
#define MAX_PWM_VALUE 999
#define MAX_VEL_MOTOR MAX_PWM_VALUE
#define MIN_VEL_MOTOR 0

// TODO: More intuitive names
#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

// TODO: enum FORWARD BACKWARD
#define FORWARD 0
#define BACKWARD 1

int32_t get_last_left_vel(void);
void set_left_motor_velocity(int32_t velocity);

#endif /* __MOTORS_H */
