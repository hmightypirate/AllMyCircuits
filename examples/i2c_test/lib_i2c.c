#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "lib_i2c.h"

void lib_i2c_setup(void) {
	/* Enable clocks for I2C2 and AFIO. */
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOB);

	/* Set alternate functions for the SCL and SDA pins of I2C1. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);

	/* Disable the I2C before changing any configuration. */
	i2c_peripheral_disable(I2C1);

	/* APB1 is running at 36MHz. */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

	/* 400KHz - I2C Fast Mode */
	//i2c_set_fast_mode(I2C1);
	i2c_set_standard_mode(I2C1);

	/*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C1, 0x1e);

	/*
	 * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
	 * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
	 * Incremented by 1 -> 11.
	 */
	i2c_set_trise(I2C1, 0x0b);

	/*
	 * This is our slave address - needed only if we want to receive from
	 * other masters.
	 */
	i2c_set_own_7bit_slave_address(I2C1, 0x00);

	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C1);
}

void lib_i2c_send_address_2_bytes(uint32_t i2c, uint16_t address,
		uint8_t readwrite) {
	uint16_t data_to_send = (uint16_t) ((address << 1) | readwrite);
	i2c_send_data(i2c, (uint8_t) ((data_to_send >> 8) & 0xFF)); // MSB of register address
	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)))
		;

	i2c_send_data(i2c, (uint8_t) (data_to_send & 0xFF)); // LSB of register address
	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)))
		;
}

void lib_i2c_send_2_bytes(uint32_t i2c, uint16_t data) {
	uint16_t data_to_send = data;
	i2c_send_data(i2c, (uint8_t) ((data_to_send >> 8) & 0xFF)); // MSB of register address
//	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

	i2c_send_data(i2c, (uint8_t) (data_to_send & 0xFF)); // LSB of register address
	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)))
		;
}

// Does all the things necessary to read one byte from a slave register. These
// things are explained in:
// https://robot-electronics.co.uk/i2c-tutorial
// http://www.micromouseonline.com/2011/09/10/i2c-use-on-the-stm32f103r-medium-density-devices/
uint8_t lib_i2c_read_byte(uint8_t slave_address, uint16_t slave_register) {
	uint32_t reg32 __attribute__((unused));
	uint8_t data = 0;

	/* Send START condition. */
	i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
		;

	/* Say to what address we want to talk to. */
	/* Yes, WRITE is correct - for selecting register in the slave. */
	i2c_send_7bit_address(I2C1, slave_address, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR))
		;

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);

	/* Send from what register we want to read.*/
	//i2c_send_data(I2C1, slave_register);
	lib_i2c_send_2_bytes(I2C1, slave_register);

	/*
	 * Now we transferred that we want to ACCESS the register.
	 * Now we send another START condition (repeated START) and then
	 * transfer the destination but with flag READ.
	 */

	/* Send START condition. */
	i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
		;

	/* Say to what address we want to talk to. */
	i2c_send_7bit_address(I2C1, slave_address, I2C_READ);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR))
		;

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);

	/* Cleaning I2C_SR1_ACK. */
	I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

	/* Now the slave should begin to send us the first byte. Await BTF. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_BTF))
		;
	data = I2C_DR(I2C1);

	/* Send STOP condition*/
	i2c_send_stop(I2C1);

	/* Original state. */
	I2C_CR1(I2C1) &= ~I2C_CR1_POS;

	return data;
}

void lib_i2c_write_byte(uint8_t slave_address, uint16_t slave_register,
		uint8_t data) {
	uint32_t reg32 __attribute__((unused));

	/* Send START condition. */
	i2c_send_start(I2C1);

	/* Waiting for START is send and therefore switched to master mode. */
	while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
		;

	/* Say to what address we want to talk to. */
	i2c_send_7bit_address(I2C1, slave_address, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR))
		;

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);

	/* Sending the register address. */
	lib_i2c_send_2_bytes(I2C1, slave_register);

	i2c_send_data(I2C1, data);
	/* After the last byte we have to wait for TxE too. */
	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)))
		;

	/* Send STOP condition. */
	i2c_send_stop(I2C1);
}
