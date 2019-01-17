#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "setup.h"
#include "pid.h"
#include "encoder.h"
#include "fsm.h"

// TODO: More intuitive names
#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

// TODO: enum FORWARD BACKWARD
#define FORWARD 0
#define BACKWARD 1

// TODO: DEBUG and PICKLE out of motor module
/* if this flag is set, both motors will receive target velocity till they reach and end of line */
#define DEBUG_INERTIA_TEST 0
#define DEBUG_INERTIA_TIME_MS 1000

void set_target_velocity(int target_vel);
int get_target_velocity(void);
void motor_control(int error);
void stop_motors();


int get_last_left_vel(void);
int get_last_right_vel(void);


#endif /* __MOTORS_H */
