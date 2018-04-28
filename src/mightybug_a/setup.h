#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include "commons.h"


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
#define RCC_PWM_BUZZER RCC_TIM3

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

/* Sensors */
#define SENSOR_ADC ADC1  //shared with battery level measurement
#define SENSOR_ON_PORT GPIOB
#define SENSOR_ON_PIN GPIO1
#define SENSOR_0_PORT GPIOA
#define SENSOR_0_PIN GPIO0
#define SENSOR_1_PORT GPIOA
#define SENSOR_1_PIN GPIO1
#define SENSOR_2_PORT GPIOA
#define SENSOR_2_PIN GPIO2
#define SENSOR_3_PORT GPIOA
#define SENSOR_3_PIN GPIO3
#define SENSOR_4_PORT GPIOA
#define SENSOR_4_PIN GPIO4
#define SENSOR_5_PORT GPIOA
#define SENSOR_5_PIN GPIO5
#define SENSOR_6_PORT GPIOA
#define SENSOR_6_PIN GPIO6
#define SENSOR_7_PORT GPIOA
#define SENSOR_7_PIN GPIO7

/* LED */
#define LED_PORT GPIOC
#define LED_PIN GPIO13

/* MOTOR CONTROL */
#define LEFT_MOTOR_IN1_PORT GPIOA
#define LEFT_MOTOR_IN1_PIN GPIO12
#define LEFT_MOTOR_IN2_PORT GPIOB
#define LEFT_MOTOR_IN2_PIN GPIO5

#define RIGHT_MOTOR_IN1_PORT GPIOB
#define RIGHT_MOTOR_IN1_PIN GPIO11
#define RIGHT_MOTOR_IN2_PORT GPIOB
#define RIGHT_MOTOR_IN2_PIN GPIO10

/* Buzzer */
#define BUZZER_TIMER TIM3
#define BUZZER_AFIO AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP
#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO4
#define BUZZER_OUTPUT_CHANNEL TIM_OC1
#define BUZZER_OUTPUT_PWM TIM_OCM_PWM1

/* Battery */
#define BATTERY_PORT GPIOB
#define BATTERY_PIN GPIO1
#define BATTERY_CHANNEL 8
#define AVG_BATTERY_SAMPLES 20

void setup_microcontroller(void);

#endif /* __SETUP_H */
