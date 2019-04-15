#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>

/*
 *Active if alternate functions of pins are used
 */
#define USE_ALTERNATE_FUNCTIONS 1
#define NO_AFIO 0

/* Wrapper of RCC timers */
#define RCC_LEFT_ENCODER RCC_TIM2
#define RCC_PWM_MOTORS RCC_TIM1

/*
 * Usart parameters
 */
#define USART_BAUDRATE 115200 // 921600 //115200
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

/* Motor CONTROL */
#define LEFT_MOTOR_IN1_PORT GPIOB
#define LEFT_MOTOR_IN1_PIN GPIO5
#define LEFT_MOTOR_IN2_PORT GPIOA
#define LEFT_MOTOR_IN2_PIN GPIO12

/* Motor PWM */
#define PWM_MOTOR_TIMER TIM1
#define LEFT_MOTOR_OUTPUT_CHANNEL TIM_OC1
#define LEFT_MOTOR_OUTPUT_PWM TIM_OCM_PWM1
#define LEFT_MOTOR_PWM_PORT GPIOA
#define LEFT_MOTOR_PWM_PIN GPIO_TIM1_CH1

/* LED */
#define LED1_PORT GPIOC
#define LED1_PIN GPIO13

void setup_microcontroller(void);

#endif /* __SETUP_H */
