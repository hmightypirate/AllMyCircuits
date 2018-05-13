#include "fill_and_send_example.h"
#include "example_commons.h"

void send_buffer_1(){
	for(int i = 0; i < USART_TX_DMA_BUFFER_SIZE; i++){
		usart_send_blocking(MY_USART, buffer_1[i]);
	}
}

void fill_and_send_example(int loops){
	char a = INITIAL_ASCII_CODE;

	while (loops > 0) {
		loops--;
		gpio_toggle(MY_LED_PORT, MY_LED); /* LED on/off */
		fill_buffer_1(a++);
		send_buffer_1();
	}
}
