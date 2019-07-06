#ifndef __MOTORS_H
#define __MOTORS_H

#include "commons.h"
#include "encoder.h"
#include "fsm.h"
#include "mapping.h"
#include "pid.h"
#include "setup.h"
#include <stdint.h>

// MOTOR LEFT/RIGHT
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// RPM PID constants
#define RPM_K_P 1
#define RPM_K_I 1 
#define RPM_K_D 0

// TODO: More intuitive names
#define RIGHT_MOTOR_PWM_ANTIPHASE 1
#define LEFT_MOTOR_PWM_ANTIPHASE 0

// TODO: enum FORWARD BACKWARD
#define FORWARD 0
#define BACKWARD 1

/* Pickle configuration */
#define TURBO_PICKLE 0
#define TURBO_PICKLE_IN_CORNERS 0
#define PICKLE_ENC_DISTANCE_DOWN 700 // No pickle going down
#define PICKLE_ENC_DISTANCE_UP 3
#define PICKLE_TURBO_VEL 200

/* anti pickle variables */
#define FLAG_MAX_VEL_DELAY 1
#define MAX_VEL_DELAY 20
#define MAX_VEL_DELAY_STEP_DOWN 300
#define MAX_VEL_DELAY_STEP_UP 50

typedef struct {
	int32_t motor_vel[MAX_VEL_DELAY];
	uint16_t current_pointer;
	uint16_t total_samples;
} veldelay_e;

void set_target_velocity(int32_t target_vel);
int32_t get_target_velocity(void);
void set_target_rpm(int32_t rpm);
int32_t get_target_rpm(void);
void motor_control(int32_t error);
void motor_control_rpm_from_line_pid(int32_t control);
void stop_motors(void);

int32_t get_last_left_vel(void);
int32_t get_last_right_vel(void);

/* Antipickle functions */
void reset_veldelay(void);
int32_t get_next_constrained_target_velocity(int32_t vel);
void increase_pointer_vel_delay(int32_t last_vel);

int32_t rpm_pid(uint8_t side, int32_t error);

#endif /* __MOTORS_H */
