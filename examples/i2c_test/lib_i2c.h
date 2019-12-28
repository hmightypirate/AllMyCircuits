#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include <stdint.h>

void lib_i2c_setup();
uint8_t lib_i2c_read_byte(uint8_t slave_address, uint16_t slave_register);
void lib_i2c_send_address_2_bytes(uint32_t i2c, uint16_t address,
		uint8_t readwrite);
void lib_i2c_send_2_bytes(uint32_t i2c, uint16_t data);
void lib_i2c_write_byte(uint8_t slave_address, uint16_t slave_register,
		uint8_t data);

#endif /* I2C_LIB_H_ */
