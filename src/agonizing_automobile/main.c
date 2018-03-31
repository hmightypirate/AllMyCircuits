#include "setup.h"
#include "pid.h"
#include "motors.h"

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
  //TODO
  
  /* reset pid */
  reset_pid();

  while(1)
    {
      //TODO read sensors
      int proportional = 0;
      
      /* pid control */
      int error = 0;
      error = pid(proportional);

      /* motor control */
      motor_control(error);
      
    }
  
  return 0;
}
