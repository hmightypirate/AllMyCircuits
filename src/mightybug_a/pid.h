#ifndef __PID_H
#define __PID_H

#include <stdio.h>

#define MAX_INTEGRAL 100
#define MAX_PID_ERROR 2000
#define PID_CONTROL_DIVISOR 100

int32_t pid(int32_t error);
void reset_pid();

void set_kp(int);
void set_ki(int);
void set_kd(int);

int get_kp(void);
int get_ki(void);
int get_kd(void);

#endif /* __PID_H */
