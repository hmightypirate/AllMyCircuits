#ifndef LIB_SPI_SLAVE_H_
#define LIB_SPI_SLAVE_H_

#include <stdint.h>

// setups the SPI1 device as slave
void lib_spi_slave_setup();

// Writes anything on the bus, so it can receive data
uint8_t lib_spi_slave_read_byte();

// Writes a byte on the bus
void lib_spi_slave_write_byte(uint8_t out);

#endif /* LIB_SPI_SLAVE_H_ */
