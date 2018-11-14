#include "main.h"

uint16_t line_sensor_value[NUM_SENSORS];
uint32_t sync_iterations = 0;

void check_running_state(void)
{
  rnstate_e running_state = get_running_state();

  switch (running_state)
  {
  case RUNNING_STLINE:
    jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
    break;
  case RUNNING_NORMAL:
    jukebox_setcurrent_song(TENOR_BEAT_ORDER);
    break;
  case RUNNING_NOOL:
    jukebox_setcurrent_song(BASS_BEAT_ORDER);
    break;
  default:
    jukebox_setcurrent_song(SONG_TWO_BEAT_ORDER);
  }
}

void music_update(int millis)
{
  state_e current_state = get_state();

  switch (current_state)
  {
  case CALLIBRATION_STATE:
    jukebox_setcurrent_song(CALLIBRATION_SONG);
    jukebox_play_in_loop(millis);
    break;
  case NO_BATTERY_STATE:
    jukebox_setcurrent_song(OUT_OF_BATTERY_SONG);
    jukebox_play_in_loop(millis);
    break;
  case PIDANDVEL_MAPPING_STATE:
    jukebox_setcurrent_song(get_map_song(get_current_pidvel_map()));
    jukebox_play_in_loop(millis);
    break;
  case RUNNING_STATE:
    if (TURBO_PITCH_DEBUG)
    {
      check_running_state();
      jukebox_play_in_loop(millis);
    }
    break;
  default:
    if (is_out_of_line())
    {
      jukebox_setcurrent_song(OUT_OF_LINE_SONG);
      jukebox_play_in_loop(millis);
    }
    else
    {
      // Running or delayed run states
      stop_music_play();
    }
  }
}

void calibration_state(void)
{
  calibrate_sensors(line_sensor_value);

  /* stop motors during calibration */
  stop_motors();
  /* led is on during callibration */
  set_led_mode(LED2, ON);
}

void idle_state(void)
{
  /* reset out of line measurements, resets callibration */
  reset_all_inline();
  reset_calibration_values();
  stop_motors();

  /* Clear led during idle state */
  set_led_mode(LED2, OFF);
}

void out_of_battery_state(void)
{
  /* Disable sensors */
  disable_line_sensors();

  /* Stop motors */
  stop_motors();

  /* Led off */
  set_led_mode(LED2, OFF);
}

void delayed_start_state(uint32_t current_millis)
{
  /* Stop motors */
  stop_motors();

  if (current_millis - get_delay_start_time() > DELAYED_START_MS)
  {
    // Reset pointer (starting from the beginning)
    if (FLAG_CIRCUIT_MAPPING)
      reset_mapping_pointer();

    if (FLAG_MAX_VEL_DELAY)
      reset_veldelay();
    reset_encoder_ticks();
    update_state(GO_TO_RUN_EVENT);
  }

  /* Led on */
  set_led_mode(LED2, ON);
}

void pid_and_vel_mapping_state(uint32_t current_millis)
{
  /* stop motors */
  stop_motors();
  if (current_millis - get_pidvel_map_time() > DELAYED_PIDVEL_CHANGE_MS)
  {
    // Return to callibration if
    stop_music_play();
    update_state(FORCE_CALLIBRATION_EVENT);
    pull_enable_jukebox();
  }
}

void pid_and_vel_change_state(uint32_t current_millis)
{
  //change the mapping
  select_next_pidvel_map();

  /* sets the ms in mapping state to the current time */
  set_pidvel_map_time(current_millis);

  update_state(FORCE_PIDANDVELMAP_EVENT);

  if (get_current_pidvel_map() == 0)
  {
    set_led_mode(LED1, BLINK);
    set_led_mode(LED2, BLINK);
  }
  else if (get_current_pidvel_map() == 1)
  {
    set_led_mode(LED1, DOUBLE_BLINK);
    set_led_mode(LED2, DOUBLE_BLINK);
  }
  else if (get_current_pidvel_map() == 2)
  {
    set_led_mode(LED1, TRIPLE_BLINK);
    set_led_mode(LED2, TRIPLE_BLINK);
  }
}

void running_state(uint32_t current_millis)
{
  // Running
  int proportional = get_line_position(line_sensor_value);

  // update proportional sequence
  update_sequential_readings(proportional, get_proportional());

  // Meas turbo mode
  if (sync_iterations % TIME_BETWEEN_STORE_POS == 0)
  {
    set_new_reading(proportional);

    if (!USE_ENCODERS_FOR_STATE)
    {

      // Check that the minimum number of readings was performed
      if (is_enable_avg_readings())
      {
        // Obtain the average number of readings
        int16_t avg_proportional = get_avg_abs_readings();

        get_next_running_state(avg_proportional);
      }
    }
    else
    {
      get_next_running_state(get_abs_diff_encoders());
    }
  }

  // Accelerate/Break in NORMAL mode
  if (!USE_ENCODERS_FOR_INCDEC)
  {
    if ((ENABLE_INCDEC_NORMAL_FLAG) && (sync_iterations % ITS_INCDEC_NORMAL == 0))
    {
      update_target_normal();
    }
  }
  else
  {
    if (ENABLE_INCDEC_NORMAL_FLAG)
    {
      update_target_normal_with_encoders();
    }
  }

  // Avoid wheelie at start

  if (FLAG_ANTI_WHEELIE_START)
  {
    set_vel_antiwheelie(current_millis);
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
         exceeds_time_out_of_line(current_millis)))) ||
      (DEBUG_INERTIA_TEST && (current_millis - get_running_ms() > DEBUG_INERTIA_TIME_MS)))
  {
    // stop the motors if out of line
    stop_motors();

    // led off
    set_led_mode(LED2, OFF);

    // Send car to callibration if reached the end of line
    if (get_all_inline())
    {
      update_state(FORCE_IDLE_EVENT);
    }
  }
  else
  {

    motor_control(error);

    // blinking: normal behaviour
    set_led_mode(LED2, BLINK);

    // Set the current ms (inline)
    if (!is_out_of_line())
    {
      update_ms_inline(current_millis);
    }

    // Do circuit mapping
    if (FLAG_CIRCUIT_MAPPING)
    {
      do_circuit_mapping();
    }
  }

  // update encoders velocity
  update_velocities_encoders();

  if (TELEMETRY)
  {
    print_telemetry(current_millis);
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
  if (!SOFT_CALLIBRATION)
  {
    hard_reset_sensors();
  }
  else
  {
    /* reset callibration values (needed for callibration) */
    reset_calibration_values();
  }

  /* reset pid */
  reset_pid();

  /* reset readings for turbo calculation */
  reset_prop_readings();

  /* led: setting async period */
  set_led_blink_period(LED1, LED_BLINK_PERIOD);
  set_led_blink_period(LED2, LED_BLINK_PERIOD);

  /* enable sensors */
  enable_line_sensors();

  /* setup jukebox */
  jukebox_setup();

  /* Setup keypad */
  uint32_t button_port_array[NUM_BUTTONS] = {(uint32_t)BUTTON0_PORT,
                                             (uint32_t)BUTTON1_PORT,
                                             (uint32_t)BUTTON2_PORT};
  uint16_t button_pin_array[NUM_BUTTONS] = {(uint16_t)BUTTON0_PIN,
                                            (uint16_t)BUTTON1_PIN,
                                            (uint16_t)BUTTON2_PIN};
  keypad_setup(get_millisecs_since_start(),
               button_port_array,
               button_pin_array);

  if (FLAG_CIRCUIT_MAPPING)
  {
    reset_circuit_mapping();
  }

  if (FLAG_MAX_VEL_DELAY)
    reset_veldelay();

  set_led_mode(LED1, ON);
  set_led_mode(LED2, OFF);

  ////////////////////////////////////////////////////////////////////////
  // MAIN LOOP
  ////////////////////////////////////////////////////////////////////////

  uint32_t last_loop_execution_ms = 0;

  while (1)
  {
    uint32_t current_loop_millisecs = get_millisecs_since_start();
    sync_iterations += 1;

    if (is_command_received())
    {
      execute_command();
    }

    /* should read battery? 
       battery measurement could have a different period than sensor/pid reads
     */
    if (current_loop_millisecs % VBATT_SYS_BETWEEN_READS == 0)
    {
      // Check if battery drained
      if (has_batt_drained())
      {
        //update_state(OUT_OF_BATTERY_EVENT);
      }
    }

    state_e current_state = get_state();

    music_update(current_loop_millisecs);
    keypad_loop(current_loop_millisecs);
    menu_functions(current_loop_millisecs);
    dma_update();
    leds_update(current_loop_millisecs);

    // loop out of period
    if (current_state == SET_NORMAL_MODE_STATE)
    {
      set_target_as_normal();
      /* change pid normal */
      reset_pids_normal();
      set_state(RUNNING_STATE); //FIXME this assignment is local (and useless)
      set_running_state(RUNNING_NORMAL);

      set_led_mode(LED1, ON);
      // reset variables used for special acc/dec in NORMAL mode
      reset_sequential_readings();
    }
    else if (current_state == SET_TURBO_MODE_STATE)
    {
      set_target_as_turbo();
      /* change pid consts */
      reset_pids_turbo();
      set_state(RUNNING_STATE); //FIXME this assignment is local (and useless)
      set_running_state(RUNNING_STLINE);
      set_led_mode(LED1, OFF);
    }
    else if (current_state == SET_NOOL_MODE_STATE)
    {
      set_target_as_nool();

      /* change pid consts */
      reset_pids_nool();
      set_state(RUNNING_STATE);
      set_running_state(RUNNING_NOOL);
      set_led_mode(LED1, BLINK);
    }

    // loop is executed at a fixed period of time
    if ((current_loop_millisecs - last_loop_execution_ms) >= TIME_BETWEEN_LOOP_ITS)
    {

      last_loop_execution_ms = current_loop_millisecs;

      /* read data from sensors */
      read_line_sensors(line_sensor_value);

      switch (current_state)
      {
      case CALLIBRATION_STATE:
        calibration_state();
        break;
      case IDLE_STATE:
        idle_state();
        break;
      case NO_BATTERY_STATE:
        out_of_battery_state();
        break;
      case DELAYED_START_STATE:
        delayed_start_state(current_loop_millisecs);
        break;
      case PIDANDVEL_MAPPING_STATE:
        pid_and_vel_mapping_state(current_loop_millisecs);
        break;
      case PIDANDVEL_CHANGE_STATE:
        pid_and_vel_change_state(current_loop_millisecs);
        break;
      default:
        running_state(current_loop_millisecs);
      }
    }

    return 0;
  }
}
