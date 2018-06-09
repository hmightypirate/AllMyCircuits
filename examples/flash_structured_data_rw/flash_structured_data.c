#include "flash_non_structured_data.h"
#include "flash_structured_data.h"

#define K_BANKS_NUMBER 4

struct packed_pid_entry {
	int32_t kp;
	int32_t ki;
	int32_t kd;
	int32_t speed;
};

struct structured_data_storage_struct {
	uint32_t myuint32;
	char mystring[10];
	char mychar;
	uint16_t myuint16;
	struct packed_pid_entry pid_constants[K_BANKS_NUMBER];
};

static struct structured_data_storage_struct structured_data_storage;
static uint8_t *structured_data_storage_raw;

inline uint8_t * get_raw_data_pointer(){

	return structured_data_storage_raw;
}

inline int32_t get_raw_data_length(){

	return sizeof(struct structured_data_storage_struct);
}

void flash_structured_setup() {
	sizeof(char[FLASH_STORAGE_SIZE
			- sizeof(struct structured_data_storage_struct)]);

	flash_non_structured_setup();
	structured_data_storage_raw = (uint8_t *) &structured_data_storage;
}

static void str_copy(const char * src, char * dst, uint32_t size){

	for (uint32_t i = 0; i < size; i++){
		dst[i] = src[i];
	}
}

void fill_persistent_structured_data() {

	structured_data_storage.myuint32 = 66;
	str_copy("abcdefghi", structured_data_storage.mystring, 10);
	structured_data_storage.mychar = 'K';
	structured_data_storage.myuint16 = 55;
	for (uint32_t i = 0; i < K_BANKS_NUMBER; i++) {
		structured_data_storage.pid_constants[i].kp = 100 + i;
		structured_data_storage.pid_constants[i].ki = 100 + i;
		structured_data_storage.pid_constants[i].kd = 100 + i;
		structured_data_storage.pid_constants[i].speed = 100 + i;
	}
}

void clean_persistent_structured_data() {

	structured_data_storage.myuint32 = 0;
	str_copy(".........", structured_data_storage.mystring, 10);
	structured_data_storage.mychar = 0;
	structured_data_storage.myuint16 = 0;
	for (uint32_t i = 0; i < K_BANKS_NUMBER; i++) {
		structured_data_storage.pid_constants[i].kp = 0;
		structured_data_storage.pid_constants[i].ki = 0;
		structured_data_storage.pid_constants[i].kd = 0;
		structured_data_storage.pid_constants[i].speed = 0;
	}
}

void put_persistant_myuint32(uint32_t x) {

	structured_data_storage.myuint32 = x;
}

void put_persistant_mystring(char * s, uint32_t size) {

	uint32_t mysize = size;
	if (size > sizeof(structured_data_storage.mystring)) {
		mysize = sizeof(structured_data_storage.mystring);
	}

	for (uint32_t i = 0; i < mysize; i++) {
		structured_data_storage.mystring[i] = s[i];
	}
}

void put_persistant_mychar(char x) {

	structured_data_storage.mychar = x;
}

void put_persistant_myuint16(uint16_t x) {

	structured_data_storage.myuint16 = x;
}

void put_persistant_pid_kp_bank(uint32_t x, uint32_t bank) {

	structured_data_storage.pid_constants[bank].kp = x;
}

void put_persistant_pid_ki_bank(uint32_t x, uint32_t bank) {

	structured_data_storage.pid_constants[bank].ki = x;
}

void put_persistant_pid_kd_bank(uint32_t x, uint32_t bank) {

	structured_data_storage.pid_constants[bank].kd = x;
}

void put_persistant_pid_speed_bank(uint32_t x, uint32_t bank) {

	structured_data_storage.pid_constants[bank].speed = x;
}


uint32_t get_persistant_myuint32() {

	return structured_data_storage.myuint32;
}

char * get_persistant_mystring() {

	return structured_data_storage.mystring;
}

char get_persistant_mychar() {

	return structured_data_storage.mychar;
}

uint16_t get_persistant_myuint16() {

	return structured_data_storage.myuint16;
}

uint32_t get_persistant_pid_kp_bank(uint32_t bank) {

	return structured_data_storage.pid_constants[bank].kp;
}

uint32_t get_persistant_pid_ki_bank(uint32_t bank) {

	return structured_data_storage.pid_constants[bank].ki;
}

uint32_t get_persistant_pid_kd_bank(uint32_t bank) {

	return structured_data_storage.pid_constants[bank].kd;
}

uint32_t get_persistant_pid_speed_bank(uint32_t bank) {

	return structured_data_storage.pid_constants[bank].speed;
}

void load_data_from_flash() {

	flash_read_data(FLASH_OPERATION_ADDRESS,
			get_raw_data_length(),
			get_raw_data_pointer());
}

uint32_t save_data_to_flash() {

	return flash_program_data(FLASH_OPERATION_ADDRESS,
			get_raw_data_pointer(),
			get_raw_data_length());
}
