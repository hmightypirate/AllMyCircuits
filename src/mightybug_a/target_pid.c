#include "target_pid.h"

static uint16_t k_p = 500; // Proportional constant
static uint16_t k_i = 0;   // Integral constant
static uint16_t k_d = 1500;   // Derivative constant

static int32_t integral = 0;
static int16_t derivative = 0;
static int16_t proportional = 0;
static int16_t last_error = 0;

static int16_t last_error_stack[DIFF_ERROR_STACK_TARGET];
static int16_t current_stack_idx = 0;



/*
 * @brief resets pid variables
 */
void reset_pid_target()
{
	last_error = 0;
	proportional = 0;
	derivative = 0;
	integral = 0;

	for (int i = 0; i < DIFF_ERROR_STACK_TARGET; i++) {
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

int16_t get_last_error_target(void)
{

	return last_error_stack[current_stack_idx];
}

static void set_new_stack_error(int32_t error)
{
	last_error_stack[current_stack_idx] = (int16_t)error;

	current_stack_idx += 1;
	current_stack_idx %= DIFF_ERROR_STACK_TARGET;
}

/*
 * @brief pid calculation
 *
 * @param[in] proportional current measure of sensors
 */
int32_t pid_target(int32_t error)
{
	int32_t control;

	proportional = error;
	integral += error;
	integral = trunc_to_range(integral, -MAX_INTEGRAL_TARGET, MAX_INTEGRAL_TARGET);
	derivative = error - get_last_error_target();

	last_error = error;
	set_new_stack_error(error);

	control = proportional * k_p + integral * k_i + derivative * k_d;
	control = control / PID_CONTROL_DIVISOR_TARGET;

	return control;
}

void set_kp_target(int kp)
{
	k_p = (uint16_t)kp;
}

void set_ki_target(int ki)
{
	k_i = (uint16_t)ki;
}

void set_kd_target(int kd)
{
	k_d = (uint16_t)kd;
}

int get_kp_target(void)
{
	return k_p;
}

int get_ki_target(void)
{
	return k_i;
}

int get_kd_target(void)
{
	return k_d;
}
