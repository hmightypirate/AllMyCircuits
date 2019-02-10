#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "encoder.h"
#include "fsm.h"
#include "mapping.h"
#include "pid.h"
#include "setup.h"
#include <stdint.h>

// TODO: More intuitive names
#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

// TODO: enum FORWARD BACKWARD
#define FORWARD 0
#define BACKWARD 1

// TODO: DEBUG and PICKLE out of motor module
/* if this flag is set, both motors will receive target velocity till they reach
 * and end of line */
#define DEBUG_INERTIA_TEST 0
#define INERTIA_MODE_LIMIT_TIME 1000

/* Pickle configuration */
#define TURBO_PICKLE 1
#define TURBO_PICKLE_IN_CORNERS 0
#define PICKLE_ENC_DISTANCE_DOWN 700 // No pickle going down
#define PICKLE_ENC_DISTANCE_UP 3
#define PICKLE_TURBO_VEL 200

/* anti pickle variables */
#define FLAG_MAX_VEL_DELAY 1
#define MAX_VEL_DELAY 20
#define MAX_VEL_DELAY_STEP_DOWN 300
#define MAX_VEL_DELAY_STEP_UP 30

typedef struct {
	int32_t motor_vel[MAX_VEL_DELAY];
	uint16_t current_pointer;
	uint16_t total_samples;
} veldelay_e;

void set_target_velocity(int32_t target_vel);
int32_t get_target_velocity(void);
void motor_control(int32_t error);
void stop_motors(void);

int32_t get_last_left_vel(void);
int32_t get_last_right_vel(void);

/* Antipickle functions */
void reset_veldelay(void);
int32_t get_next_constrained_target_velocity(int32_t vel);
void increase_pointer_vel_delay(int32_t last_vel);

#endif /* __MOTORS_H */
