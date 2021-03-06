#ifndef __SETUP_H
#define __SETUP_H

#include "commons.h"
#include "encoder.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/dwt.h>

#define USE_ENCODER_TIMER 1
#define MILLISEC_SLICES 72000

/*
 *Active if alternate functions of pins are used
 */
#define USE_ALTERNATE_FUNCTIONS 1
#define NO_AFIO 0

/* Wrapper of RCC timers */
#define RCC_LEFT_ENCODER RCC_TIM2
#define RCC_RIGHT_ENCODER RCC_TIM4
#define RCC_PWM_MOTORS RCC_TIM1
#define RCC_ADC_LINE_SENSORS RCC_ADC1
#define RCC_PWM_BUZZER RCC_TIM3

/*
 * Usart parameters
 */
#define USART_BAUDRATE 115200 //460800 //921600 // 921600 //115200
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

#define RIGHT_ENCODER_TIMER TIM4
#define RIGHT_ENCODER_AFIO NO_AFIO
#define RIGHT_ENCODER_CHANNEL1 TIM_IC1
#define RIGHT_ENCODER_CHANNEL2 TIM_IC2
#define RIGHT_ENCODER_CHANNEL1_TI TIM_IC_IN_TI1
#define RIGHT_ENCODER_CHANNEL2_TI TIM_IC_IN_TI2

/* Sensors */
#define SENSOR_ADC ADC1 // shared with battery level measurement
#define SENSOR_ON_PORT GPIOB
#define SENSOR_ON_PIN GPIO1
#define SENSOR_0_PORT GPIOA
#define SENSOR_0_PIN GPIO7
#define SENSOR_1_PORT GPIOA
#define SENSOR_1_PIN GPIO6
#define SENSOR_2_PORT GPIOA
#define SENSOR_2_PIN GPIO5
#define SENSOR_3_PORT GPIOA
#define SENSOR_3_PIN GPIO4
#define SENSOR_4_PORT GPIOA
#define SENSOR_4_PIN GPIO3
#define SENSOR_5_PORT GPIOA
#define SENSOR_5_PIN GPIO2
#define SENSOR_6_PORT GPIOA
#define SENSOR_6_PIN GPIO1
#define SENSOR_7_PORT GPIOA
#define SENSOR_7_PIN GPIO0

/* LEDs */
#define LED1_PORT GPIOC
#define LED1_PIN GPIO13
#define LED2_PORT GPIOB
#define LED2_PIN GPIO12

/* MOTOR CONTROL */
#define LEFT_MOTOR_IN1_PORT GPIOB
#define LEFT_MOTOR_IN1_PIN GPIO5
#define LEFT_MOTOR_IN2_PORT GPIOA
#define LEFT_MOTOR_IN2_PIN GPIO12

#define RIGHT_MOTOR_IN1_PORT GPIOB
#define RIGHT_MOTOR_IN1_PIN GPIO9
#define RIGHT_MOTOR_IN2_PORT GPIOB
#define RIGHT_MOTOR_IN2_PIN GPIO8

/* MOTOR PWM
Both motors are controlled with one timer
*/
#define PWM_MOTOR_TIMER TIM1
#define LEFT_MOTOR_OUTPUT_CHANNEL TIM_OC1
#define LEFT_MOTOR_OUTPUT_PWM TIM_OCM_PWM1
#define RIGHT_MOTOR_OUTPUT_CHANNEL TIM_OC4
#define RIGHT_MOTOR_OUTPUT_PWM TIM_OCM_PWM2
#define LEFT_MOTOR_PWM_PORT GPIOA
#define LEFT_MOTOR_PWM_PIN GPIO_TIM1_CH1
#define RIGHT_MOTOR_PWM_PORT GPIOA
#define RIGHT_MOTOR_PWM_PIN GPIO_TIM1_CH4

/* Buzzer */
#define BUZZER_TIMER TIM3
#define BUZZER_AFIO AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP
#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO4
#define BUZZER_OUTPUT_CHANNEL TIM_OC1
#define BUZZER_OUTPUT_PWM TIM_OCM_PWM1

/* Battery */
#define BATTERY_PORT GPIOB
#define BATTERY_PIN GPIO0
#define BATTERY_CHANNEL 8
#define AVG_BATTERY_SAMPLES 20
#define BATTERY_ADC ADC1

/* Keypad */
#define NUM_BUTTONS 3
#define BUTTON1_PORT GPIOC
#define BUTTON1_PIN GPIO14
#define BUTTON2_PORT GPIOC
#define BUTTON2_PIN GPIO15
#define BUTTON3_PORT GPIOB
#define BUTTON3_PIN GPIO13

void setup_microcontroller(void);

#endif /* __SETUP_H */
