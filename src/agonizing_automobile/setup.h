#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

/** 
 * Maximum PWM period 
*/
#define MAX_VEL_MOTOR 1024

/* 
 *Active if alternate functions of pins are used 
*/
#define USE_ALTERNATE_FUNCTIONS 1
#define NO_AFIO 0

/* Wrapper of RCC timers */
#define RCC_LEFT_ENCODER RCC_TIM2
#define RCC_RIGHT_ENCODER RCC_TIM1
#define RCC_PWM_ENGINE RCC_TIM4
#define RCC_ADC_SENSORS RCC_ADC1

/*
 * Usart parameters
 */
#define USART_BAUDRATE 115200
#define USART_DATABITS 9
#define USART_STOPBITS USART_STOPBITS_1
#define USART_MODE USART_MODE_TX_RX
#define USART_PARITY USART_PARITY_EVEN
#define USART_FLOWCONTROL USART_FLOWCONTROL_NONE


/* Encoder parameters */
#define LEFT_ENCODER_TIMER TIM2
#define LEFT_ENCODER_AFIO AFIO_MAPR_TIM2_REMAP_FULL_REMAP
#define LEFT_ENCODER_CHANNEL1 TIM_IC1
#define LEFT_ENCODER_CHANNEL2 TIM_IC2
#define LEFT_ENCODER_CHANNEL1_TI TIM_IC_IN_TI1
#define LEFT_ENCODER_CHANNEL2_TI TIM_IC_IN_TI2

#define RIGHT_ENCODER_TIMER TIM1
#define RIGHT_ENCODER_AFIO NO_AFIO
#define RIGHT_ENCODER_CHANNEL1 TIM_IC1
#define RIGHT_ENCODER_CHANNEL2 TIM_IC4
#define RIGHT_ENCODER_CHANNEL1_TI TIM_IC_IN_TI1
#define RIGHT_ENCODER_CHANNEL2_TI TIM_IC_IN_TI4


void setup(void);

#endif /* __SETUP_H */
