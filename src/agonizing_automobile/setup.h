#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/** 
 * Maximum PWM period 
*/
#define MAX_VEL_MOTOR 1024

/* 
 *Active if alternate functions of pins are used 
*/
#define USE_ALTERNATE_FUNCTIONS 1

/* Wrapper of RCC timers */
#define RCC_LEFT_ENCODER RCC_TIM2
#define RCC_RIGHT_ENCODER RCC_TIM1
#define RCC_PWM_ENGINE RCC_TIM4

void setup(void);

#endif /* __SETUP_H */
