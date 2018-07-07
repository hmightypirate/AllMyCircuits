#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "setup.h"
#include "pid.h"

#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

#define FORWARD 0
#define BACKWARD 1

/* if this flag is set, both motors will received target velocity till they reach and end of line */
#define DEBUG_INERTIA_TEST 0

void reset_target_velocity(int target_vel);
int get_target_velocity(void);
void reset_target_velocity_turbo(int target_vel);
void motor_control(int error);
void stop_motors();
void set_target_as_turbo(void);
void set_target_as_normal(void);

int get_last_left_vel(void);
int get_last_right_vel(void);


#endif /* __MOTORS_H */
