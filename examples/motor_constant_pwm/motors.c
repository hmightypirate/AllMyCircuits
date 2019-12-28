#include "motors.h"


/* storing last velocities for debug */
static int32_t last_left_vel = 0;

/*
 * @brief helper function to truncate a value between min and max
 */
static int32_t trunc_to_range(int32_t value, int32_t min, int32_t max)
{
	int32_t trunc_value = value;

	if (value < min)
		trunc_value = min;
	else if (value > max)
		trunc_value = max;

	return trunc_value;
}

/*
 * @brief set the state of left motor
 *
 * @param[in] may be FORWARD or BACKWARD
 */
void set_left_motor_state(uint8_t state)
{
	if (state == FORWARD) {
		gpio_set(LEFT_MOTOR_IN1_PORT, LEFT_MOTOR_IN1_PIN);
		gpio_clear(LEFT_MOTOR_IN2_PORT, LEFT_MOTOR_IN2_PIN);
	} else if (state == BACKWARD) {
		gpio_clear(LEFT_MOTOR_IN1_PORT, LEFT_MOTOR_IN1_PIN);
		gpio_set(LEFT_MOTOR_IN2_PORT, LEFT_MOTOR_IN2_PIN);
	}
}

/*
 * @brief set the value of the PWM of the left motor to control velocity
 *
 * @param[in] an integer between 0 and 1023
 */
void set_left_motor_pwm(int32_t value)
{
	if (LEFT_MOTOR_PWM_ANTIPHASE)
		value = MAX_PWM_VALUE - value;
	timer_set_oc_value(PWM_MOTOR_TIMER, LEFT_MOTOR_OUTPUT_CHANNEL, value);
}


/*
 * @brief set left motor velocity
 */
void set_left_motor_velocity(int32_t velocity)
{
	velocity = trunc_to_range(velocity, MIN_VEL_MOTOR, MAX_VEL_MOTOR);

	last_left_vel = velocity;

	if (velocity >= 0) {
		set_left_motor_state(FORWARD);
		set_left_motor_pwm(velocity);
	} else {
		set_left_motor_state(BACKWARD);
		set_left_motor_pwm(-velocity);
	}
}

/*
 * @brief get last left velocity
 */
int32_t get_last_left_vel()
{
	return last_left_vel;
}
