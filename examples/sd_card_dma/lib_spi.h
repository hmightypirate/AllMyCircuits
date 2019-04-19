#ifndef LIB_SPI_H_
#define LIB_SPI_H_

#include <stdint.h>

void lib_spi_master_setup();
uint8_t lib_spi_master_read_byte();
void lib_spi_master_write_byte(uint8_t out);
uint8_t lib_spi_master_write_read_byte(uint8_t out);
uint8_t lib_spi_master_drain_spi();

#endif /* LIB_SPI_H_ */
