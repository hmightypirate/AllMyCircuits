#include "motors.h"

static int target_velocity = 0;

/* storing last velocities for debug */
static int last_left_vel = 0;
static int last_right_vel = 0;

/*
 * @brief configure the motor for forward move
 *
 * @param[in] gpio_forward gpio of input 1 of motor control
 * @param[in] port_forward port of input 1 of motor control
 * @param[in] gpio_backward gpio of input 2 of motor control
 * @param[in] port_backward port of input 2 of motor control
 */
void move_forward(uint32_t gpio_forward,
                  uint32_t port_forward,
                  uint32_t gpio_backward,
                  uint32_t port_backward)
{
  gpio_set(gpio_forward, port_forward);
  gpio_clear(gpio_backward, port_backward);
}


/*
 * @brief configure the motor for backward move
 *
 * @param[in] gpio_forward gpio of input 1 of motor control
 * @param[in] port_forward port of input 1 of motor control
 * @param[in] gpio_backward gpio of input 2 of motor control
 * @param[in] port_backward port of input 2 of motor control
 */
void move_backward(uint32_t gpio_forward,
                  uint32_t port_forward,
                  uint32_t gpio_backward,
                  uint32_t port_backward)
{
  gpio_clear(gpio_forward, port_forward);
  gpio_set(gpio_backward, port_backward);
}

/*
 * @brief sets pwm for motor velocity
 *
 * @param[in] left_vel velocity of left motor
 * @param[in] right_vel velocity of right motor
 */
void set_motor_velocity(int left_vel, int right_vel)
{
  if (LEFT_INVERTED)
    {
      left_vel = MAX_VEL_MOTOR - left_vel;
    }

  if (RIGHT_INVERTED)
    {
      right_vel = MAX_VEL_MOTOR - right_vel;
    }

  timer_set_oc_value(TIM4, TIM_OC3, left_vel);
  timer_set_oc_value(TIM4, TIM_OC4, right_vel);
 
}

/*
 * @brief resets target velocity to a given value
 *
 * @param[in] target_vel initial target velocity
 */
void reset_target_velocity(int target_vel)
{
  target_velocity = target_vel;
}

/*
 * given an error, obtain the velocity for the left and right motors 
 *
 * @param[in] error pid error (typically line position error) 
 */
void motor_control(int error)
{
  int left_vel = 0;
  int right_vel = 0;

  if (error > MAX_PID_ERROR)
    {
      error = MAX_PID_ERROR;
    }
  else if (error < -MAX_PID_ERROR)
    {
      error = -MAX_PID_ERROR;
    }

  left_vel = target_velocity + error;
  right_vel = target_velocity - error;
  
  if (left_vel > MAX_VEL_MOTOR)
    {
      left_vel = MAX_VEL_MOTOR;
    }
  else if (left_vel < -MAX_VEL_MOTOR)
    {
      left_vel = -MAX_VEL_MOTOR;
    }

  if (right_vel > MAX_VEL_MOTOR)
    {
      right_vel = MAX_VEL_MOTOR;
    }
  else if (right_vel < MAX_VEL_MOTOR)
    {
      right_vel = -MAX_VEL_MOTOR;
    }

  last_left_vel = left_vel;
  last_right_vel = right_vel;

  if (left_vel >= 0)
    {
      /* move left motor forward */
      move_forward(LEFT_MOTOR_IN1_GPIO,
                   LEFT_MOTOR_IN1_PORT,
                   LEFT_MOTOR_IN2_GPIO,
                   LEFT_MOTOR_IN2_PORT);
    }
  else
    {
      /* move left motor backward */
      move_backward(LEFT_MOTOR_IN1_GPIO,
                    LEFT_MOTOR_IN1_PORT,
                    LEFT_MOTOR_IN2_GPIO,
                    LEFT_MOTOR_IN2_PORT);

      left_vel = -left_vel;
    }

  if (right_vel >= 0)
    {
      /* move right motor forward */
      move_forward(RIGHT_MOTOR_IN1_GPIO,
                   RIGHT_MOTOR_IN1_PORT,
                   RIGHT_MOTOR_IN2_GPIO,
                   RIGHT_MOTOR_IN2_PORT);
    }
  else
    {
      /* move right motor backward */
      move_backward(RIGHT_MOTOR_IN1_GPIO,
                    RIGHT_MOTOR_IN1_PORT,
                    RIGHT_MOTOR_IN2_GPIO,
                    RIGHT_MOTOR_IN2_PORT);

      right_vel = -right_vel;
    }
  
  set_motor_velocity(left_vel, right_vel);
  
}

  
