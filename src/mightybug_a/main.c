#include "setup.h"
#include "pid.h"
#include "motors.h"
#include "sensors.h"
#include "led.h"
#include "fsm.h"
#include "cli.h"
#include "libjukebox.h"
#include "cron.h"
#include "libmusic.h"

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

  /* setup jukebox */
  jukebox_setup();

  clear_led();

  /*
  uint32_t last_milisec = 0;
  uint32_t millisecs_since_start = 0;

  while(1)
    {
      millisecs_since_start = get_millisecs_since_start();

      if ((millisecs_since_start % 1000) == 0) {
        
        gpio_toggle(GPIOC, GPIO13);
      }
      
      play_note(A4);

    }
  
  */
  
  //FIXME: do some state machine here (callibration, running, etc)
  while(1)
    {
      if (is_command_received()) {
        execute_command();
      }

      
      /* read data from sensors */
      uint16_t sensor_value[NUM_SENSORS];
      read_line_sensors(sensor_value);

 
      state_e current_state = get_state();
      
      if (current_state == CALLIBRATION_STATE)
        {
          
          calibrate_sensors(sensor_value);
          
          // /* FIXME: forcing running if all sensors are callibrated 
          //    better do it after some event is detected or some time
          //    has passed, or some command has been received
          //  */
          // if (get_callibrated_sensors() == NUM_SENSORS)
          //   {
          //     update_state(GO_TO_RUN_EVENT);
          //   }
      
          /* stop motors during callibration */
          stop_motors();

          /* led is off during callibration */
          
          set_led();
          /* set song and play in loop */
          jukebox_setcurrent_song(CALLIBRATION_SONG);
          jukebox_play_in_loop(get_millisecs_since_start());          
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
              clear_led();

              /* set song and play in loop */
              jukebox_setcurrent_song(OUT_OF_LINE_SONG);
              jukebox_play_in_loop(get_millisecs_since_start());
              
            }
          else
            {
              motor_control(error);
              
              // blinking: normal behaviour
              async_blink();

              /* stop the music */
              stop_music_play();
            }
        }
    }
  
  return 0;
}
