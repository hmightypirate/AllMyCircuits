
#ifndef SETUP_H
#define SETUP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

/* LEDs */                                                                                       
#define LED1_PORT INTERNAL_LED_PORT
#define LED1_PIN INTERNAL_LED
#define LED2_PORT GPIOC  
#define LED2_PIN GPIO13

void setup();

#endif
