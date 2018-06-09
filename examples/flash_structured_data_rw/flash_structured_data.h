// This lib defines a data structure where you can write data that is stored in
// flash, so it remains between resets (not between reflashes)

#ifndef FLASH_STR_DATA_H
#define FLASH_STR_DATA_H

// Call it once before using this functionality (calls
// flash_non_structured_setup())
void flash_structured_setup();

// Reads from flash and stores it in the struct
void load_data_from_flash();
// Stores the struct in the flash
uint32_t save_data_to_flash();

uint8_t * get_raw_data_pointer();
int32_t get_raw_data_length();

// Writes some data in the data structure (RAM)
void fill_persistent_structured_data();
// Fills with zeros the data structure (RAM)
void clean_persistent_structured_data();

// Puts for the structure data fields
void put_persistant_myuint32(uint32_t x);
void put_persistant_mystring(char * s, uint32_t size);
void put_persistant_mychar(char x);
void put_persistant_myuint16(uint16_t x);
void put_persistant_pid_kp_bank(uint32_t x, uint32_t bank);
void put_persistant_pid_ki_bank(uint32_t x, uint32_t bank);
void put_persistant_pid_kd_bank(uint32_t x, uint32_t bank);
void put_persistant_pid_speed_bank(uint32_t x, uint32_t bank);

// Getters for the structure data fields
uint32_t get_persistant_myuint32();
char * get_persistant_mystring();
char get_persistant_mychar();
uint16_t get_persistant_myuint16();
uint32_t get_persistant_pid_kp_bank(uint32_t bank);
uint32_t get_persistant_pid_ki_bank(uint32_t bank);
uint32_t get_persistant_pid_kd_bank(uint32_t bank);
uint32_t get_persistant_pid_speed_bank(uint32_t bank);

#endif
