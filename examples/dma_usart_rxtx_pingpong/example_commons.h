
#ifndef E_COMMONS_H
#define E_COMMONS_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "utils.h"

#define USART_TX_DMA_BUFFER_SIZE 1024
char buffer_1[USART_TX_DMA_BUFFER_SIZE];
char buffer_2[USART_TX_DMA_BUFFER_SIZE];

#define INITIAL_ASCII_CODE '0'

void fill_buffer_1(char a);

#endif
