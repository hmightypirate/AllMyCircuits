#include "encoder.h"

static volatile uint32_t left_encoder_array[ENCODER_BUFFER_LEN];
static volatile uint32_t right_encoder_array[ENCODER_BUFFER_LEN];

static volatile int32_t left_encoder_acc_array[ACC_MEAS];
static volatile int32_t right_encoder_acc_array[ACC_MEAS];

static uint8_t measures_done = 0;

static uint32_t left_encoder = 0;
static uint32_t right_encoder = 0;
static uint32_t left_velocity = 0;
static uint32_t right_velocity = 0;

static uint32_t last_left_encoder = 0;
static uint32_t last_right_encoder = 0;

static uint16_t acc_array_index = 0;
static int32_t left_encoder_acc = 0;
static int32_t right_encoder_acc = 0;

static uint16_t encoder_array_index = 0;

/*
 * @brief get the absolute diff between encoders
 */

int16_t get_abs_diff_encoders(void)
{
	if (left_encoder > right_encoder) {
		return (int16_t)(left_encoder - right_encoder);
	} else {
		return (int16_t)(right_encoder - left_encoder);
	}
}

/*
 * @brief updating the acceleration of the encoders
 */
void update_acc_encoders(int32_t acc_left, int32_t acc_right)
{
	left_encoder_acc_array[acc_array_index] = acc_left;
	right_encoder_acc_array[acc_array_index] = acc_right;

	acc_array_index += 1;
	acc_array_index %= ACC_MEAS;
}

/*
 * @brief summing the contributions of the acc of the encoder in the window
 */
int32_t get_sum_acc_array(volatile int32_t *p_encoder)
{
	int32_t sum = 0;
	for (int i = 0; i < ACC_MEAS; i++) {
		sum += p_encoder[i];
	}
	return sum;
}

/*
 * @brief summing the contributions of the encoder in the window
 */
uint32_t get_sum_encoder_array(volatile uint32_t *p_encoder)
{
	uint32_t sum = 0;
	for (int i = 0; i < ENCODER_BUFFER_LEN; i++) {
		sum += p_encoder[i];
	}
	return sum;
}

/*
 * @brief get current encoder ticks (left engine)
 *
 * @note actually it is the difference between two measures
 */
uint32_t get_left_encoder_ticks(void)
{
	return left_encoder;
}

/*
 * @brief get current encoder ticks (right engine)
 *
 */
uint32_t get_right_encoder_ticks(void)
{
	return right_encoder;
}

/*
 * @brief get current velocity of left encoder
 */
uint32_t get_left_velocity(void)
{
	return left_velocity;
}

/*
 * @brief get current velocity of right encoder
 */
uint32_t get_right_velocity(void)
{
	return right_velocity;
}

/*
 * @brief get the current acceleration of left encoder
 */
int32_t get_left_acc(void)
{
	return left_encoder_acc;
}

/*
 * @brief get the current acceleration of the right encoder
 */
int32_t get_right_acc(void)
{
	return right_encoder_acc;
}

/*
 * @brief perform encoder measurements
 */
static uint32_t encoder_measurement(uint32_t value)
{

	if (value > WEIRD_ENCODER_MEAS) {
		/* summing 1 because the first tick is the max number of ticks
		 */
		value = UINT16_MAX + 1 - value;
	}

	return value;
}

void reset_encoder_ticks(void)
{
	for (int i = 0; i < ENCODER_BUFFER_LEN; i++) {
		left_encoder_array[i] = 0;
		right_encoder_array[i] = 0;
	}
}

/*
 * @brief obtain velocities of encoders
 *
 * @note only update new velocities if a new measurement was done
 */
void update_velocities_encoders(void)
{
	if (measures_done > 0) {
		measures_done = 0;

		last_left_encoder = left_encoder;
		last_right_encoder = right_encoder;

		/* sum array */
		left_encoder = get_sum_encoder_array(left_encoder_array);
		right_encoder = get_sum_encoder_array(right_encoder_array);

		/* obtain velocity */
		left_velocity = (left_encoder * WHEEL_PERIMETER) / WHEEL_RATIO;
		right_velocity =
		    (right_encoder * WHEEL_PERIMETER) / WHEEL_RATIO;

		/* obtain acceleration info */
		update_acc_encoders((int32_t)left_encoder - last_left_encoder,
				    (int32_t)right_encoder -
					last_right_encoder);

		left_encoder_acc = get_sum_acc_array(left_encoder_acc_array);
		right_encoder_acc = get_sum_acc_array(right_encoder_acc_array);
	}
}

/*
 * @brief obtain new measure of encoder ticks
 */
void update_encoder_ticks()
{

	measures_done += 1;

	// Get left encoder
	uint32_t timer_tmp = (uint32_t)timer_get_counter(LEFT_ENCODER_TIMER);
	left_encoder_array[encoder_array_index] =
	    encoder_measurement(timer_tmp);

	// Get right encoder
	timer_tmp = (uint32_t)timer_get_counter(RIGHT_ENCODER_TIMER);
	right_encoder_array[encoder_array_index] =
	    encoder_measurement(timer_tmp);

	// Reset the timers
	timer_set_counter(LEFT_ENCODER_TIMER, 0);
	timer_set_counter(RIGHT_ENCODER_TIMER, 0);

	// Update current_tick
	encoder_array_index += 1;
	encoder_array_index %= ENCODER_BUFFER_LEN;
}

/*
 * @brief obtain last measurement pointer to encoder ticks
 */
uint16_t get_last_meas_pointer()
{
	if (encoder_array_index == 0) {
		return ENCODER_BUFFER_LEN - 1;
	} else {
		return encoder_array_index - 1;
	}
}

/*
 * @brief get last left tick
 */
uint32_t get_last_left_ticks()
{
	return left_encoder_array[get_last_meas_pointer()];
}

/*
 * @brief get last right ticks
 */
uint32_t get_last_right_ticks()
{
	return right_encoder_array[get_last_meas_pointer()];
}
