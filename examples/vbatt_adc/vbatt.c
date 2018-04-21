/*
 * vbatt.c
 *
 *  Created on: 21/04/2018
 *      Author: jorge
 */


#include "vbatt.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>


void vbatt_adc_setup() {
	int i;

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0);

	/* Make sure the ADC doesn't run during config. */
	adc_power_off(ADC1);

	/* We configure everything for one single conversion. */
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	/* We want to read the temperature sensor, so we have to enable it. */

	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	for (i = 0; i < 800000; i++) /* Wait a bit. */
		__asm__("nop");

	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
}

static uint16_t read_adc_naiive(uint8_t channel) {
	uint8_t channel_array[1];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_direct(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

static uint16_t read_adc_mean(uint8_t channel, int samples) {
	unsigned int total = 0;
	int my_samples = 1;
	if (samples > 1) my_samples = samples;
	for (int i = 0; i < my_samples; i++){
		total = total + (unsigned int) read_adc_naiive(channel);
	}

	return total / my_samples;
}

// returns battery in milivolts
uint16_t read_vbatt() {

	return read_adc_mean(8, 20) * 100 / 37;
}
