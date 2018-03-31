#include "setup.h"
#include "pid.h"
#include "motors.h"
#include "sensors.h"

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
      motor_control(error);
      
    }
  
  return 0;
}
