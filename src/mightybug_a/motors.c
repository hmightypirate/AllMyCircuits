#include "motors.h"

static int target_velocity = 0;

/* storing last velocities for debug */
static int last_left_vel = 0;
static int last_right_vel = 0;

/* pickle ticks: they depend on the wheels and the encoders measurement period */
uint16_t PICKLE_TURBO_TABLE[33] = {
    0, 0, 0, 18, 20, 24, 26, 30,                // pickle 0 to 224
    34, 38, 42, 46, 50, 54, 58, 60,             // pickle 256 to 480
    66, 70, 74, 78, 82, 86, 90, 94,             // pickle 512 to 736
    98, 102, 106, 110, 114, 118, 122, 126, 130, // pickle 768 to 992
};

/*
 * @brief set the state of left motor
 * 
 * @param[in] may be FORWARD or BACKWARD
 */
void set_left_motor_state(char state)
{
  if (state == FORWARD)
  {
    gpio_set(LEFT_MOTOR_IN1_PORT, LEFT_MOTOR_IN1_PIN);
    gpio_clear(LEFT_MOTOR_IN2_PORT, LEFT_MOTOR_IN2_PIN);
  }
  else if (state == BACKWARD)
  {
    gpio_clear(LEFT_MOTOR_IN1_PORT, LEFT_MOTOR_IN1_PIN);
    gpio_set(LEFT_MOTOR_IN2_PORT, LEFT_MOTOR_IN2_PIN);
  }
}

/*
 * @brief set the state of right motor
 * 
 * @param[in] may be FORWARD or BACKWARD
 */
void set_right_motor_state(char state)
{
  if (state == FORWARD)
  {
    gpio_set(RIGHT_MOTOR_IN1_PORT, RIGHT_MOTOR_IN1_PIN);
    gpio_clear(RIGHT_MOTOR_IN2_PORT, RIGHT_MOTOR_IN2_PIN);
  }
  else if (state == BACKWARD)
  {
    gpio_clear(RIGHT_MOTOR_IN1_PORT, RIGHT_MOTOR_IN1_PIN);
    gpio_set(RIGHT_MOTOR_IN2_PORT, RIGHT_MOTOR_IN2_PIN);
  }
}

/*
 * @brief set the value of the PWM of the left motor to control velocity
 * 
 * @param[in] an integer between 0 and 1023
 */
void set_left_motor_pwm(int value)
{
  if (LEFT_MOTOR_PWM_ANTIPHASE)
    value = MAX_PWM_VALUE - value;
  timer_set_oc_value(PWM_MOTOR_TIMER, LEFT_MOTOR_OUTPUT_CHANNEL, value);
}

/*
 * @brief set the value of the PWM of the right motor to control velocity
 * 
 * @param[in] an integer between 0 and 1023
 */
void set_right_motor_pwm(int value)
{
  if (RIGHT_MOTOR_PWM_ANTIPHASE)
    value = MAX_PWM_VALUE - value;
  timer_set_oc_value(PWM_MOTOR_TIMER, RIGHT_MOTOR_OUTPUT_CHANNEL, value);
}

/*
 * @brief obtain the pickle turbo (an extra acceleration if the motors are far from the objective (measured with the encoders)
 *
 */

int32_t get_pickle_turbo(int32_t velocity, uint32_t current_enc)
{
  int32_t new_velocity = velocity;

  int16_t idx_enc = velocity >> 5;

  int32_t min_vel = idx_enc << 5;
  int32_t max_vel = (idx_enc + 1) << 5;

  uint16_t target_enc = 0;
  if ((max_vel - velocity) > (velocity - min_vel))
  {
    target_enc = PICKLE_TURBO_TABLE[idx_enc + 1];
  }
  else
  {
    target_enc = PICKLE_TURBO_TABLE[idx_enc];
  }

  if (current_enc > target_enc)
  {
    // Is braking
    if ((current_enc - target_enc) > PICKLE_ENC_DISTANCE_DOWN)
    {
      // ANTIPICKLE
      new_velocity = velocity - PICKLE_TURBO_VEL;
    }
  }
  else
  {
    // Is accelearting
    if ((target_enc - current_enc) > PICKLE_ENC_DISTANCE_UP)
    {
      // PICKLE
      new_velocity = velocity + PICKLE_TURBO_VEL;
    }
  }

  return new_velocity;
}

/*
 * @brief resets target velocity to a given value (also de normal velocity)
 *
 * @param[in] target_vel initial target velocity
 */
void set_target_velocity(int target_vel)
{
  target_velocity = target_vel;
}

/*
 * @brief get current target velocity
 */
int get_target_velocity(void)
{
  return target_velocity;
}

/*
 * @brief helper function to truncate a value between min and max
 */
static int trunc_to_range(int value, int min, int max)
{
  int trunc_value = value;

  if (value < min)
    trunc_value = min;
  else if (value > max)
    trunc_value = max;

  return trunc_value;
}

/*
 * @brief set left motor velocity
 */
void set_left_motor_velocity(int velocity)
{
  velocity = trunc_to_range(velocity, MIN_VEL_MOTOR, MAX_VEL_MOTOR);

  last_left_vel = velocity;

  if (velocity >= 0)
  {
    set_left_motor_state(FORWARD);
    set_left_motor_pwm(velocity);
  }
  else
  {
    set_left_motor_state(BACKWARD);
    set_left_motor_pwm(-velocity);
  }
}

/*
 * @brief set right motor velocity
 */
void set_right_motor_velocity(int velocity)
{
  velocity = trunc_to_range(velocity, MIN_VEL_MOTOR, MAX_VEL_MOTOR);

  last_right_vel = velocity;

  if (velocity >= 0)
  {
    set_right_motor_state(FORWARD);
    set_right_motor_pwm(velocity);
  }
  else
  {
    set_right_motor_state(BACKWARD);
    set_right_motor_pwm(-velocity);
  }
}

/*
 * given a control signal, obtain the velocity for the left and right motors 
 *
 * @param[in] control from pid
 */
void motor_control(int control)
{
  control = trunc_to_range(control, -MAX_PID_ERROR, MAX_PID_ERROR);

  if (DEBUG_INERTIA_TEST)
  {
    // running in straight lines
    control = 0;
  }

  int32_t step_target_velocity = target_velocity;
  if (FLAG_MAX_VEL_DELAY)
  {
    step_target_velocity = get_next_constrained_target_velocity(target_velocity);
  }

  int32_t left_velocity = step_target_velocity + control;
  int32_t right_velocity = step_target_velocity - control;

  // aplying pickle
  if ((TURBO_PICKLE && TURBO_PICKLE_IN_CORNERS) ||
      (TURBO_PICKLE && get_running_state() == RUNNING_STLINE))
  {

    left_velocity = get_pickle_turbo(left_velocity,
                                     get_left_encoder_ticks());
    right_velocity = get_pickle_turbo(right_velocity,
                                      get_right_encoder_ticks());
  }

  set_left_motor_velocity(left_velocity);
  set_right_motor_velocity(right_velocity);

  // add real velocities to the vel delay structure
  if (FLAG_MAX_VEL_DELAY)
    increase_pointer_vel_delay(step_target_velocity);
}

/*
 * @brief stop the motors
 *
 * @note: this function (to date) did not change the motor control signals
 * 
 */
void stop_motors()
{
  set_left_motor_velocity(0);
  set_right_motor_velocity(0);
}

/*
 * @brief get last left velocity
 */
int get_last_left_vel()
{
  return last_left_vel;
}

/*
 * @brief get last right velocity
 */
int get_last_right_vel()
{
  return last_right_vel;
}
