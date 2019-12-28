#ifndef __TARGET_PID_H
#define __TARGET_PID_H

#include <stdint.h>

#define MAX_INTEGRAL_TARGET 100
#define MAX_PID_ERROR_TARGET 2000
#define PID_CONTROL_DIVISOR_TARGET 100

/* STORE POSITION (in main loop iterations)
   # store line position every <it> * TIME_BETWEEN_STORE_POS
*/
#define DIFF_ERROR_STACK_TARGET 5

int32_t pid_target(int32_t error);
void reset_pid_target();

void set_kp_target(int);
void set_ki_target(int);
void set_kd_target(int);

int get_kp_target(void);
int get_ki_target(void);
int get_kd_target(void);

int16_t get_last_error_target(void);
void reset_prop_readings_target();
void set_new_reading_target(uint16_t proportional);

#endif /* __TARGET_PID_H */
