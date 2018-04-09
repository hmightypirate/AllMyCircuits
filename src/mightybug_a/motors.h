#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "setup.h"
#include "pid.h"

#define RIGHT_INVERTED 1
#define LEFT_INVERTED 0

void reset_target_velocity(int target_vel);
void motor_control(int error);
void stop_motors();

#endif /* __MOTORS_H */
