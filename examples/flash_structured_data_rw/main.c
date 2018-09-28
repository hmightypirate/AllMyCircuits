#include "setup.h"
#include "utils.h"
#include "systick.h"
#include <stdint.h>
#include "flash_non_structured_data.h"
#include "flash_structured_data.h"


struct choice_entry_st{
	char choice;
	void (*functionPtr)(void);
};


// Example of writing a string in flash a read it.
void write_raw_data_example(){

	uint8_t str_send[FLASH_STORAGE_SIZE], str_verify[FLASH_STORAGE_SIZE];

	usart_send_string_stop_on_zero((uint8_t*)"Please enter string to write into Flash memory:\n\r"
			, FLASH_STORAGE_SIZE);
	usart_get_string(str_send, FLASH_STORAGE_SIZE);
	uint32_t result = flash_program_data(FLASH_OPERATION_ADDRESS, str_send
			, FLASH_STORAGE_SIZE);

	switch(result){
	case RESULT_OK: /*everything ok*/
		usart_send_string_stop_on_zero((uint8_t*)"Verification of written data: "
				, FLASH_STORAGE_SIZE);
		flash_read_data(FLASH_OPERATION_ADDRESS, FLASH_STORAGE_SIZE, str_verify);
		usart_send_string_stop_on_zero(str_verify, FLASH_STORAGE_SIZE);
		break;
	case FLASH_WRONG_DATA_WRITTEN: /*data read from Flash is different than written data*/
		usart_send_string_stop_on_zero((uint8_t*)"Wrong data written into flash memory"
				, FLASH_STORAGE_SIZE);
		break;
	default: /*wrong flags' values in Flash Status Register (FLASH_SR)*/
		usart_send_string_stop_on_zero((uint8_t*)"Wrong value of FLASH_SR: "
				, FLASH_STORAGE_SIZE);
		local_ltoa_hex(result, str_send);
		usart_send_string_stop_on_zero(str_send, FLASH_STORAGE_SIZE);
		break;
	}
	usart_send_string_stop_on_zero((uint8_t*)"\n", 2);
}

// Read a string from flash and print it
void read_raw_data_example(){

	uint8_t str_verify[FLASH_STORAGE_SIZE];

	usart_send_string_stop_on_zero((uint8_t*)"Written data: "
			, FLASH_STORAGE_SIZE);
	flash_read_data(FLASH_OPERATION_ADDRESS, FLASH_STORAGE_SIZE, str_verify);
	usart_send_string_stop_on_zero(str_verify, FLASH_STORAGE_SIZE);

	usart_send_string_stop_on_zero((uint8_t*)"\n", 2);
}

// Writes data in a structure with fixed data and stores it on flash, then
// checks the data
void write_structured_data_example(){

	uint8_t str_verify[FLASH_STORAGE_SIZE];

	usart_send_string_stop_on_zero((uint8_t*)"Loading some data to the struct:\n\r"
				, FLASH_STORAGE_SIZE);
	fill_persistent_structured_data();

	usart_send_string_stop_on_zero((uint8_t*)"Saving on flash:\n\r"
				, FLASH_STORAGE_SIZE);
	uint32_t result = save_data_to_flash();

	switch(result){
	case RESULT_OK: /*everything ok*/
		usart_send_string_stop_on_zero((uint8_t*)"Verification of written data: "
				, FLASH_STORAGE_SIZE);
		flash_read_data(FLASH_OPERATION_ADDRESS, FLASH_STORAGE_SIZE, str_verify);
		usart_send_string(str_verify, FLASH_STORAGE_SIZE);
		break;
	case FLASH_WRONG_DATA_WRITTEN: /*data read from Flash is different than written data*/
		usart_send_string_stop_on_zero((uint8_t*)"Wrong data written into flash memory"
				, FLASH_STORAGE_SIZE);
		break;
	default: /*wrong flags' values in Flash Status Register (FLASH_SR)*/
		usart_send_string_stop_on_zero((uint8_t*)"Wrong value of FLASH_SR: "
				, FLASH_STORAGE_SIZE);
		//local_ltoa_hex(result, str_send);
		//usart_send_string(str_send, FLASH_STORAGE_SIZE);
		break;
	}
	usart_send_string_stop_on_zero((uint8_t*)"\n", 2);
}

// Read data from flash into a structure and prints the data
void read_structured_data_example(){

	clean_persistent_structured_data(); //Clean RAM before loading from flash
	load_data_from_flash();
	printf("\n\n   myuint32 %u\n"
			"   mystring: %s\n"
			"   mychar: %c\n"
			"   myuint16: %u\n"
			"   kp_bank(0): %u\n"
			"   ki_bank(0): %u\n"
			"   kd_bank(0): %u\n"
			"   speed_bank(0): %u\n"
			"   kp_bank(1): %u\n"
			"   ki_bank(1): %u\n"
			"   kd_bank(1): %u\n"
			"   speed_bank(1): %u\n"
			"   kp_bank(2): %u\n"
			"   ki_bank(2): %u\n"
			"   kd_bank(2): %u\n"
			"   speed_bank(2): %u\n"
			"\n",
			(unsigned int) get_persistant_myuint32(),
			get_persistant_mystring(),
			get_persistant_mychar(),
			(unsigned int) get_persistant_myuint16(),
			(unsigned int) get_persistant_pid_kp_bank(0),
			(unsigned int) get_persistant_pid_kp_bank(0),
			(unsigned int) get_persistant_pid_kp_bank(0),
			(unsigned int) get_persistant_pid_speed_bank(0),
			(unsigned int) get_persistant_pid_kp_bank(1),
			(unsigned int) get_persistant_pid_kp_bank(1),
			(unsigned int) get_persistant_pid_kp_bank(1),
			(unsigned int) get_persistant_pid_speed_bank(1),
			(unsigned int) get_persistant_pid_kp_bank(2),
			(unsigned int) get_persistant_pid_kp_bank(2),
			(unsigned int) get_persistant_pid_kp_bank(2),
			(unsigned int) get_persistant_pid_speed_bank(2)
			);
}

#define CHOICES_LEN 4 // Number of examples
struct choice_entry_st choices[] = {
		{.choice = '1', .functionPtr = write_raw_data_example},
		{.choice = '2', .functionPtr = read_raw_data_example},
		{.choice = '3', .functionPtr = write_structured_data_example},
		{.choice = '4', .functionPtr = read_structured_data_example}
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
