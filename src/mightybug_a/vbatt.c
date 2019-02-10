#include "vbatt.h"

static uint16_t last_measurements[VBATT_LAST_MEASUREMENTS_ARRAY_LEN];
static uint8_t last_measurements_index = 0;

/*
 * @brief Add current measure to measurements array
 */
uint8_t add_measurement(uint16_t measurement)
{
	last_measurements[last_measurements_index] = measurement;
	last_measurements_index =
	    (last_measurements_index + 1) % VBATT_LAST_MEASUREMENTS_ARRAY_LEN;

	return last_measurements_index;
}

/*
 * @brief feeds measurements array with value
 * @param value the value of all cells in measurement array
 */
uint8_t init_measurements_array(uint16_t value)
{
	last_measurements_index = 0;
	for (uint8_t i = 0; i < VBATT_LAST_MEASUREMENTS_ARRAY_LEN; i++) {
		add_measurement(value);
	}
	return last_measurements_index;
}

/*
 * @brief Auxiliar function to access ADC
 */
static uint16_t read_adc_naiive(uint8_t channel)
{
	adc_set_regular_sequence(BATTERY_ADC, 1, &channel);
	adc_start_conversion_direct(BATTERY_ADC);
	while (!adc_eoc(BATTERY_ADC))
		;
	return (uint16_t)adc_read_regular(BATTERY_ADC);
}

/*
 * @brief Auxiliar function to take several samples of the ADC and return its
 * mean
 */
static uint16_t read_adc_mean(uint8_t channel, uint8_t samples)
{
	uint32_t sum = 0;
	if (samples < 1)
		samples = 1;
	for (uint8_t i = 0; i < samples; i++) {
		sum = sum + (uint32_t)read_adc_naiive(channel);
	}

	return sum / samples;
}

/*
 * @brief Auxiliar function to obtain the mean of measurements
 */
uint16_t mean_measurements(void)
{
	uint32_t sum = 0;
	for (uint8_t i = 0; i < VBATT_LAST_MEASUREMENTS_ARRAY_LEN; i++) {
		sum = sum + last_measurements[i];
	}
	return (uint16_t)(sum / VBATT_LAST_MEASUREMENTS_ARRAY_LEN);
}

/*
 * @brief Initialization functions of vbatt module
 */
void vbatt_setup(void)
{
	init_measurements_array(BATTERY_LIMIT_MV + 10);
}

/*
 * @brief returns battery level in millivolts
 */
uint16_t read_vbatt(void)
{
	int16_t raw_measurment =
	    read_adc_mean(BATTERY_CHANNEL, AVG_BATTERY_SAMPLES);
	int16_t mv_value = raw_measurment * 100 / RESISTOR_DIVISOR;
	return mv_value;
}

/*
 * @brief Get a new measurement of vbatt level and return the mean of last
 * measurements
 */
uint16_t read_vbatt_mean()
{
	add_measurement(read_vbatt());
	return mean_measurements();
}

/*
 * @brief Function to check if battery is under level voltage
 */
uint8_t is_vbatt_drained(void)
{
	return (read_vbatt_mean() < BATTERY_LIMIT_MV);
}