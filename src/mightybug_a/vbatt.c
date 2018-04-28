#include "vbatt.h"

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

