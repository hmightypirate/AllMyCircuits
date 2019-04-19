#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "lib_spi_master.h"

void lib_spi_master_setup() {
	/* Configure GPIOs: SS=PB12, SCK=PB13, MISO=PB14 and MOSI=PB15 */
	gpio_set_mode(
			GPIOB,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO12 | GPIO13 | GPIO15);

	gpio_set_mode(
			GPIOB,
			GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_FLOAT,
			GPIO14);

	/* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
	spi_reset(SPI2);

	/* Explicitly disable I2S in favour of SPI operation */
	SPI2_I2SCFGR = 0;

	/* Set up SPI in Master mode with:
	 * Clock baud rate: 1/256 of peripheral clock frequency
	 * Clock polarity: Idle Low
	 * Clock phase: Data valid on 1st clock pulse
	 * Data frame format: 8-bit
	 * Frame format: MSB First
	 */
	spi_init_master(
			SPI2,
			SPI_CR1_BAUDRATE_FPCLK_DIV_256,
			SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
			SPI_CR1_CPHA_CLK_TRANSITION_1,
			SPI_CR1_DFF_8BIT,
			SPI_CR1_MSBFIRST);

	spi_set_full_duplex_mode(SPI2);

	/*
	 * Set NSS management to software.
	 *
	 * Note:
	 * Setting nss high is very important, even if we are controlling the GPIO
	 * ourselves this bit needs to be at least set to 1, otherwise the spi
	 * peripheral will not send any data out.
	 */
	spi_enable_software_slave_management(SPI2);
	spi_set_nss_high(SPI2);
	spi_disable_crc(SPI2);

	spi_enable(SPI2);
}

/*---------------------------------------------------------------------------*/
/** @brief Receive a byte from MMC via SPI

 Sends a byte 0xFF and picks up whatever returns.

 @returns BYTE value received.
 */

uint8_t lib_spi_master_read_byte() {
	return lib_spi_master_write_read_byte(0xff);
}

void lib_spi_master_write_byte(uint8_t out) {
	spi_set_nss_high(SPI2);
	spi_write(SPI2, (uint16_t) out);
	spi_set_nss_low(SPI2);
}

uint8_t lib_spi_master_write_read_byte(uint8_t out) {
	return spi_xfer(SPI2, out);
}

uint8_t lib_spi_master_drain_spi(){
	volatile uint8_t dummyread;
	while (!(SPI_SR(SPI2) & SPI_SR_TXE))
		;

	return dummyread = SPI_DR(SPI2);
}
