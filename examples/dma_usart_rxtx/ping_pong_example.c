#include "ping_pong_example.h"
#include "example_commons.h"

bool send_if_possible(char tx_char){
	if (usart_get_flag(MY_USART, USART_SR_TXE)){ //TXE: Transmit data buffer empty
		usart_send(MY_USART, tx_char);
		return true;
	}
	return false;
}

void send_one_and_fill_the_other(const char * send_buffer, char * fill_buffer
		, const char fill_with_this){
	int sending_index = 0;
	int filling_index = 0;
	while(sending_index < USART_TX_DMA_BUFFER_SIZE){
		if (send_if_possible(send_buffer[sending_index])){
			sending_index++;
		}
		if (filling_index < USART_TX_DMA_BUFFER_SIZE){
			fill_buffer[filling_index] = fill_with_this;
			filling_index++;
		}
	}
}

void ping_pong_example(int loops){
	char a = INITIAL_ASCII_CODE;

	fill_buffer_1(a++);

	while (loops > 0) {
		loops--;
		gpio_toggle(MY_LED_PORT, MY_LED); /* LED on/off */
		send_one_and_fill_the_other(buffer_1, buffer_2, a);
		if (loops <= 0){
			break;
		}
		loops--;
		a++;
		gpio_toggle(MY_LED_PORT, MY_LED); /* LED on/off */
		send_one_and_fill_the_other(buffer_2, buffer_1, a);
		a++;
	}
}
