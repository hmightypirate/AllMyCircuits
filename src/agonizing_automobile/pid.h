#ifndef __PID_H
#define __PID_H

#include <stdio.h>

#define MAX_ERROR 500
#define MAX_INTEGRAL 100

/*
  PID values
  k_p = (0.3 * 100)
  k_i = 0
  k_d = 10 (* 100)

 */
uint32_t k_p = 30;  /* 0.3 * 100 */
uint32_t k_i = 0; /* integral part */
uint32_t k_d = 1000; /* derivative term */ 

int proportional = 0;
long integral = 0;
int derivative = 0;
int former_proportional = 0;

int pid(int proportional);


#endif /* __PID_H */
