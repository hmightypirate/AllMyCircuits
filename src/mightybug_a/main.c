#include "main.h"

uint16_t line_sensor_value[NUM_SENSORS];
uint32_t sync_iterations = 0;
uint32_t current_loop_millisecs = 0;

uint32_t pidvel_map_ms = 0;

void set_pidvel_map_time(uint32_t current_time);
uint32_t get_pidvel_map_time(void);

void keypad_events(void)
{
  if (button_released(BUTTON1))
  {
    update_state(BUTTON1_RELEASED_EVENT);
  }

  if (button_pressed(BUTTON2))
  {
    update_state(BUTTON2_PRESSED_EVENT);
  }

  if (button_pressed(BUTTON3))
  {
    update_state(BUTTON3_PRESSED_EVENT);
  }
}

void check_rn_state(void)
{
  rnstate_e running_state = get_running_state();

  if (!is_out_of_line())
  {
    jukebox_setcurrent_song(NO_SONG);
  }

  switch (running_state)
  {
  case RUNNING_STLINE:
    set_target_as_turbo();
    reset_pids_turbo();
    if (TURBO_PITCH_DEBUG)
      jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
    set_led_mode(LED_1, OFF);
    break;
  case RUNNING_NORMAL:
    set_target_as_normal();
    reset_pids_normal();
    if (TURBO_PITCH_DEBUG)
      jukebox_setcurrent_song(TENOR_BEAT_ORDER);
    set_led_mode(LED_1, ON);
    // reset variables used for special acc/dec in NORMAL mode
    reset_sequential_readings();
    break;
  case RUNNING_NOOL:
    set_target_as_nool();
    reset_pids_nool();
    if (TURBO_PITCH_DEBUG)
      jukebox_setcurrent_song(BASS_BEAT_ORDER);
    set_led_mode(LED_1, BLINK);
    break;
  default:
    set_target_as_normal();
    reset_pids_normal();
    if (TURBO_PITCH_DEBUG)
      jukebox_setcurrent_song(SONG_TWO_BEAT_ORDER);
    set_led_mode(LED_1, ON);
  }

  if (is_out_of_line())
  {
    jukebox_setcurrent_song(OUT_OF_LINE_SONG);
  }
}

void music_update(void)
{
  jukebox_update(current_loop_millisecs);
}

void calibration_state(void)
{
  read_line_sensors(line_sensor_value);

  calibrate_sensors(line_sensor_value);

  /* stop motors during calibration */
  stop_motors();
  /* led is on during calibration */
  set_led_mode(LED_2, ON);

  jukebox_setcurrent_song(CALIBRATION_SONG);
}

void idle_state(void)
{
  /* reset out of line measurements, resets calibration */
  reset_all_inline();
  reset_calibration_values();
  stop_motors();

  /* Clear led during idle state */
  set_led_mode(LED_2, OFF);
}

void out_of_battery_state(void)
{
  /* Disable sensors */
  disable_line_sensors();

  /* Stop motors */
  stop_motors();

  /* Led off */
  set_led_mode(LED_2, OFF);

  jukebox_setcurrent_song(OUT_OF_BATTERY_SONG);
}

void delayed_start_state(void)
{
  /* Stop motors */
  stop_motors();

  if (current_loop_millisecs - get_delay_start_time() > DELAYED_START_MS)
  {
    // Reset pointer (starting from the beginning)
    if (FLAG_CIRCUIT_MAPPING)
      reset_mapping_pointer();

    if (FLAG_MAX_VEL_DELAY)
      reset_veldelay();
    reset_encoder_ticks();
    update_state(DELAYED_START_TIMEOUT_EVENT);
  }

  /* Led on */
  set_led_mode(LED_2, ON);
}

void info_map_state(void)
{
  /* stop motors */
  stop_motors();

  if (get_current_pidvel_map() == 0)
  {
    set_led_mode(LED_1, BLINK);
    set_led_mode(LED_2, BLINK);
  }
  else if (get_current_pidvel_map() == 1)
  {
    set_led_mode(LED_1, DOUBLE_BLINK);
    set_led_mode(LED_2, DOUBLE_BLINK);
  }
  else if (get_current_pidvel_map() == 2)
  {
    set_led_mode(LED_1, TRIPLE_BLINK);
    set_led_mode(LED_2, TRIPLE_BLINK);
  }

  jukebox_setcurrent_song(get_map_song(get_current_pidvel_map()));

  if (current_loop_millisecs - get_pidvel_map_time() > DELAYED_PIDVEL_CHANGE_MS)
  {
    stop_music_play();
    pull_enable_jukebox();
    update_state(CHANGE_MAP_TIMEOUT_EVENT);
  }
}

void change_map_state(void)
{
  select_next_pidvel_map();
  reset_pids_normal();
  set_pidvel_map_time(current_loop_millisecs);
  update_state(CHANGED_MAP_EVENT);
}

void select_running_state(int error)
{
  if (sync_iterations % TIME_BETWEEN_STORE_POS == 0)
  {
    set_new_reading(error);

    if (!USE_ENCODERS_FOR_STATE)
    {

      // Check that the minimum number of readings was performed
      if (is_enable_avg_readings())
      {
        // Obtain the average number of readings
        int16_t avg_error = get_avg_abs_readings();

        get_next_running_state(avg_error);
      }
    }
    else
    {
      get_next_running_state(get_abs_diff_encoders());
    }
  }
}

void acceleartion_and_brake_control(void)
{
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

  if (FLAG_ANTI_WHEELIE_START)
  {
    set_vel_antiwheelie(current_loop_millisecs);
  }
}

void stop_state()
{
  // stop the motors if out of line
  stop_motors();

  // led off
  set_led_mode(LED_2, OFF);

  // Send car to calibration if reached the end of line
  if (get_all_inline())
  {
    update_state(ALL_SENSORS_IN_LINE_EVENT);
  }
}

void just_run_state(int control)
{

  motor_control(control);

  // blinking: normal behaviour
  set_led_mode(LED_2, BLINK);

  // Set the current ms (inline)
  if (!is_out_of_line())
  {
    update_ms_inline(current_loop_millisecs);
  }

  // Do circuit mapping
  if (FLAG_CIRCUIT_MAPPING)
  {
    do_circuit_mapping();
  }
}

bool stop_conditions(void)
{
  /* motor control 

	          stops if:
	          1) is out of line and not delay is used
	          2) is out of line and has exceeded the maximum time allowed
	          3) the time of an inertia test is over

	*/

  return ((is_out_of_line() && !DEBUG_INERTIA_TEST &&
           (!FLAG_DELAY_STOP_OUT_OF_LINE ||
            (FLAG_DELAY_STOP_OUT_OF_LINE &&
             exceeds_time_out_of_line(current_loop_millisecs)))) ||
          (DEBUG_INERTIA_TEST && (current_loop_millisecs - get_running_ms() > DEBUG_INERTIA_TIME_MS)));
}

void running_state(void)
{
  sync_iterations += 1;

  read_line_sensors(line_sensor_value);

  // Running
  int error = get_line_position(line_sensor_value);

  // update consecutive iterations increasing or decreasing error
  update_sequential_readings(error, get_last_error());

  // check with encoders or line position if we are in st_line or not
  select_running_state(error);
  // set running parameters according to running state (music, leds, velocities, ...)
  check_rn_state();

  // Accelerate/Break in NORMAL mode and antiwheelie
  acceleartion_and_brake_control();

  if (stop_conditions())
  {
    stop_state();
  }
  else
  {
    /* pid control */
    int control = 0;
    control = pid(error);
    just_run_state(control);
  }

  // update encoders velocity
  update_velocities_encoders();

  if (TELEMETRY)
  {
    print_telemetry(current_loop_millisecs);
  }
}

void setup_keypad(void)
{
  keypad_setup(current_loop_millisecs);
}

void check_command(void)
{
  if (is_command_received())
  {
    execute_command();
  }
}

void update_modules(void)
{
  music_update();
  keypad_update();
  keypad_events();
  dma_update();
  leds_update();
}

void set_car_default_parameters(void)
{
  reset_pids_normal();

  if (!SOFT_CALIBRATION)
  {
    hard_reset_sensors();
  }
  else
  {
    /* reset calibration values (needed for calibration) */
    reset_calibration_values();
  }

  /* reset pid */
  reset_pid();

  /* reset readings for turbo calculation */
  reset_prop_readings();

  if (FLAG_CIRCUIT_MAPPING)
  {
    reset_circuit_mapping();
  }

  if (FLAG_MAX_VEL_DELAY)
    reset_veldelay();
}

void setup_modules()
{
  /* led: setting async period */
  set_led_blink_period(LED_1, LED_BLINK_PERIOD_DEFAULT);
  set_led_blink_period(LED_2, LED_BLINK_PERIOD_DEFAULT);

  /* enable sensors */
  enable_line_sensors();

  /* setup jukebox */
  jukebox_setup();

  /* Setup keypad */
  setup_keypad();
}

void execute_state(state_e state)
{
  switch (state)
  {
  case RUNNING_STATE:
    running_state();
    break;
  case CALIBRATION_STATE:
    calibration_state();
    break;
  case IDLE_STATE:
    idle_state();
    break;
  case OUT_OF_BATTERY_STATE:
    out_of_battery_state();
    break;
  case DELAYED_START_STATE:
    delayed_start_state();
    break;
  case CHANGE_MAP_STATE:
    change_map_state();
    break;
  case INFO_MAP_STATE:
    info_map_state();
  default:
    running_state();
  }
}

void check_battery(void)
{
  if (current_loop_millisecs % VBATT_TIME_BETWEEN_READS == 0)
  {
    if (is_vbatt_drained())
    {
      update_state(OUT_OF_BATTERY_EVENT);
    }
  }
}

/*
 * @brief sets the time entering a pid/vel mapping state
 * 
 */
void set_pidvel_map_time(uint32_t current_time)
{
  pidvel_map_ms = current_time;
}

/*
 * @brief obtain the time entering a pid/vel mapping state
 * 
 * Used to wait x seconds before enable changing pidvel map again
 */
uint32_t get_pidvel_map_time(void)
{
  return pidvel_map_ms;
}

/*
 * @brief Initial setup and main loop
 */
int main(void)
{
  uint32_t last_loop_millisecs = 0;

  setup_microcontroller();
  set_car_default_parameters();
  setup_modules();

  while (true)
  {
    current_loop_millisecs = get_millisecs_since_start();

    check_command();
    check_battery();
    update_modules();

    // inner loop is executed at a fixed period of time
    if ((current_loop_millisecs - last_loop_millisecs) >= FIXED_LOOP_TIME)
    {
      last_loop_millisecs = current_loop_millisecs;
      execute_state(get_state());
    }
  }

  return 0;
}
