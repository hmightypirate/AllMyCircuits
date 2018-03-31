#ifndef __PID_H
#define __PID_H

#include <stdio.h>

#define MAX_INTEGRAL 100
#define MAX_PID_ERROR 2000

int pid(int proportional);
void reset_pid();

#endif /* __PID_H */
