#ifndef FLASH_STR_DATA_H
#define FLASH_STR_DATA_H

#include <stdint.h>

#define SEND_BUFFER_SIZE 256
#define FLASH_OPERATION_ADDRESS ((uint32_t)0x0800f000)
#define FLASH_PAGE_NUM_MAX 127
#define FLASH_PAGE_SIZE 0x800
#define FLASH_WRONG_DATA_WRITTEN 0x80
#define RESULT_OK 0

void flash_setup();

uint32_t flash_program_data(uint32_t start_address, uint8_t *input_data
		, uint16_t num_elements);

void flash_read_data(uint32_t start_address, uint16_t num_elements
		, uint8_t *output_data);


#endif
