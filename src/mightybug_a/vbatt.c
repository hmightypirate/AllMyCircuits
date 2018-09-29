#include "vbatt.h"

static uint16_t last_measurements[VBATT_LAST_MEASUREMENTS_ARRAY_LEN];
static uint8_t last_measurements_index = 0;

uint8_t add_measurement(uint16_t measurement)
{
  last_measurements[last_measurements_index] = measurement;
  last_measurements_index = (last_measurements_index + 1)
    % VBATT_LAST_MEASUREMENTS_ARRAY_LEN;
  
  return last_measurements_index;
}

uint8_t init_measurements_array(uint16_t value)
{
  last_measurements_index = 0;
  for (uint8_t i = 0; i < VBATT_LAST_MEASUREMENTS_ARRAY_LEN; i++){
    add_measurement(value);
  }
  return last_measurements_index;
}

void vbatt_setup(){
  init_measurements_array(BATTERY_LIMIT_MV + 10);
}

static uint16_t read_adc_naiive(uint8_t channel) {
	adc_set_regular_sequence(BATTERY_ADC, 1, &channel);
	adc_start_conversion_direct(BATTERY_ADC);
	while (!adc_eoc(BATTERY_ADC));
	return (uint16_t) adc_read_regular(BATTERY_ADC);
}

static uint16_t read_adc_mean(uint8_t channel, uint8_t samples) {
	uint32_t sum = 0;
	if (samples < 1) samples = 1;
	for (uint8_t i = 0; i < samples; i++){
		sum = sum + (uint32_t) read_adc_naiive(channel);
	}

	return sum / samples;
}

// returns battery in milivolts
uint16_t read_vbatt()
{
  int16_t raw_measurment = read_adc_mean(BATTERY_CHANNEL, AVG_BATTERY_SAMPLES);
  int16_t mv_value = raw_measurment * 100 / RESISTOR_DIVISOR;
  return mv_value;
}

uint16_t mean_measurements()
{
  uint32_t sum = 0;
  for (uint8_t i = 0; i < VBATT_LAST_MEASUREMENTS_ARRAY_LEN; i++){
    sum = sum + last_measurements[i];
  }
  return (uint16_t) (sum / VBATT_LAST_MEASUREMENTS_ARRAY_LEN);  
}

uint16_t read_vbatt_mean()
{
  add_measurement(read_vbatt());
  return mean_measurements();
}

uint8_t has_batt_drained(void)
{
  return (read_vbatt_mean() < BATTERY_LIMIT_MV);
}