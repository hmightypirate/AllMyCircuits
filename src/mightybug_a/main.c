#include "setup.h"
#include "pid.h"
#include "motors.h"
#include "sensors.h"
#include "led.h"

/**
 * @brief Initial setup and main loop
 */

int main(void)
{
  /* setup microcontroller */
  setup();

  /* reset motors */
  reset_target_velocity(INITIAL_TARGET_VELOCITY);

  /* reset sensors */
  //FIXME: better do some callibration
  hard_reset_sensors();
  
  /* reset pid */
  reset_pid();

  /* led: setting async period */
  set_async_period(LED_ASYNC_PERIOD);
  
  //FIXME: do some state machine here (callibration, running, etc)
  while(1)
    {
      /* read data from sensors */
      uint16_t sensor_value[NUM_SENSORS];
      read_line_sensors(sensor_value);
      int proportional = get_line_position(sensor_value);
      
      /* pid control */
      int error = 0;
      error = pid(proportional);
  
      /* motor control */
      if (!is_out_of_line())
        {
          motor_control(error);

          // blinking: normal behaviour
          async_blink();
        }
      else
        {
          // stop the motors if out of line
          stop_motors();

          // sets the led
          set_led();
        }
    }
  
  return 0;
}
