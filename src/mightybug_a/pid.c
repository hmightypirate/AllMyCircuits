#include "pid.h"

static uint16_t k_p = 100; // Proportional constant
static uint16_t k_i = 0;   // Integral constant
static uint16_t k_d = 0;   // Derivative constant

static int32_t integral = 0;
static int16_t derivative = 0;
static int16_t proportional = 0;
static int16_t last_error = 0;

static int16_t last_error_stack[DIFF_ERROR_STACK];
static int16_t current_stack_idx = 0;

int16_t array_prop_readings[NUMBER_POS_READINGS];
uint8_t total_prop_readings = 0;
uint8_t curr_prop_idx = 0;

void reset_prop_readings()
{
	for (int i = 0; i < NUMBER_POS_READINGS; i++) {
		array_prop_readings[i] = 0;
	}
	total_prop_readings = 0;
	curr_prop_idx = 0;
}

void set_new_reading(uint16_t proportional)
{
	array_prop_readings[curr_prop_idx] = proportional;

	curr_prop_idx += 1;
	total_prop_readings += 1;

	if (curr_prop_idx >= NUMBER_POS_READINGS) {
		curr_prop_idx = 0;
	}

	if (total_prop_readings > NUMBER_POS_READINGS) {
		total_prop_readings = NUMBER_POS_READINGS;
	}
}

uint8_t is_enable_avg_readings()
{
	if (total_prop_readings == NUMBER_POS_READINGS) {
		return 1;
	}

	return 0;
}

/*
 * @brief obtain the mean of absolute position readings
 */
int16_t get_avg_abs_readings()
{
	int16_t avg_prop = 0;

	for (int i = 0; i < NUMBER_POS_READINGS; i++) {
		if (array_prop_readings[i] > 0) {
			avg_prop += array_prop_readings[i];
		} else {
			avg_prop -= array_prop_readings[i];
		}
	}
	return avg_prop /= NUMBER_POS_READINGS;
}

/*
 * @brief resets pid variables
 */
void reset_pid()
{
	last_error = 0;
	proportional = 0;
	derivative = 0;
	integral = 0;

	for (int i = 0; i < DIFF_ERROR_STACK; i++) {
		last_error_stack[i] = 0;
	}
	current_stack_idx = 0;
}

static int32_t trunc_to_range(int32_t value, int32_t min, int32_t max)
{
	int32_t trunc_value = value;

	if (value > max) {
		trunc_value = max;
	} else if (value < min) {
		trunc_value = min;
	}

	return trunc_value;
}

int16_t get_last_error(void)
{

	if (!FLAG_USE_DIFF_STACK) {
		return last_error;
	}

	return last_error_stack[current_stack_idx];
}

void set_new_stack_error(int32_t error)
{
	last_error_stack[current_stack_idx] = (int16_t)error;

	current_stack_idx += 1;
	current_stack_idx %= DIFF_ERROR_STACK;
}

/*
 * @brief pid calculation
 *
 * @param[in] proportional current measure of sensors
 */
int32_t pid(int32_t error)
{
	int32_t control;

	proportional = error;
	integral += error;
	integral = trunc_to_range(integral, -MAX_INTEGRAL, MAX_INTEGRAL);
	derivative = error - get_last_error();

	last_error = error;
	set_new_stack_error(error);

	control = proportional * k_p + integral * k_i + derivative * k_d;
	control = control / PID_CONTROL_DIVISOR;

	return control;
}

void set_kp(int kp)
{
	k_p = (uint16_t)kp;
}

void set_ki(int ki)
{
	k_i = (uint16_t)ki;
}

void set_kd(int kd)
{
	k_d = (uint16_t)kd;
}

int get_kp(void)
{
	return k_p;
}

int get_ki(void)
{
	return k_i;
}

int get_kd(void)
{
	return k_d;
}

int get_proportional(void)
{
	return proportional;
}

int get_integral(void)
{
	return integral;
}

int get_derivative(void)
{
	return derivative;
}
