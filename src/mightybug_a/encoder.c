#include "encoder.h"

static uint16_t former_left_encoder = 0;
static uint16_t former_right_encoder = 0;
static uint16_t new_left_encoder = 0;
static uint16_t new_right_encoder = 0;

static uint8_t measures_done = 0;

static uint32_t left_encoder_ticks = 0;
static uint32_t right_encoder_ticks = 0;
static uint32_t left_velocity = 0;
static uint32_t right_velocity = 0;

uint16_t systick_between_meas = SYSTICK_MEAS;
static uint16_t current_ticks = 0;


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
static uint16_t encoder_measurement(uint16_t former_encoder, uint16_t new_encoder)
{
  uint16_t encoder_ticks = 0;
  
  if (former_encoder >= new_encoder)
    {
      encoder_ticks = former_encoder - new_encoder;
    }
  else
    {
      encoder_ticks = MAX_INT - new_encoder;
      encoder_ticks += former_encoder;
    }

  return encoder_ticks;
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
  
      left_encoder_ticks = encoder_measurement(former_left_encoder,
                                           new_left_encoder);

      right_encoder_ticks = encoder_measurement(former_right_encoder,
                                                new_right_encoder);
      
      /* ticks per ms */
      left_encoder_ticks = left_encoder_ticks/measures_done;
      right_encoder_ticks = right_encoder_ticks/measures_done;

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
  current_ticks += 1;
  if (current_ticks == systick_between_meas)
    {
      new_left_encoder = (uint16_t)timer_get_counter(LEFT_ENCODER_TIMER);
      new_right_encoder = (uint16_t)timer_get_counter(RIGHT_ENCODER_TIMER);
      
      measures_done += 1;
      current_ticks = 0;
    }
}
