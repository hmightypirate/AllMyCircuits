#include "sensors.h"

static uint16_t black_sensors[NUM_SENSORS];
static uint16_t white_sensors[NUM_SENSORS];
static uint16_t calibrated_sensors[NUM_SENSORS];
static uint16_t threshold[NUM_SENSORS];
static uint8_t last_drift = LEFT_DRIFT;
static uint8_t former_detected_all_inline = 0;
static uint8_t detected_all_inline = 0;
static int out_of_line = 0;

/* This var stores the number of sensors correctly calibrated */
static uint8_t sensors_calibrated_count = 0;

static int started_calibration = 0;

int position = 0;

/*
 * @brief reads a line sensor
 *
 * @param[in] channel specifies the channel to read
 *
 */
static uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(SENSOR_ADC, 1, channel_array);
	adc_start_conversion_direct(SENSOR_ADC);
	while (!adc_eoc(SENSOR_ADC))
		;
	uint16_t reg16 = adc_read_regular(SENSOR_ADC);
	return reg16;
}

/*
 * @brief set manually black and white thresholds to sensors
 *
 * @note it is more advisable to calibrate the sensors rathen
 *        that using this function
 */
void hard_reset_sensors()
{
	for (int i = 0; i < NUM_SENSORS; i++) {
		black_sensors[i] = BLACK_MEASURE;
		white_sensors[i] = WHITE_MEASURE;
		threshold[i] = (black_sensors[i] + white_sensors[i]) / 2;
		calibrated_sensors[i] = 0;
	}

	sensors_calibrated_count = 0;
}

/*
 * @resets calibration values
 *
 * @note called automatically only once during calibration
 */

void reset_calibration_values()
{
	for (int i = 0; i < NUM_SENSORS; i++) {
		black_sensors[i] = WHITE_MEASURE;
		white_sensors[i] = BLACK_MEASURE_HARDRESET;
		threshold[i] = 0;
		calibrated_sensors[i] = 0;
	}
	started_calibration = 1;
	sensors_calibrated_count = 0;
}

/*
 * @brief obtain one measure of all the sensors
 *
 * @param[in] sensor_value array where to store the sensor measures
 */
void read_line_sensors(uint16_t *sensor_value)
{
	for (int i = 0; i < NUM_SENSORS; i++) {
		sensor_value[i] = read_adc_naiive(i);
	}
}

static uint16_t trunc_to_range(uint16_t value, uint16_t min, uint16_t max)
{
	uint16_t trunc_value = value;

	if (value < min) {
		trunc_value = min;
	} else if (value > max) {
		trunc_value = max;
	}

	return trunc_value;
}

/*
 * @brief rescale vale between min and max using scale steps
 */
uint16_t rescale_in_range(uint16_t value, uint16_t min, uint16_t max,
			  uint16_t scale)
{
	return ((value - min) * (scale / (max - min)));
}

int get_position_before_drift()
{
	int pos;

	if (last_drift == LEFT_DRIFT) {
		pos = 1 * SEP_SENSORS;
	} else {
		pos = NUM_SENSORS * SEP_SENSORS;
	}

	return pos;
}

void set_drift_side(int pos)
{
	if (pos < ((NUM_SENSORS + 1) * SEP_SENSORS / 2))
		last_drift = LEFT_DRIFT;
	else
		last_drift = RIGHT_DRIFT;
}

/*
 * @brief transform sensor measures in line position
 *
 * @param[in] value sensor measures
 *
 * @note set the flag FOLLOW_BLACK_LINE to 0 to follow a white line
 *
 */
int get_line_position(uint16_t *value)
{
	uint8_t whites_detected = 0;
	uint8_t blacks_detected = 0;

	uint32_t avg_sensors = 0;
	uint16_t sum_sensors = 0;

	out_of_line = 0;

	// store past value here
	former_detected_all_inline = detected_all_inline;
	detected_all_inline = 0;

	for (int i = 0; i < NUM_SENSORS; i++) {
		// Check whites/blacks detected
		if (value[i] > threshold[i])
			blacks_detected++;
		if (value[i] < threshold[i])
			whites_detected++;

		value[i] = trunc_to_range(value[i], white_sensors[i],
					  black_sensors[i]);
		value[i] = rescale_in_range(value[i], white_sensors[i],
					    black_sensors[i], K_SENSOR);

		avg_sensors += ((uint32_t)value[i]) * (i + 1) * SEP_SENSORS;
		sum_sensors += value[i];
	}

	// all sensors are black
	if ((whites_detected == 0 && (FINISH_ALL_INLINE || FORCE_STATECHANGE_ALL_INLINE) && FOLLOW_BLACK_LINE) ||
	     (blacks_detected == 0 && (FINISH_ALL_INLINE || FORCE_STATECHANGE_ALL_INLINE) && FOLLOW_WHITE_LINE)) {
	  detected_all_inline = 1;

	  // specific to stopping the car
	  if (FINISH_ALL_INLINE)
	    {
		out_of_line = 1;
		position = 0;
	    }
	}

	if ((blacks_detected == 0 && FOLLOW_BLACK_LINE) ||
	    (whites_detected == 0 && FOLLOW_WHITE_LINE)) {

		/* Out of line */
		out_of_line = 1;
		position = get_position_before_drift();

	} else {

		position = (int32_t)avg_sensors / sum_sensors;
	}

	if (FOLLOW_WHITE_LINE) {
		position = (NUM_SENSORS + 1) * SEP_SENSORS - position;
	}

	if (!out_of_line) {
		set_drift_side(position);
	}

	/* Zero-center position */
	position = position - (NUM_SENSORS + 1) * SEP_SENSORS / 2;

	// modify the position
	if (out_of_line) {
		position = (20 * position) / 100;
	}

	position = -1 * position;

	return position;
}

/*
 * @brief return the out of line flag
 *
 */
int is_out_of_line()
{
	return out_of_line;
}

void check_calibrated_sensors(void)
{

	sensors_calibrated_count = 0;

	for (int i = 0; i < NUM_SENSORS; i++) {

		if (black_sensors[i] > white_sensors[i]) {
			if ((black_sensors[i] - white_sensors[i]) >
			    THRESHOLD_CALIBRATION) {
				sensors_calibrated_count++;
				calibrated_sensors[i] = 1;
			} else {
				calibrated_sensors[i] = 0;
			}
		} else {
			calibrated_sensors[i] = 0;
		}
	}
}

/*
 * @brief calibrate sensors (one step)
 *
 * @param[in] values a variable that holds current measurements
 *
 */
void calibrate_sensors(uint16_t *values)
{

	/* reset calibration first time */
	if (!started_calibration) {
		reset_calibration_values();
	}

	for (int i = 0; i < NUM_SENSORS; i++) {
		/* check if current value is higher than previous max value */
		if (values[i] > black_sensors[i]) {
			black_sensors[i] = values[i];
		}
		/* check if current value is lower than previous min value */
		if (values[i] < white_sensors[i]) {
			white_sensors[i] = values[i];
		}

		threshold[i] = (black_sensors[i] + white_sensors[i]) / 2;
	}
	check_calibrated_sensors();
}

/*
 * @brief enable sensors
 */
void enable_line_sensors()
{
	gpio_set(SENSOR_ON_PORT, SENSOR_ON_PIN);
}

/*
 * @brief disable sensors
 */
void disable_line_sensors()
{
	gpio_clear(SENSOR_ON_PORT, SENSOR_ON_PIN);
}

/*
 * @brief get the number of calibrated sensors
 */
uint8_t get_calibrated_sensors_count()
{
	return sensors_calibrated_count;
}

/*
 * @brief get value of white of each sensor
 */
uint16_t *get_whites()
{
	return white_sensors;
}

/*
 * @brief get value of white of each sensor
 */
uint16_t *get_blacks()
{
	return black_sensors;
}

/*
 * @brief get value of white of each sensor
 */
uint16_t *get_thresholds()
{
	return threshold;
}

/*
 * @bried get last position calcualted
 */
int get_position()
{
	return position;
}

/*
 * @brief Get the flag, all sensors in line
 */
uint8_t get_all_inline(void)
{
	return detected_all_inline;
}

/*  @brief rest_all_inline
 *
 */
void reset_all_inline(void)
{
	detected_all_inline = 0;
}


/* 
 * @brief obtain rising-edge changes in inline full detection
 */ 
uint8_t get_inline_change(void)
{
  return (detected_all_inline ^ former_detected_all_inline) && detected_all_inline;
}
