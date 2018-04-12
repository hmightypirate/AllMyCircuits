#include "setup.h"
#include "pid.h"
#include "motors.h"
#include "sensors.h"
#include "led.h"
#include "fsm.h"

/*
 * @brief Initial setup and main loop
 */
int main(void)
{
  /* setup microcontroller */
  setup_microcontroller();

  /* reset motors */
  reset_target_velocity(INITIAL_TARGET_VELOCITY);

  /* reset sensors */
  //FIXME: better do some callibration

  if (SOFT_CALLIBRATION)
    {
      hard_reset_sensors();
    }
  else
    {
      /* reset callibration values (needed for callibration) */
      reset_callibration_values();
    }
  
  /* reset pid */
  reset_pid();

  /* led: setting async period */
  set_async_period(LED_ASYNC_PERIOD);

  /* enable sensors */
  enable_sensors();
  
  //FIXME: do some state machine here (callibration, running, etc)
  while(1)
    {
      /* read data from sensors */
      uint16_t sensor_value[NUM_SENSORS];
      read_line_sensors(sensor_value);

      state_e current_state = get_state();
      
      if (current_state == CALLIBRATION_STATE)
        {
          calibrate_sensors(sensor_value);

          /* FIXME: forcing running if all sensors are callibrated 
             better do it after some event is detected or some time
             has passed, or some command has been received
           */
          if (get_callibrated_sensors() == NUM_SENSORS)
            {
              update_state(GO_TO_RUN_EVENT);
            }
        }
      else
        {
          // Running 
          int proportional = get_line_position(sensor_value);
      
          /* pid control */
          int error = 0;
          error = pid(proportional);
  
          /* motor control */
          if (is_out_of_line())
            {
              // stop the motors if out of line
              stop_motors();

              // sets the led
              set_led();
            }
          else
            {
              motor_control(error);
              
              // blinking: normal behaviour
              async_blink();
            }
        }
    }
  
  return 0;
}
