#include "dma.h"

#define MY_DMA_NUMBER DMA1
#define MY_DMA_CHANNEL DMA_CHANNEL4

uint8_t dma_finished = 1; //1 when DMA has completely transfer all data and can be used again

uint16_t writePos = 0;
uint16_t readPos = 0;
uint8_t dma_buffer[DMA_BUFFER_SIZE];
uint8_t dma_send_buffer[DMA_BUFFER_SIZE];

void write_dma(char *p, int len)
{
	if (len > DMA_BUFFER_SIZE - writePos) {
		memcpy(dma_buffer + writePos, p, DMA_BUFFER_SIZE - writePos);
		memcpy(dma_buffer, p + DMA_BUFFER_SIZE - writePos, len + writePos - DMA_BUFFER_SIZE);
	} else {
		memcpy(dma_buffer + writePos, p, len);
	}
	writePos = (writePos + len) % DMA_BUFFER_SIZE;
}

uint16_t data_buffered_size() {
	return writePos >= readPos ? writePos - readPos : DMA_BUFFER_SIZE + writePos - readPos;
}

void cpy_to_send_buffer(uint16_t size) {
	if (writePos > readPos) {
		memcpy(dma_send_buffer, dma_buffer + readPos, size);
	} else {
		memcpy(dma_send_buffer, dma_buffer + readPos, DMA_BUFFER_SIZE - readPos);
		memcpy(dma_send_buffer + DMA_BUFFER_SIZE - readPos, dma_buffer, size + readPos - DMA_BUFFER_SIZE);
	}
	readPos = (readPos + size) % DMA_BUFFER_SIZE;
}

void start_dma_transfer(){

	dma_finished = 0;
	// Doc in https://github.com/leaflabs/libmaple/blob/master/notes/dma-stm32f1.txt
	dma_channel_reset(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_set_peripheral_address(MY_DMA_NUMBER, MY_DMA_CHANNEL, (uint32_t)&USART1_DR);

	dma_set_memory_address(MY_DMA_NUMBER, MY_DMA_CHANNEL, (uint32_t)dma_send_buffer);
	uint16_t data_size = data_buffered_size();
	cpy_to_send_buffer(data_size);
    dma_set_number_of_data(MY_DMA_NUMBER, MY_DMA_CHANNEL, data_size);

	dma_set_read_from_memory(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_enable_memory_increment_mode(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_set_peripheral_size(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(MY_DMA_NUMBER, MY_DMA_CHANNEL, DMA_CCR_PL_MEDIUM); // 4  priorities http://libopencm3.org/docs/latest/stm32l1/html/group__dma__ch__pri.html

	dma_enable_transfer_complete_interrupt(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	dma_enable_channel(MY_DMA_NUMBER, MY_DMA_CHANNEL);

	usart_enable_tx_dma(USART1);

}

void dma_update() {
   if (dma_finished && data_buffered_size()) start_dma_transfer();
}

void dma1_channel4_isr(){

	dma_disable_transfer_complete_interrupt(MY_DMA_NUMBER, MY_DMA_CHANNEL);
	usart_disable_tx_dma(USART1);
	dma_disable_channel(MY_DMA_NUMBER, MY_DMA_CHANNEL);

	dma_finished = 1;

	if (data_buffered_size()) start_dma_transfer();

}