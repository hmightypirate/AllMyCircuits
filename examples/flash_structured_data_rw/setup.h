
#ifndef SETUP_H
#define SETUP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>


// Calls all the hardware setup commands
void setup();

#endif
