#include "dma_example.h"
#include "example_commons.h"
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

#define MY_DMA_NUMBER DMA1
#define MY_DMA_CHANNEL DMA_CHANNEL4

int dma_finished = 0; //1 when DMA has completely transfer all data and can be used again

void start_dma_transfer(){

	dma_finished = 0;
	// Doc in https://github.com/leaflabs/libmaple/blob/master/notes/dma-stm32f1.txt
	dma_channel_reset(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_set_peripheral_address(MY_DMA_NUMBER, MY_DMA_CHANNEL
			, (uint32_t)&USART1_DR);
	dma_set_memory_address(MY_DMA_NUMBER, MY_DMA_CHANNEL, (uint32_t)buffer_1);
	dma_set_number_of_data(MY_DMA_NUMBER, MY_DMA_CHANNEL
			, USART_TX_DMA_BUFFER_SIZE);
	dma_set_read_from_memory(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_enable_memory_increment_mode(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_set_peripheral_size(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_PL_MEDIUM); // 4  priorities http://libopencm3.org/docs/latest/stm32l1/html/group__dma__ch__pri.html

	dma_enable_transfer_complete_interrupt(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_enable_channel(MY_DMA_NUMBER, MY_DMA_CHANNEL);

	usart_enable_tx_dma(USART1);
}

void dma1_channel4_isr(){

	dma_disable_transfer_complete_interrupt(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	usart_disable_tx_dma(USART1);
	dma_disable_channel(MY_DMA_NUMBER, MY_DMA_CHANNEL);

	dma_finished = 1;
}

// This is where the power of DMA resides: you can do other stuff while the DMA
// is working (here we just do nothing) and check if dma_finished == 1 when
// you need to transfer more
void wait_until_transfer_is_done(){
	while(dma_finished != 1){
		__asm__("nop");
	}
	dma_finished = 0;
}

void dma_example(int loops){
	char a = INITIAL_ASCII_CODE;

	while (loops > 0) {
		gpio_toggle(MY_LED_PORT, MY_LED); /* LED on/off */
		fill_buffer_1(a);
		start_dma_transfer();
		loops--;
		a++;
		wait_until_transfer_is_done();
	}
}
