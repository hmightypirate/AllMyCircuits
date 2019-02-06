#include "main.h"

uint16_t line_sensor_value[NUM_SENSORS];
uint32_t sync_iterations = 0;
uint32_t current_loop_millisecs = 0;

uint32_t pidvel_map_ms = 0;

void get_next_running_state(int16_t avg_error);

uint32_t last_ms_inline = 0;
uint32_t running_loop_millisecs = 0;

// Variables for handling target velocity in normal mode
static uint16_t seq_decrease_line_pos = 0;
static uint16_t seq_increase_line_pos = 0;

/* 
 * @brief obtains the aggregate number of pos readings improving/decreasing line position
 *
 */
void update_sequential_readings(int16_t error, int16_t last_error)
{
  if (error < 0)
  {
    error = -error;
  }

  if (last_error < 0)
  {
    last_error = -last_error;
  }

  if (error > last_error)
  {
    seq_increase_line_pos += 1;
    seq_decrease_line_pos = 0;
  }
  else
  {
    seq_decrease_line_pos += 1;
    seq_increase_line_pos = 0;
  }
}

void reset_sequential_readings(void)
{
  seq_decrease_line_pos = 0;
  seq_increase_line_pos = 0;
}

/* 
 * @brief updates the target velocity
 *
 */
void update_target_normal()
{
  /* only works in normal mode */
  if (get_running_state() == RUNNING_NORMAL)
  {
    if ((seq_decrease_line_pos > DEC_NORMAL_THRESHOLD) && (get_target_velocity() < MAX_VEL_MOTOR_DEC_MODE))
    {
      set_target_velocity(get_target_velocity() + DEC_NORMAL_QTY);
      if (RESET_DEC_AFTER_SET)
      {
        seq_decrease_line_pos = 0;
      }
    }
    else if ((seq_increase_line_pos > INC_NORMAL_THRESHOLD) && (get_target_velocity() > MIN_VEL_MOTOR_INC_MODE))
    {
      set_target_velocity(get_target_velocity() + INC_NORMAL_QTY);
      if (RESET_INC_AFTER_SET)
      {
        seq_increase_line_pos = 0;
      }
    }
  }
}

/* 
 * @brief updates the target velocity
 *
 */
void update_target_normal_with_encoders()
{

  /* only works in normal mode */
  if (get_running_state() == RUNNING_NORMAL)
  {
    int32_t diff_acc = 0;
    // is left wheel running faster than right wheel
    if (get_left_encoder_ticks() > get_right_encoder_ticks())
    {
      // right wheel is acc faster ?
      diff_acc = get_right_acc() - get_left_acc();
    }
    else
    {
      diff_acc = get_left_acc() - get_right_acc();
    }

    int32_t step_qty = 0;

    if (diff_acc > 0)
    {
      step_qty = STEP_NORMAL_QTY_INC;
    }
    else
    {
      step_qty = STEP_NORMAL_QTY_DEC;
    }

    int32_t next_vel = vel_maps[get_current_pidvel_map()] + step_qty * diff_acc;

    /*
      if (next_vel < MIN_VEL_MOTOR_INC_MODE)
	{
	  next_vel = MIN_VEL_MOTOR_INC_MODE;
	}
      else if (next_vel > MAX_VEL_MOTOR_DEC_MODE)
	{
	  next_vel = MAX_VEL_MOTOR_DEC_MODE;
	}
      */

    set_target_velocity(next_vel);
  }
}

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

void turbo_running_state()
{
  set_target_as_turbo();
  reset_pids_turbo();
  jukebox_setcurrent_song(NO_SONG);
  if (TURBO_PITCH_DEBUG)
    jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
  set_led_mode(LED_1, OFF);
}

void normal_running_state()
{
  set_target_as_normal();
  reset_pids_normal();
  jukebox_setcurrent_song(NO_SONG);
  if (TURBO_PITCH_DEBUG)
    jukebox_setcurrent_song(TENOR_BEAT_ORDER);
  set_led_mode(LED_1, ON);
  // reset variables used for special acc/dec in NORMAL mode
  reset_sequential_readings();
}

void nool_running_state()
{
  set_target_as_nool();
  reset_pids_nool();
  jukebox_setcurrent_song(NO_SONG);
  if (TURBO_PITCH_DEBUG)
    jukebox_setcurrent_song(BASS_BEAT_ORDER);
  set_led_mode(LED_1, BLINK);
}

void stop_running_state()
{
  stop_motors();
  jukebox_setcurrent_song(OUT_OF_LINE_SONG);
}

void recovery_running_state()
{
  jukebox_setcurrent_song(OUT_OF_LINE_SONG);
}

void check_rn_state(void)
{
  rnstate_e running_state = get_running_state();

  switch (running_state)
  {
  case RUNNING_STLINE:
    turbo_running_state();
    break;
  case RUNNING_NORMAL:
    normal_running_state();
    break;
  case RUNNING_NOOL:
    nool_running_state();
    break;
  case RUNNING_RECOVERY:
    recovery_running_state();
    break;
  case RUNNING_STOP:
  default:
    stop_running_state();
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
  static uint32_t delayed_start_time = 0;

  if (delayed_start_time == 0)
  {
    if (get_calibrated_sensors_count() < NUM_SENSORS - MAX_NUM_NOT_CALLIBRATED_SENSORS)
    {
      update_state(FORCE_CALIBRATION_EVENT);
      return;
    }
    delayed_start_time = get_millisecs_since_start();
  }

  /* Stop motors */
  stop_motors();

  if (current_loop_millisecs - delayed_start_time > DELAYED_START_MS)
  {
    delayed_start_time = 0;

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

  if (current_loop_millisecs - pidvel_map_ms > DELAYED_PIDVEL_CHANGE_MS)
  {
    stop_music_play();
    pull_enable_jukebox();
    update_state(CHANGE_MAP_TIMEOUT_EVENT);
  }
}

void change_map_state(void)
{
  select_next_pidvel_map();
  pidvel_map_ms = current_loop_millisecs);
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

/*
 * @brief change velocity to avoid wheelie at start
 */
void set_vel_antiwheelie(uint32_t current_loop_millisecs)
{
  if ((current_loop_millisecs - running_loop_millisecs) < MAX_DURATION_WHEELIE_START)
  {
    set_target_velocity(MAX_VEL_WHEELIE_START);
  }
  else
  {
    if (get_running_state() == RUNNING_STLINE)
    {
      set_target_velocity(vel_turbo_maps[get_current_pidvel_map()]);
    }
    else if (get_running_state() == RUNNING_NOOL)
    {
      set_target_velocity(vel_nool_maps[get_current_pidvel_map()]);
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
    last_ms_inline = current_loop_millisecs;
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
             ((current_loop_millisecs - last_ms_inline) > MS_DELAY_OUT_OF_LINE)))) ||
          (DEBUG_INERTIA_TEST && (current_loop_millisecs - running_loop_millisecs > DEBUG_INERTIA_TIME_MS)));
}

void running_state(void)
{
  static uint32_t running_loop_millisecs = 0;

  if (running_loop_millisecs == 0)
  {
    running_loop_millisecs = get_millisecs_since_start();
  }
  if (current_loop_millisecs - running_loop_millisecs > MAX_DURATION_WHEELIE_START)
  {
    running_loop_millisecs = 0;
  }

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

/*
 * @brief get next sub-state (running)
 *
 */
void get_next_running_state(int16_t avg_error)
{

  if (get_running_state() == RUNNING_NORMAL)
  {
    if (ENABLE_TURBO_MODE && (avg_error < OUT_NORMAL_HYST))
    {
      update_running_state(SET_TURBO_MODE_STATE);
    }
    else if (ENABLE_NOOL_MODE && (avg_error > OUT_NORMAL_NOOL_HYST))
    {
      update_running_state(SET_NOOL_MODE_STATE);
    }
  }
  else if (get_running_state() == RUNNING_STLINE)
  {
    if (avg_error > OUT_TURBO_HYST)
    {
      update_running_state(SET_NORMAL_MODE_STATE);
    }
  }
  else if (get_running_state() == RUNNING_NOOL)
  {
    if (avg_error < OUT_NOOL_NORMAL_HYST)
    {
      update_running_state(SET_NORMAL_MODE_STATE);
    }
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
