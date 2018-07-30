#include "encoder.h"

static uint32_t new_left_encoder[SYSTICK_MEAS];
static uint32_t new_right_encoder[SYSTICK_MEAS];

static uint8_t measures_done = 0;

static uint32_t left_encoder_ticks = 0;
static uint32_t right_encoder_ticks = 0;
static uint32_t left_velocity = 0;
static uint32_t right_velocity = 0;

uint16_t systick_between_meas = SYSTICK_MEAS;
static uint16_t current_ticks = 0;


/*
 * @brief summing the contributions of the encoder in the window
 */
uint32_t get_sum_ticks(uint32_t *p_encoder)
{

  uint32_t sum_ticks = 0;
  for (int i=0; i<SYSTICK_MEAS; i++)
    {
      sum_ticks += p_encoder[i];
    }

  return sum_ticks;
}

/*
 * @brief get current encoder ticks (left engine)
 *
 * @note actually it is the difference between two measures
 */
uint32_t get_left_encoder_ticks(void)
{
  return left_encoder_ticks;
}

/*
 * @brief get current encoder ticks (right engine)
 *
 */
uint32_t get_right_encoder_ticks(void)
{
  return right_encoder_ticks;
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
 * @brief perform encoder measurements
 */
static uint32_t encoder_measurement(uint32_t new_encoder)
{

  if (new_encoder > WEIRD_ENCODER_MEAS)
    {
      /* summing 1 because the first tick is the max number of ticks */
      new_encoder = UINT16_MAX + 1 - new_encoder;
    }
  
  return new_encoder;
}

void reset_encoder_ticks(void)
{
  for (int i=0; i<SYSTICK_MEAS; i++)
    {
      new_left_encoder[i] = 0;
      new_right_encoder[i] = 0;
    }
}

/* 
 * @brief obtain velocities of encoders
 *
 * @note only update new velocities if a new measurement was done
 */
void update_velocities_encoders(void)
{
  if (measures_done > 0)
    {


      /* ticks per ms * SYSTICK_BETWEEN_MEAS */
      left_encoder_ticks = get_sum_ticks(new_left_encoder); // it is not +
      
      right_encoder_ticks = get_sum_ticks(new_right_encoder); // IT IS NOT +
      
      /* obtain velocity */
      left_velocity = (left_encoder_ticks * WHEEL_PERIMETER)/WHEEL_RATIO;
      right_velocity = (right_encoder_ticks * WHEEL_PERIMETER)/WHEEL_RATIO;

      
      measures_done = 0;
    }
}

/*
 * @brief obtain new measure of encoder ticks
 */
void update_encoder_ticks()
{

  measures_done += 1;
  
  uint32_t timer_tmp = (uint32_t)timer_get_counter(LEFT_ENCODER_TIMER); 
      
  new_left_encoder[current_ticks] = encoder_measurement(timer_tmp);
  timer_tmp = (uint32_t)timer_get_counter(RIGHT_ENCODER_TIMER);
  new_right_encoder[current_ticks] = encoder_measurement(timer_tmp);

  // Reset the timers
  timer_set_counter(LEFT_ENCODER_TIMER, 0);
  timer_set_counter(RIGHT_ENCODER_TIMER, 0);

  current_ticks += 1;
  if (current_ticks == systick_between_meas)
    {
      current_ticks = 0;
    }
}
