#ifndef LIB_SPI_MASTER_H_
#define LIB_SPI_MASTER_H_

#include <stdint.h>

// setups the SPI2 device as master
void lib_spi_master_setup();

// Writes anything on the bus, so it can receive data
uint8_t lib_spi_master_read_byte();

// Writes a byte on the bus
void lib_spi_master_write_byte(uint8_t out);

// Writes a byte and receives another one
uint8_t lib_spi_master_write_read_byte(uint8_t out);

// Removes remaining data on the bus, preparing the SPI bus for a new
// transmission
uint8_t lib_spi_master_drain_spi();

#endif /* LIB_SPI_MASTER_H_ */
