#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "lib_spi_master.h"

void lib_spi_slave_setup() {
	/* Configure GPIOs: SS=PA4, SCK=PA5, MISO=PA6 and MOSI=PA7 */
	gpio_set_mode(
			GPIOA,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO6);

	gpio_set_mode(
			GPIOA,
			GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_FLOAT,
			GPIO4 | GPIO5 | GPIO7);

	/* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
	spi_reset(SPI1);

	/* Explicitly disable I2S in favour of SPI operation */
	SPI1_I2SCFGR = 0;

	spi_set_slave_mode(SPI1);

	spi_set_full_duplex_mode(SPI1);

	spi_disable_crc(SPI1);

	spi_enable(SPI1);
}


uint8_t lib_spi_slave_read_byte() {
	return spi_read(SPI1);
}

void lib_spi_slave_write_byte(uint8_t out) {
	spi_write(
			SPI1,
			(uint16_t) out);
}

