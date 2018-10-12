#ifndef __DMA_H
#define __DMA_H

#include <stdint.h>
#include <string.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>

#define DMA_BUFFER_SIZE 256

void dma_update();
void write_dma(char *p, int len);

#endif // __DMA_H
