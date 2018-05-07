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

void set_left_motor_pwm(int value)
{
  if (LEFT_INVERTED) value = MAX_VEL_MOTOR - value;
  timer_set_oc_value(TIM4, TIM_OC3, value);
}

void set_right_motor_pwm(int value)
{
  if (RIGHT_INVERTED) value = MAX_VEL_MOTOR - value;
  timer_set_oc_value(TIM4, TIM_OC4, value);
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
 * @brief get current target velocity
 */
int get_target_velocity(void) {
  return target_velocity;
}


void left_motor_velocity(int velocity)
{
  if (velocity > MAX_VEL_MOTOR) {
      velocity = MAX_VEL_MOTOR;
  }
  else if (velocity < -MAX_VEL_MOTOR) {
      velocity = -MAX_VEL_MOTOR;
  }

  last_left_vel = velocity;

  if (velocity >= 0) {
      /* move left motor forward */
      move_forward(LEFT_MOTOR_IN1_PORT,
                   LEFT_MOTOR_IN1_PIN,
                   LEFT_MOTOR_IN2_PORT,
                   LEFT_MOTOR_IN2_PIN);
  } else {
      /* move left motor backward */
      move_backward(LEFT_MOTOR_IN1_PORT,
                    LEFT_MOTOR_IN1_PIN,
                    LEFT_MOTOR_IN2_PORT,
                    LEFT_MOTOR_IN2_PIN);

      velocity = -velocity;
  }

  set_left_motor_pwm(velocity);
}

void right_motor_velocity(int velocity)
{
  if (velocity > MAX_VEL_MOTOR)
    {
      velocity = MAX_VEL_MOTOR;
    }
  else if (velocity < -MAX_VEL_MOTOR)
    {
      velocity = -MAX_VEL_MOTOR;
    }
  
  last_right_vel = velocity;

  if (velocity >= 0)
    {
      /* move right motor forward */
      move_forward(RIGHT_MOTOR_IN1_PORT,
                   RIGHT_MOTOR_IN1_PIN,
                   RIGHT_MOTOR_IN2_PORT,
                   RIGHT_MOTOR_IN2_PIN);
    }
  else
    {
      /* move right motor backward */
      move_backward(RIGHT_MOTOR_IN1_PORT,
                    RIGHT_MOTOR_IN1_PIN,
                    RIGHT_MOTOR_IN2_PORT,
                    RIGHT_MOTOR_IN2_PIN);

      velocity = -velocity;
    }
  
  set_right_motor_pwm(velocity);

}

/*
 * given an error, obtain the velocity for the left and right motors 
 *
 * @param[in] error pid error (typically line position error) 
 */
void motor_control(int control)
{
  int left_vel = 0;
  int right_vel = 0;

  if (control > MAX_PID_ERROR)
    {
      control = MAX_PID_ERROR;
    }
  else if (control < -MAX_PID_ERROR)
    {
      control = -MAX_PID_ERROR;
    }

  left_vel = target_velocity - control;
  right_vel = target_velocity + control;

  left_motor_velocity(left_vel);
  right_motor_velocity(right_vel);
  
}

/*
 * @brief stop the motors
 *
 * @note: this function (to date) did not change the motor control signals
 * 
 */
void stop_motors()
{
  left_motor_velocity(0);
  right_motor_velocity(0);
}

