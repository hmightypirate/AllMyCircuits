#include "setup.h"
#include "utils.h"
#include "systick.h"
#include <stdint.h>
#include "flash_non_structured_data.h"


struct choice_entry_st{
	char choice;
	void (*functionPtr)(void);
};


void write_raw_data_example(){

	uint8_t str_send[SEND_BUFFER_SIZE], str_verify[SEND_BUFFER_SIZE];

	usart_send_string((uint8_t*)"Please enter string to write into Flash memory:\n\r"
			, SEND_BUFFER_SIZE);
	usart_get_string(str_send, SEND_BUFFER_SIZE);
	uint32_t result = flash_program_data(FLASH_OPERATION_ADDRESS, str_send
			, SEND_BUFFER_SIZE);

	switch(result){
	case RESULT_OK: /*everything ok*/
		usart_send_string((uint8_t*)"Verification of written data: "
				, SEND_BUFFER_SIZE);
		flash_read_data(FLASH_OPERATION_ADDRESS, SEND_BUFFER_SIZE, str_verify);
		usart_send_string(str_verify, SEND_BUFFER_SIZE);
		break;
	case FLASH_WRONG_DATA_WRITTEN: /*data read from Flash is different than written data*/
		usart_send_string((uint8_t*)"Wrong data written into flash memory"
				, SEND_BUFFER_SIZE);
		break;
	default: /*wrong flags' values in Flash Status Register (FLASH_SR)*/
		usart_send_string((uint8_t*)"Wrong value of FLASH_SR: "
				, SEND_BUFFER_SIZE);
		local_ltoa_hex(result, str_send);
		usart_send_string(str_send, SEND_BUFFER_SIZE);
		break;
	}
	usart_send_string((uint8_t*)"\n", 2);
}


void read_raw_data_example(){
	uint8_t str_verify[SEND_BUFFER_SIZE];

	usart_send_string((uint8_t*)"Written data: "
			, SEND_BUFFER_SIZE);
	flash_read_data(FLASH_OPERATION_ADDRESS, SEND_BUFFER_SIZE, str_verify);
	usart_send_string(str_verify, SEND_BUFFER_SIZE);

	usart_send_string((uint8_t*)"\n", 2);
}

#define CHOICES_LEN 2
struct choice_entry_st choices[] = {
		{.choice = '1', .functionPtr = write_raw_data_example},
		{.choice = '2', .functionPtr = read_raw_data_example}
};

int main(void) {
	setup();

	while(1){

		char user_choice = prompt();

		for (int i = 0; i < CHOICES_LEN; i++){
			if (choices[i].choice == user_choice){
				uint32_t c = get_millisecs_since_start();
				choices[i].functionPtr();
				c = get_millisecs_since_start() - c;
				printf("\nTest has lasted %lu ms\n", c);
			}
		}
	}
}
