#ifndef __SENSORS_H
#define __SENSORS_H

#include "commons.h"
#include "setup.h"

#define RIGHT_INVERTED 1
#define LEFT_INVERTED 0

void reset_target_velocity(int target_vel);
void motor_control(int error);

#endif /* __SENSORS_H */
