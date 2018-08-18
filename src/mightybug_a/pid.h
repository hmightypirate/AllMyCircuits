#ifndef __PID_H
#define __PID_H

#include <stdio.h>

#define MAX_INTEGRAL 100
#define MAX_PID_ERROR 2000 //FIXME Value was 2000
#define PID_CONTROL_DIVISOR 100


/* STORE POSITION (in main loop iterations) 
   # store line position every <it> * TIME_BETWEEN_STORE_POS
*/
#define TIME_BETWEEN_STORE_POS 1
#define NUMBER_POS_READINGS 5
#define AVG_READINGS_POS 20 // 25 works in mightybuga (alpha version)


#define DIFF_ERROR_STACK 20
#define FLAG_USE_DIFF_STACK 1

int32_t pid(int32_t error);
void reset_pid();

void set_kp(int);
void set_ki(int);
void set_kd(int);

int get_kp(void);
int get_ki(void);
int get_kd(void);

int get_proportional(void);
int get_integral(void);
int get_derivative(void);

// TURBO MODE
void reset_prop_readings();
void set_new_reading(uint16_t proportional);
uint8_t is_enable_avg_readings();
int16_t  get_avg_abs_readings();

#endif /* __PID_H */
