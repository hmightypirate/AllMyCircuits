#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "setup.h"
#include "pid.h"

#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

#define FORWARD 0
#define BACKWARD 1

void reset_target_velocity(int target_vel);
int get_target_velocity(void);
void motor_control(int error);
void stop_motors();

#endif /* __MOTORS_H */
