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
#include "encoder.h"
#include "vbatt.h"
#include "commons.h"
#include "keypad.h"
#include "keypad_menu.h"
#include "telemetry.h"


void music_update(int millis)
{
  state_e current_state = get_state();
  if (current_state == CALLIBRATION_STATE) {  
    jukebox_setcurrent_song(CALLIBRATION_SONG);
    jukebox_play_in_loop(millis);
  } else if (current_state == NO_BATTERY_STATE) {
    jukebox_setcurrent_song(OUT_OF_BATTERY_SONG);
    jukebox_play_in_loop(millis);
  } else if (current_state == PIDANDVEL_MAPPING_STATE) {
    jukebox_setcurrent_song(get_map_song(get_current_pidvel_map()));
    jukebox_play_in_loop(millis);
  } else if (TURBO_PITCH_DEBUG && (current_state == RUNNING_STATE)) {

    if (get_running_state() == RUNNING_STLINE)
      {
	jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
	jukebox_play_in_loop(millis);
      }
    else if (get_running_state() == RUNNING_NORMAL)
      {
	jukebox_setcurrent_song(TENOR_BEAT_ORDER);
	jukebox_play_in_loop(millis);
      }
    else if (get_running_state() == RUNNING_NOOL)
      {
	jukebox_setcurrent_song(BASS_BEAT_ORDER);
	jukebox_play_in_loop(millis);
      }    
    else
      {
	jukebox_setcurrent_song(SONG_TWO_BEAT_ORDER);
	jukebox_play_in_loop(millis);
      }
      
  } else {
    if (is_out_of_line()) {
      jukebox_setcurrent_song(OUT_OF_LINE_SONG);
      jukebox_play_in_loop(millis);
    } else {
      // Running or delayed run states
      stop_music_play();
    }
  }
}

/*
 * @brief Initial setup and main loop
 */
int main(void)
{
  /* setup microcontroller */
  setup_microcontroller();
  
  /* reset motors and pid to the current mapping */
  force_mapping_to_current();
  
  /* reset sensors */
  //FIXME: better do some callibration
  if (!SOFT_CALLIBRATION) {
      hard_reset_sensors();
  } else {
      /* reset callibration values (needed for callibration) */
     reset_calibration_values();
  }
  
  /* reset pid */
  reset_pid();

  /* reset readings for turbo calculation */
  reset_prop_readings();

  /* led: setting async period */
  set_async_period(LED_ASYNC_PERIOD);

  /* enable sensors */
  enable_line_sensors();

  /* setup jukebox */
  jukebox_setup();

  /* Setup keypad */
  uint32_t button_port_array[NUM_BUTTONS] = {(uint32_t) BUTTON0_PORT,
                                             (uint32_t) BUTTON1_PORT,
                                             (uint32_t) BUTTON2_PORT};
  uint16_t button_pin_array[NUM_BUTTONS] = {(uint16_t) BUTTON0_PIN,
                                            (uint16_t) BUTTON1_PIN,
                                            (uint16_t) BUTTON2_PIN};  
  keypad_setup(get_millisecs_since_start(),
               button_port_array,
               button_pin_array);
  
  clear_led();

  uint32_t last_loop_execution_ms = 0;
  uint32_t sync_iterations = 0;
  
  while(1) {
    uint32_t current_loop_millisecs = get_millisecs_since_start();
    sync_iterations += 1;
    
    if (is_command_received()) {
      execute_command();
    }

    /* should read battery? 
       battery measurement could have a different period than sensor/pid reads
     */
    if (current_loop_millisecs % VBATT_SYS_BETWEEN_READS == 0) {
      // Check if battery drained
      if (has_batt_drained()) {
        update_state(OUT_OF_BATTERY_EVENT); 
      }
    }

    state_e current_state = get_state();

    music_update(current_loop_millisecs);
    keypad_loop(current_loop_millisecs);
    menu_functions(current_loop_millisecs);

    // loop out of period
    if (current_state == SET_NORMAL_MODE_STATE)
      {
        set_target_as_normal();
        /* change pid normal */
        reset_pids_normal();
        set_state(RUNNING_STATE); //FIXME this assignment is local (and useless)
        set_running_state(RUNNING_NORMAL);

	// reset variables used for special acc/dec in NORMAL mode
	reset_sequential_readings();
      }
    else if (current_state == SET_TURBO_MODE_STATE)
      {
        set_target_as_turbo();
        /* change pid consts */
        reset_pids_turbo();
        set_state(RUNNING_STATE);  //FIXME this assignment is local (and useless)
        set_running_state(RUNNING_STLINE);
      }
    else if (current_state == SET_NOOL_MODE_STATE)
      {
	set_target_as_nool();

	/* change pid consts */
	reset_pids_nool();
	set_state(RUNNING_STATE);
	set_running_state(RUNNING_NOOL);
      }
    
    // loop is executed at a fixed period of time
    if ((current_loop_millisecs - last_loop_execution_ms) >= TIME_BETWEEN_LOOP_ITS) {
        
      last_loop_execution_ms = current_loop_millisecs;
    
      /* read data from sensors */
      uint16_t sensor_value[NUM_SENSORS];
      read_line_sensors(sensor_value);
        
      if (current_state == CALLIBRATION_STATE) {        
        calibrate_sensors(sensor_value);
                     
        /* stop motors during calibration */
        stop_motors();
         /* led is on during callibration */
        set_led();

      } else if (current_state == IDLE_STATE) {

        /* reset out of line measurements, resets callibration */
        reset_all_inline();
        reset_calibration_values();
        stop_motors();

        /* Clear led during idle state */
        clear_led();
        
      } else if (current_state == NO_BATTERY_STATE) {
            
        /* Disable sensors */
        disable_line_sensors();
            
        /* Stop motors */
        stop_motors();
            
        /* Led off */
        clear_led();

      } else if (current_state == DELAYED_START_STATE)
        {
          /* Stop motors */
          stop_motors();
          
          if (current_loop_millisecs - get_delay_start_time() >
              DELAYED_START_MS)
            {              
              reset_encoder_ticks();
              update_state(GO_TO_RUN_EVENT);
            }

          /* Led on */
          set_led();
        }
      else if (current_state == PIDANDVEL_MAPPING_STATE)
        {
          /* stop motors */
          stop_motors();
          if (current_loop_millisecs - get_pidvel_map_time() >
              DELAYED_PIDVEL_CHANGE_MS)
            {
              // Return to callibration if
              stop_music_play();
              update_state(FORCE_CALLIBRATION_EVENT);
              pull_enable_jukebox();
            }

          set_led();
        }
      else if (current_state == PIDANDVEL_CHANGE_STATE)
        {
          //change the mapping
          select_next_pidvel_map();

          /* sets the ms in mapping state to the current time */
          set_pidvel_map_time(current_loop_millisecs);

          update_state(FORCE_PIDANDVELMAP_EVENT);       
 
      } else {
            
        // Running 
        int proportional = get_line_position(sensor_value);

	// update proportional sequence
	update_sequential_readings(proportional, get_proportional());
	
        // Meas turbo mode
        if (sync_iterations % TIME_BETWEEN_STORE_POS == 0)
          {
            set_new_reading(proportional);

            // Check that the minimum number of readings was performed
            if (is_enable_avg_readings())
              {
                // Obtain the average number of readings
                int16_t avg_proportional = get_avg_abs_readings();

                get_next_running_state(avg_proportional);
              }
          }


	// Accelerate/Break in NORMAL mode
	if ((ENABLE_INCDEC_NORMAL_FLAG) && (sync_iterations % ITS_INCDEC_NORMAL == 0))
	  {
	    update_target_normal();
	  }
        
        /* pid control */
        int error = 0;
        error = pid(proportional);
            
        /* motor control 

	   stops if:
	   1) is out of line and not delay is used
	   2) is out of line and has exceeded the maximum time allowed
	   3) the time of an inertia test is over

	 */
        if ((is_out_of_line() && !DEBUG_INERTIA_TEST &&
	     (!FLAG_DELAY_STOP_OUT_OF_LINE ||
	      (FLAG_DELAY_STOP_OUT_OF_LINE &&
	       exceeds_time_out_of_line(current_loop_millisecs)))) ||
            (DEBUG_INERTIA_TEST &&  (current_loop_millisecs - get_running_ms() > DEBUG_INERTIA_TIME_MS))) {
          // stop the motors if out of line
          stop_motors();
                
          // led off
          clear_led();

          // Send car to callibration if reached the end of line
          if (get_all_inline())
            {
              update_state(FORCE_IDLE_EVENT);
            }
                            
        } else {
	  
          motor_control(error);
                
          // blinking: normal behaviour
          async_blink();

	  // Set the current ms (inline)
	  if (!is_out_of_line())
	    {
	      update_ms_inline(current_loop_millisecs);
	    }
         }

        // update encoders velocity
        update_velocities_encoders();

        if (TELEMETRY) {
          print_telemetry(current_loop_millisecs);
        }

      }
    }
  }

  return 0;
}
