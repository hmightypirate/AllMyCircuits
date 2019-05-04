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

/*
 *Active if alternate functions of pins are used
 */
#define USE_ALTERNATE_FUNCTIONS 1
#define NO_AFIO 0

/* Wrapper of RCC timers */
#define RCC_LEFT_ENCODER RCC_TIM2

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

/* LED */
#define LED1_PORT GPIOC
#define LED1_PIN GPIO13

void setup_microcontroller(void);

#endif /* __SETUP_H */
