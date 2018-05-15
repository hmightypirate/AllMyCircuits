#include "example_commons.h"

void fill_buffer_1(char a){
	for(int i = 0; i < USART_TX_DMA_BUFFER_SIZE; i++){
		buffer_1[i] = a;
	}
}
